
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <stdlib.h>

int run = 1;

void interapt(int signo) {
    if (signo == SIGINT) {
        run = 0;
    }
}

int main(int argc, char** argv) {
    char* IP = "192.168.1.255";
    uint16_t port = 1354;
    signal(SIGINT, interapt);
    int sd_listener;
    if ((sd_listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error: Con't create socket");
        return 1;
    }
    int val = 1;
    setsockopt(sd_listener, SOL_SOCKET, SO_BROADCAST, &val, sizeof(int));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    struct in_addr ip_addr;
    memset(&ip_addr, 0, sizeof(struct in_addr));
    if(inet_aton(IP, &ip_addr) == 0) {
        perror("Error: Can't fill ip address");
        close(sd_listener);
        return 1;
    };
    addr.sin_addr = ip_addr;
    addr.sin_port = htons(port);
    char* buff = "hello world\n";
    socklen_t socklen = sizeof(struct sockaddr);
    while (run) {
        sendto(sd_listener, buff, 13, 0, (const struct sockaddr *) &addr, socklen);
        printf("send: ");
        printf("%s", buff);
        sleep(1);
    }
    if (sd_listener > 0)
        close(sd_listener);
    if (buff)
        free(buff);
    return 0;
}