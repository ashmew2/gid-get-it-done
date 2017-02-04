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

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

static volatile sig_atomic_t sigusr1_caught = 0;
static volatile sig_atomic_t sigusr2_caught = 0;

/*********** Global management stuff for managing threads ************/
std::vector<int> threads_to_be_killed;
std::map<int, std::thread> thread_id_thread_map;
bool tcplistener_running;
static unsigned int numThreads;

std::map<int, std::string> thread_requests;
std::map<int, std::string> thread_responses;

/*********** Locks for Global management stuff we have above *********/
static std::mutex threadKillerLock;
/* Lock for map should come here */
static std::mutex tcpListenerStatusLock;
static unsigned int numThreads;

static std::mutex tcpRequestLock;
static std::mutex tcpResponseLock;
static std::mutex threadRequestLock;
static std::mutex threadResponseLock;

/*********** End of Global management + locking **********************/

/* TODO: Replace with unix socket */
std::string GID_PIPE_FILE="/tmp/timer";
int LISTEN_PORT = 42000;
// == DELIBERATE ERROR ==
int FUCKMY LIFE BITCJ;

execute(std::string cmd) {

}

void slave(std::string task, int sleeptime, bool saveout, int reqkey) {

  /* Invoke with std::thread(red, ).detach(); */

  /* What is sanity? */
  if(cmdmap.find(task) == map::end) {
    KILLED_BY_MAKER();
  }

  std::this_thread::sleep_for(std::chrono::seconds(sleeptime));
  std::string taskoutput;

  taskoutput = execute(task);

  /*inform the reader of this somehow*/
  /* Use the select loop as the menu loop of this code. Send the response from here in an FD back to select */

  if(saveout) {
    responses[reqkey] = taskoutput;

    /* Notify */
    char msg[MASTER_MSG_SIZE];

    int d;
    for(d=0; reqkey!=0; d++, reqkey=reqkey/10) {
      msg[d] = char(reqkey%10 + int('0'));
    }
    for(int i = 0; i<=d/2; i++)
      char x=msg[i], msg[i]=msg[d-i], msg[d-i]=x;

    write(mastersfd, msg, d);
  }
  
  ~thread();
}

