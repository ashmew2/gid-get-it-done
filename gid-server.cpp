#include<iostream>
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

void timeout(int time_in_sex, int thread_id) {
  cout<<"dreamer " <<thread_id<<": Created for : "<<time_in_sex<<" seconds"<<endl;
  this_thread::sleep_for(chrono::seconds(time_in_sex));
  cout<<"dreamer " <<thread_id<<": woke up"<<endl;

  // Do what we want to do here, like play an alarm or trigger the phone
  // Or call the phone. Each thread should have a final purpose that gid.sh set for it
  // using USR1

  theLock.lock();
  t.push_back(thread_id);
  theLock.unlock();
}

void handle_sigusr1(int signum) {
  sig_caught=1;
}

int main() {

  signal(SIGUSR1, handle_sigusr1);
  int POLL_INTERVAL=1;
  int tid =0;
  int fd[2];
  int piped=pipe(fd);
  int number_of_lines_read=0;
  int last_offset=0;

  /* TODO - this wont scale for large and will overflow eventually */
  /* TODO - create gid config so that the filenames in sh and c stay the same for gidpiping */
  
  std::string line;

  while (1) {
    this_thread::sleep_for(chrono::seconds(POLL_INTERVAL));

    if(sig_caught==1) {
      /* The data about the new thread should already be in the fd */
      /* very bad way to get this done */
      std::ifstream input( "/tmp/timer" );
      if (input == NULL) {
	printf("fuckedup with /tmp/timer");
	exit(-2);
      }

      for(int i = 0; i < number_of_lines_read && getline(input, line); i++);
      getline( input, line );
      cout<<line<<endl;
      
      input.close();
      int time_for_task=atoi(line.c_str());

      /* 0 timer tasks shouldn't be passed to the gid server. we will simply ignore it*/
      if(time_for_task > 0) {
	++tid;
	m[tid]=std::thread(timeout, time_for_task, tid);
	sig_caught=0;
	++number_of_lines_read;
      }

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
