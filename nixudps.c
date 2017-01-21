#include<sys/socket.h>
#include<sys/un.h>

int main() {
  int sockfd;
  int outfd = 1;
  int listening = 1;
  char msg[100];
  char SOCKET_PATHNAME[] = "/tmp/gidnixock.socket";
  struct sockaddr_un sockaddr;

  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

  if (sockfd < 0) {
    return -1;
  }

  unlink(SOCKET_PATHNAME);

  memset(&sockaddr, 0, sizeof(struct sockaddr_un));
  sockaddr.sun_family = AF_UNIX;
  strcpy(sockaddr.sun_path, SOCKET_PATHNAME);

  if (bind(sockfd, (const struct sockaddr*)&sockaddr, sizeof(struct sockaddr_un)) < 0) {
    return -1;
  }

  while(listening) {
    if (recv(sockfd, msg, sizeof(msg), 0) < 0)
      listening = 0;

    if (write(outfd, msg, sizeof(msg)) < 0)
      listening = 0;
  }

  unlink(SOCKET_PATHNAME);
  close(sockfd);
  return 0;
}