int main() {

  /* Create all FDs here */
  int slavefd = open("/tmp/", O_TMPFILE);
  int tcplistenerfd = init_tcp_listener();
  fd_set READFDS, WRITEFDS;
  std::vector<int> tcpreq;
  std::map<int, std::string> resp;
  std::vector<int> tcpresp;

  while(true) {
    FD_ZERO(&READFDS);
    FD_ZERO(&WRITEFDS);
    FD_SET(requestfd, &READFDS);
    FD_SET(tcplistenerfd, &READFDS);

    /* Calculate nfds properly here */
    int nfds = ;

    /* Block on select */
    select(nfds, READFDS, WRITEFDS, NULL, NULL);

    /* Check for new client connection requests */
    if(FD_ISSET(tcplistenerfd, &READFDS)) {
      sockaddr_in client_addr;
      socklen_t clientlen;
      int clientfd = accept(tcplistenerfd, (struct sockaddr *)&client_addr, &clientlen);
      if(clientfd != -1) {
        tcpreq.push_back(clientfd);
      }
    }

    /* Got any new TCP requests from clients */
    if(FD_ISSET(tcpreq.forall(), &READFDS)) {
      /* Put in whatever amounts of request data we got.
         If it's the end of the request according to PROTO, we will spawn a new detached thread
         For EOF, remove this file descriptor from tcpreq..What about tcpresp?
      */
    }

    /* Got something from the slaves */
    if(FD_ISSET(slavefd, &READFDS)) {
      /* This means that some execution thread finished, time to let the client know about it. */
      tcpresp.push_back();
    }

    /* Check if we can send back responses to the TCP client that requested something. */
    if(FD_ISSET(tcpresp.all(), &WRITEFDS)) {
      /* This means that some execution thread finished, time to let the client know about it. */
      write(tcpresp.clientfd, waiting response, ahead);
    }
  }
}




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
/* We guarantee that the passed-by-reference args are not modified if parsing fails */
bool parse_time_cmd(std::string& inputline, long& timeout, std::string& cmdstr) {

/*                              TODO: Need a lock for this function                 */

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

    std::this_thread.join();
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

/* Host a contest on a whatsapp profile ( Android, sighup to whatsapp to reload its contents like photo) 
   Have a range of questions : Q1 to Q9.
   Whoever sends a message, take it and pass it to a judge to record who did it.
   Announce stats at the end.
   Repeat.
 */

void tcp_server_process(int conn_backlog) {

    /* Create the tcp listener if it doesn't exist. */

    tcpListenerStatusLock.lock();

    if(tcplistener_running == true) {

      /* TODO: Do health checks? */
      tcpListenerStatusLock.unlock();
      return;
    }

    /* If still here, we haven't released the lock */
    tcplistener_running = true;
    tcpListenerStatusLock.unlock();

    /* TODO: Extend to multiple listeners on several ports (with configurability) */
    int listenfd=-1;
    struct sockaddr_in listenaddr;

    /* TODO: use SEQPACKET instead of STREAM ? */
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

    std::cout << std::endl << "Now accepting TCP connections on " << LISTEN_PORT
              << std::endl;

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size = sizeof(struct sockaddr_in);

    int failures = 0;

    std::vector<int> connected_clients;
    std::map<int, std::string> clientfd_msg_map;
    fd_set recv_fdset;
    fd_set send_fdset;
    int max_clientfd = 0;

    FD_ZERO(&recv_fdset);
    FD_ZERO(&send_fdset);

    while(true) {

      /* TODO: Set soft and hard client limits to prevent being flooded */
      int clientfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_size);

      if(clientfd < 0) {
        failures += 1;

        if(failures >= 10) {
          std::cout << "Too many failures with accept() returning bogus FDs. Going down." << std::endl;
          exit(4);
        }

        continue;
      }
    
      /* Valid clientfd here */
      /* Enable a simple hello cipher here : TODO */

      FD_SET(clientfd, &recv_fdset);
      connected_clients.push_back(clientfd);
      clientfd_msg_map[clientfd] = "";
      
      if(max_clientfd < clientfd)
        max_clientfd = clientfd;

      struct timeval recv_tv;
      recv_tv.tv_sec = 5;
      recv_tv.tv_usec = 0;
      
      int select_ret = select(max_clientfd + 1, &recv_fdset, &send_fdset, NULL, &recv_tv);

      if(select_ret < 0)
        std::cout << "[WARNING] Select failed!" << std::endl;
      else if(select_ret > 0) {
        for(iterator::vector<int> it = connected_clients.begin(); it++; it != connected_clients.end()) {
          if (FD_ISSET(*it, &recv_fdset)) {
            char msgbuf[73];
            ssize_t bytes_received = recv(clientfd, msgbuf, sizeof(msgbuf), 0);

            if(bytes_received > 0) {

              for(i = 0; i < bytes_received; i++) {
                clientfd_msg_map[clientfd]+=msgbuf[i];

                if(msgbuf[i] == ';') {

                  long time_input;
                  if (parse_time_cmd(clientfd_msg_map[clientfd], time_input, cmdstr)) {

                    /* TODO: Create a thread and maintain a different pool of threads */

                    std::thread(timeout, clientfd, time_input, clientfd_msg_map[clientfd]);
                  }

                  clientfd_msg_map[clientfd].clear();
                }
              }
            } /*  if(bytes_received > 0) end  */
          }
        }
      }

      /* For each accepted client, we create a handler thread which will be alive till the client
         disconnects (deliberately, or we kick it) */

      std::thread(tcp_client_handler, clientfd);
    }

    /* TODO: Also set tcplistener_running to  false when killing tcp server. */
    tcpListenerStatusLock.lock();
    tcplistener_running = false;
    tcpListenerStatusLock.unlock();

    std::cout << "TCP Listener has been shut down." << std::endl;
}

/* Implement a get_bytes function to fetch (continuously at the dictated byterate, acoording to varying options TODO */

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

    /* TODO: Make an installer which installs stuff at sane places.
       So that development does not interfere with the releases and vice versa. */

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

          std::cout << "Creating [UNIX] thread." << std::endl;
          ++tid;
          thread_id_thread_map[tid]=std::thread(timeout, time_for_task, tid, str_cmd);
        }
        else {
          std::cout << "Failed to parse : " << inputline << std::endl;
        }
      }

      threadKillerLock.lock();
      if(threads_to_be_killed.size() > 0) {
        thread_id_thread_map[threads_to_be_killed[0]].join();
        std::cout << "T[" << threads_to_be_killed[0] << "]: Reaped (and sent to hell)" << std::endl;
        thread_id_thread_map.erase(threads_to_be_killed[0]);
        threads_to_be_killed.erase(threads_to_be_killed.begin());
      }
      threadKillerLock.unlock();

      /* TODO: Replace this with TCP listener's own lock */
      tcpListenerStatusLock.lock();

      if(tcplistener_running != true) {
        std::cout << "The TCP Server has died. Reaping (and sending it to Hell)";
        thread_tcp_listener.join();
        tcplistener_running = true; /* BAD HACK, cant restart! TODO: */
      }

      tcpListenerStatusLock.unlock();
    }

    return 0;
}
