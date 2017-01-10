#include "network.h"
#include "msghandler.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

int initNetwork(const char *ip, const int port)
{
    int sockfd;
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) {
        perror("create socket:"); 
        return -1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton:");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect:"); 
        return -1;
    }
    return sockfd;
}

void startReadBuffer(int sockfd)
{
    char buf[BUFFSIZE];
    int n, cnt;
    memset(buf, 0, sizeof(buf));
    while((n = recv(sockfd, buf, MSGSIZE, MSG_WAITALL)) > 0) {
        handleBuffer(sockfd, buf, n);     
    }
}
