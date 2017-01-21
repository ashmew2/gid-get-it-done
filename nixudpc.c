#include<sys/socket.h>
#include<sys/un.h>

int main() {
  int clientfd;
  char msg[100] = "AF_UNIX lives again!";
  char SOCKET_PATHNAME[] = "/tmp/gidnixock.socket";
  struct sockaddr_un servaddr;

  clientfd = socket(AF_UNIX, SOCK_DGRAM, 0);

  if (clientfd < 0) {
    return -1;
  }

  memset(&servaddr, 0, sizeof(struct sockaddr_un));
  servaddr.sun_family = AF_UNIX;
  strcpy(servaddr.sun_path, SOCKET_PATHNAME);

  if (connect(clientfd, (const struct sockaddr*)&servaddr, sizeof(struct sockaddr_un)) < 0) {
    return -1;
  }

  write(clientfd, msg, sizeof(msg));
  close(clientfd);
  
  return 0;
}
