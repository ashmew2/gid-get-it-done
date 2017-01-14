#include <memory>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include <fstream>
#include <string>

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

/* Berkeley */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

static volatile sig_atomic_t sigusr1_caught = 0;
static volatile sig_atomic_t sigusr2_caught = 0;

static std::mutex theLock;
std::vector<int> t;
std::map<int, std::thread> m;

/* TODO: Replace with unix socket */
std::string GID_PIPE_FILE="/tmp/timer";
int LISTEN_PORT = 42000;
bool tcplistener_running;

/* Read gidserver.conf at runtime and reload with SIGHUP */

/* TODO: Implement warn(), error(), log(), debug() and supress output (and code?) in production builds */

/* TODO: What happens if gidserver tries to execute gidserver? ;) */
/* TODO: exec_cmd does not seem to play nicely with mpg123
         (possibly, with some other interactive thingies as well) */
std::string exec_cmd(std::string cmd) {
    char buffer[128];
    std::string result="";
    std::string f;

    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe) throw std::runtime_error("popen failed.");

    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) {
            result += buffer;
            std::cout << buffer;
            }
    }

    return result;
}


/* Our format for inputline is : <timeout><space><commands><;> */
/* We guarantee that the timeoutstr and cmdstr are not modified if parsing fails */
bool parse_time_cmd(std::string& inputline, long& timeout, std::string& cmdstr) {

  /* Ensure only empty cmdstr is passed so that we parse and fill it */
  if(cmdstr != "") {
    std::cout << "[WARNING]: Failed parse. cmdstr: [E0]" << cmdstr << std::endl;
    return false;
  }

  /* Stick to inputline format */
  if(inputline == "" || inputline[inputline.length() - 1] != ';' ||
     inputline[0] == '-') {

    std::cout << "[WARNING]: Failed to parse line : [E1] " << inputline << std::endl;
    return false;
  }

  size_t end_of_timeout = inputline.find(" ");

  if(end_of_timeout == std::string::npos || end_of_timeout == 0 ||
     end_of_timeout == inputline.length() - 1) {

    /* No proper timeout = whine and bail */
    std::cout << "[WARNING]: Failed to parse line : [E2] " << inputline << std::endl;
    return false;
  }
  else {
    /* Make sure that all digits till space are numbers */
    size_t i;
    for(i = 0; i < end_of_timeout; i++)
      if(!isdigit(inputline[i]))
        break;

    if(i!=end_of_timeout) {
      std::cout << "[WARNING]: Failed to parse line : [E3] " << inputline << std::endl;
      return false;
    }

    std::string timeoutstr = inputline.substr(0, end_of_timeout);
    long timeout_ = 0;
    try {
      timeout_ = stol(timeoutstr, nullptr, 10);
    }
    catch(const std::exception& fucked) { return false; }

    timeout=timeout_;
    cmdstr = inputline.substr(end_of_timeout+1);
    cmdstr.erase(cmdstr.end()-1);
  }

  /* TODO: Curse the client who put this damn line into our input feed */

  return true;
}

std::string execute_if_sane(std::string cmd_to_execute) {
    if(cmd_to_execute == "adb-pull") {
      return exec_cmd("$HOME/gid/adb-pull.sh");
    }
    else if(cmd_to_execute == "adb-push") {
      return exec_cmd("$HOME/gid/adb-push.sh");
    }

    /*TODO: Possibly can be fixed, but here's the situation:
            If we execute gidserver as ./gidserver& (aka in the background)
            then the mpg123 execution from playalarm.sh blocks and puts gidserver
            in a stopped state. After we do a fg %1 and wake it up, the alarm is
            sounded. This can be fixed by running server as ./gidserver and use another
            terminal to send the USR1 signals (or run tda --timer 1 alarm).
    */

    else if(cmd_to_execute == "alarm") {
      system("$HOME/gid/playalarm.sh 2>/dev/null");
      return "";
    }
    else {
        return "UNDEFINED (for now)";
    }
}

void timeout(int time_in_sex, int thread_id, std::string cmd_to_execute) {

    std::cout << "T[" << thread_id << "]: Sleep for : " << time_in_sex << " seconds and execute : " << cmd_to_execute << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(time_in_sex));

    // Do what we want to do here, like play an alarm or trigger the phone
    // Or call the phone. Each thread should have a final purpose that gid.sh set for it
    // using USR1

    std::cout << "T[" << thread_id << "] Executing command:" << cmd_to_execute << std::endl;
    std::cout << "T[" << thread_id << "] result is : " << execute_if_sane(cmd_to_execute) << std::endl;

    theLock.lock();
    t.push_back(thread_id);
    theLock.unlock();
}

/* TODO - Check how the children handle these signals. Should we uninstall the signal handler for children threads? */

void handle_sighup(int signum) {

    /* TODO - gidserver.conf should be placed inside ~/.gid (decided at install time) */

    std::ifstream conf("$HOME/gid/gidserver.conf");
    std::string filepath;

    if(!conf)
        return;

    getline(conf, filepath);

    GID_PIPE_FILE=filepath;

    conf.close();
}

void handle_sigusr1(int signum) {

    sigusr1_caught=1;
}

void handle_sigusr2(int signum) {

  sigusr2_caught=1;
}

