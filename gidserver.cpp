#include <iostream>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <signal.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <fstream>

/* Berkeley */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <cstring>

using namespace std;

static volatile sig_atomic_t sigusr1_caught = 0;
static volatile sig_atomic_t sigusr2_caught = 0;

static mutex theLock;
vector<int> t;
map<int, std::thread> m;

/* TODO: REplace with unix socket */
std::string GID_PIPE_FILE="/tmp/timer";

bool tcplistener_running;

/* Read gidserver.conf at runtime and reload with SIGHUP */

/* TODO: What happens if gidserver tries to execute gidserver? ;) */
std::string exec(string cmd) {

    char buffer[128];
    string result="";
    string f;

    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe) throw std::runtime_error("popen failed.");

    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) {
            result += buffer;
            cout<<buffer;
            }
        }

    return result;
    }

std::string execute_if_sane(std::string cmd_to_execute) {
    if (cmd_to_execute=="adb-pull") {
        return exec("$HOME/gid/adb-pull.sh");
        }
    else {
        return "UNDEFINED (for now)";
        }
    }

void timeout(int time_in_sex, int thread_id, string cmd_to_execute) {

    cout<<"dreamer " <<thread_id<<": Created for : "<<time_in_sex<<" seconds"<<endl;
    this_thread::sleep_for(chrono::seconds(time_in_sex));
    cout<<"dreamer " <<thread_id<<": woke up"<<endl;

    // Do what we want to do here, like play an alarm or trigger the phone
    // Or call the phone. Each thread should have a final purpose that gid.sh set for it
    // using USR1

    cout<<"Executing command: " << cmd_to_execute<<endl;
    cout<<"result is : " << execute_if_sane(cmd_to_execute);

    theLock.lock();
    t.push_back(thread_id);
    theLock.unlock();
    }

/* TODO - Check how the children handle these signals. Should we uninstall the signal handler for children threads? */

void handle_sighup(int signum) {

    /* TODO - gidserver.conf should be placed inside ~/.gid (decided at install time) */

    ifstream conf("$HOME/gid/gidserver.conf");
    std::string filepath;

    if(conf == NULL)
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
    cout<<"Could not get a socket. Aborting.";
    exit(1);
    }

    /* Clear the struct instance */
    memset(&listenaddr, 0, sizeof(struct sockaddr_in));

    /* Set address specifics of our listener */
    listenaddr.sin_family = AF_INET;
    listenaddr.sin_port   = 42000;
    inet_aton("127.0.0.1", &(listenaddr.sin_addr));
    //listenaddr.sin_addr.s_addr = (127 << 24) + (0 << 16) + (0 << 8) + (1 << 0);

    if(bind(listenfd, (struct sockaddr *) &listenaddr, sizeof(struct sockaddr_in)) < 0) {
      cout<<"bind failed. Going down..."<<endl;
      exit(2);
    }

    if(listen(listenfd, 1) < 0) {
      cout<<"listen failed. Bailing out.."<<endl;
      exit(3);
    }

    cout<<"Now accepting connections on listenfd"<<endl;

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size = sizeof(struct sockaddr_in);
    int clientfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_size);

    if(clientfd < 0) {
      cout<<"screwed up with accept()...dying.."<<endl;
      exit(4);
    }

    std::string msg_hello_client = "Your journey begins here.\n";

    /* Enable a simple hello cipher here : TODO */
    int bytes_sent = send(clientfd, msg_hello_client.c_str(), msg_hello_client.length(), 0);

    if(bytes_sent < msg_hello_client.length()) {
      cout<<"send() is not reliable...dying.."<<endl;
      exit(5);
    }

    cout<<"Going down with the ship. Goodbye, Your Captain"<<endl;

    theLock.lock();
    /* TODO: Also set this to  false when killing tcp server. */
    tcplistener_running = false;
    cout<<"Set listener to false!"<<endl;
    theLock.unlock();
}

/* TODO: Replace communication with a pipe instead */
    int main() {

    signal(SIGHUP,  handle_sighup);
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    std::thread thread_tcp_listener = std::thread(tcp_server_process, 2);

    int POLL_INTERVAL = 1;
    int tid = 0;
    int number_of_lines_read = 0;

    /* TODO - this wont scale for large and will overflow eventually */
    /* TODO - create gid config so that the filenames in sh and c stay the same for gidpiping */
    /* TODO - nixock */

    std::string timeline;
    std::string cmdline;

    /*TODO: Remove this infinite loop */
    while (1) {
      this_thread::sleep_for(chrono::seconds(POLL_INTERVAL));

      cout<<"Main thread woke up from sleep.."<<endl;

      /* TODO - add a counter for ignoring client file if client misbehaves */
      if(sigusr1_caught==1) {

        sigusr1_caught=0;

        /* very bad way to get this done */
        std::ifstream input(GID_PIPE_FILE);
        if (input == NULL) {
          cout << "fuckedup with GID_PIPE_FILE: " << GID_PIPE_FILE << endl;
          exit(-2);
        }

        cout<<"number of lines read: "<< number_of_lines_read<<endl;
        for(int i = 0; i < number_of_lines_read && getline(input, timeline); i++);

        /* time get */
        getline(input, timeline);

        if(timeline.length() == 0)
          continue;

        number_of_lines_read++;

        int time_for_task=atoi(timeline.c_str());
        /* todo; special case 0? */

        /* cmd get */
        getline(input, cmdline);
        input.close();

        if(cmdline.length() == 0)
          continue;

        number_of_lines_read++;
        ++tid;
        m[tid]=std::thread(timeout, time_for_task, tid, cmdline);
      }

      theLock.lock();
      if (t.size() > 0) {
        m[t[0]].join();
        cout<<"dreamer " <<t[0]<<": dead.."<<endl;
        m.erase(t[0]);
        t.erase(t.begin());
      }

      theLock.unlock();

      theLock.lock();
      if(tcplistener_running != true) {
        cout<<"The TCP Server has died..Reaping it..";
        thread_tcp_listener.join();
        tcplistener_running = true; /* BAD HACK, cant restart! TODO: */
      }
      theLock.unlock();
    }

    return 0;
    }
