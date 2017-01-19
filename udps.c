#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<string.h>
#include<unistd.h>

int main(){
    int sockfd;
    struct sockaddr_in sock_addr;
    char buf[100];
    int listening=1;
    int outfd=1;
    int nbftl = 0;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = 8888;
    inet_aton("127.0.0.1", &sock_addr.sin_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
    
    while(listening | nbftl) {
            if (recv(sockfd, buf, sizeof(buf), 0) <= 0)
               listening=0;
               
            if (write(outfd, buf, sizeof(buf)) <= 0)
               listening=0;
    }

    return 0;
}
