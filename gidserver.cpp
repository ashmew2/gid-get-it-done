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
using namespace std;

static volatile sig_atomic_t sig_caught = 0;
static mutex theLock;
vector<int> t;
map<int, std::thread> m;
std::string GID_PIPE_FILE="/tmp/timer";

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

  sig_caught=1;
}

int main() {

  signal(SIGHUP, handle_sighup);
  signal(SIGUSR1, handle_sigusr1);

  int POLL_INTERVAL=1;
  int tid =0;
  int number_of_lines_read=0;
  int last_offset=0;

  /* TODO - this wont scale for large and will overflow eventually */
  /* TODO - create gid config so that the filenames in sh and c stay the same for gidpiping */
  /* TODO - nixock */

  std::string timeline;
  std::string cmdline;

  while (1) {
    this_thread::sleep_for(chrono::seconds(POLL_INTERVAL));

    /* TODO - add a counter for ignoring client file if client misbehaves */
    if(sig_caught==1) {

      sig_caught=0;

      /* very bad way to get this done */
      std::ifstream input( GID_PIPE_FILE );
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
  }

  return 0;
}