void tcp_client_handler(int clientfd) {

  char msgbuf[73];
  std::string input, cmdstr;
  long timeout;

  while(true) {
    /* TODO: Is recv() thread safe? */
    ssize_t bytes_received = recv(clientfd, msgbuf, sizeof(msgbuf), 0);

    if(bytes_received < 0) {
      /* Kick client? */
      std::cout << "Error with recv(). " << std::endl;
    }
    else {
      int i;

      for(i = 0; i < bytes_received; i++) {
        input+=msgbuf[i];

        if(msgbuf[i] == ';') {
          if (parse_time_cmd(input, timeout, cmdstr)) {
            /* Create a thread and maintain a different pool of threads */
          }

          input.clear();
        }
      }

      /* If we received more data than one command, store it */
      if(i<bytes_received) {
        std::string remainder;

        for(i = 0; i < bytes_received; i++) {
          remainder+=msgbuf[i];
          if(msgbuf[i] == ';')
            break;
        }
      }
    }
  }
}

void tcp_server_process(int conn_backlog) {

    /* Create the tcp listener if it doesn't exist. */

    theLock.lock();
    if(tcplistener_running == true) {
      /* TODO: Do health checks? */
      theLock.unlock();
      return;
    }

    /* If still here, we haven't released the lock */
    tcplistener_running = true;
    theLock.unlock();

    /* TODO: Extend to multiple listeners on several ports (with configurability) */
    int listenfd=-1;
    struct sockaddr_in listenaddr;

    /* use SEQPACKET instead of STREAM ? */
    listenfd=socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd < 0) {
    std::cout << "Could not get a socket. Aborting.";
    exit(1);
    }

    /* Clear the struct instance */
    memset(&listenaddr, 0, sizeof(struct sockaddr_in));

    /* Set address specifics of our listener */
    listenaddr.sin_family = AF_INET;
    listenaddr.sin_port   = LISTEN_PORT;
    inet_aton("127.0.0.1", &(listenaddr.sin_addr));
    //listenaddr.sin_addr.s_addr = (127 << 24) + (0 << 16) + (0 << 8) + (1 << 0);

    if(bind(listenfd, (struct sockaddr *) &listenaddr, sizeof(struct sockaddr_in)) < 0) {
      std::cout << "bind failed. Going down..." << std::endl;
      exit(2);
    }

    if(listen(listenfd, 1) < 0) {
      std::cout << "listen failed. Bailing out.." << std::endl;
      exit(3);
    }

    std::cout << "Now accepting connections on " << LISTEN_PORT << std::endl;

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size = sizeof(struct sockaddr_in);

    int failures = 0;
    while(true) {

      /* TODO: Set soft and hard client limits to prevent being flooded */
      int clientfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_size);

      if(clientfd < 0) {
        failures += 1;

        if(failures >= 10) {
          std::cout << "Too many failures with accept() returning bogus FDs. Going down." << std::endl;
          exit(4);
        }
      }

      /* Valid clientfd here */
      /* Enable a simple hello cipher here : TODO */

      std::thread(tcp_client_handler, clientfd);
    }

    /* TODO: Fix -Wsign-compare warnings to shut up gcc for good ;-) */

    theLock.lock();
    /* TODO: Also set this to  false when killing tcp server. */
    tcplistener_running = false;
    std::cout << "Set listener to false!" << std::endl;
    theLock.unlock();
}


/* TODO: Replace communication with a nixock instead */
int main() {

    signal(SIGHUP,  handle_sighup);
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    std::thread thread_tcp_listener = std::thread(tcp_server_process, 2);

    /* TODO [ADVANCED]: Can we get rid of this polling altogether? */
    int POLL_INTERVAL = 1;
    int tid = 0;
    int number_of_lines_read = 0;

    /* TODO - this wont scale for large and will overflow eventually */
    /* TODO - create gid config so that the filenames in sh and c stay the same for gidpiping */
    /* TODO - nixock */

    std::string inputline;
    std::string str_cmd="";
    long time_for_task;

    /* TODO: Remove this infinite loop */
    while (1) {
      std::this_thread::sleep_for(std::chrono::seconds(POLL_INTERVAL));

      /* TODO - add a counter for ignoring client file if clients misbehaves */
      if(sigusr1_caught == 1) {

        sigusr1_caught = 0;

        /* TODO: Save offset in the file rather than number of lines. Replace this painfully
                 inefficient way to step through lines in the input. */

        /* very bad way to get this done */
        std::ifstream input(GID_PIPE_FILE);
        if (!input) {
          std::cout << "[ERROR] Can't open GID_PIPE_FILE: " << GID_PIPE_FILE << std::endl;
          exit(-2);
        }

        /* std::cout << "number of lines read: " << number_of_lines_read << std::endl; */
        for(int i = 0; i < number_of_lines_read && getline(input, inputline); i++);

        /* time and cmd get */
        getline(input, inputline);

        if(inputline.length() == 0) {
          std::cout << "[WARNING]: SIGUSR1 received but no lines to read." << std::endl;
          continue;
        }

        number_of_lines_read++;

        if(parse_time_cmd(inputline, time_for_task, str_cmd) == true) {

          std::cout << "Creating thread." << std::endl;
          ++tid;
          m[tid]=std::thread(timeout, time_for_task, tid, str_cmd);
        }
        else {
          std::cout << "Failed to parse : " << inputline << std::endl;
        }
      }

      theLock.lock();
      if(t.size() > 0) {
        m[t[0]].join();
        std::cout << "T[" << t[0] << "]: Reaped (and sent to hell)" << std::endl;
        m.erase(t[0]);
        t.erase(t.begin());
      }
      theLock.unlock();

      /* TODO: Replace this with TCP listener's own lock */
      theLock.lock();
      if(tcplistener_running != true) {
        std::cout << "The TCP Server has died. Reaping (and sending it to Hell)";
        thread_tcp_listener.join();
        tcplistener_running = true; /* BAD HACK, cant restart! TODO: */
      }

      theLock.unlock();
    }

    return 0;
}
