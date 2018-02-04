//
// Created by evgenii on 22.01.18.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char* IP = "192.168.1.255";
    uint16_t port = 1354;
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error: Con't create socket");
        return 1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    struct in_addr ip_addr;
    memset(&ip_addr, 0, sizeof(struct in_addr));
    if(inet_aton(IP, &ip_addr) == 0) {
        perror("Error: Can't fill ip address");
        close(sock);
        return 1;
    };
    addr.sin_addr = ip_addr;
    addr.sin_port = htons(port);
    char* buff = malloc(1024 * sizeof(char));
    ssize_t bytes_read;
    socklen_t socklen = sizeof(struct sockaddr);
    bind(sock, (const struct sockaddr *) &addr, socklen);
    while (1) {
        if ((bytes_read = recv(sock,buff, 13, 0)) <= 0) {
            close(sock);
        }
        printf("Recv: ");
        printf("%s", buff);
    }
}