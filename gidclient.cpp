#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

using namespace std;

int main() {

  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if(clientfd < 0) {
    cout<<"Could not get a client fd from socket()"<<endl;
    exit(1);
  }

  struct sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = 42000;
  inet_aton("127.0.0.1", &serveraddr.sin_addr);
  //serveraddr.sin_addr.s_addr = (127 << 24) + (0 << 16) + (0 << 8) + (1 << 0);

  if(connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) < 0) {
    cout<<"Connect to server failed."<<endl;
    close(clientfd);
    return 2;
  }

  char command[] = "10 random_command_here;";
  cout<<" Sent: " << send(clientfd, (void *)command, sizeof(command), 0) <<" bytes out of "<< sizeof(command) << endl;
  close(clientfd);
  return 0;
}
