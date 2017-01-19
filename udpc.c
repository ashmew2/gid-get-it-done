#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main() {
    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 8888;
    inet_aton("127.0.0.1", &server_addr.sin_addr);
    char buf[] = "echoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoechoecho";
    sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    return 0;
}
