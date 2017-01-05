#include "msghandler.h"
#include "network.h"

#include "common.h"
#include <stdlib.h>


int sockfd;

int init(char *ip, int port) {
    if (registHandleFuncs() < 0)
        return -1;
    sockfd = initNetwork(ip, port);
    if (sockfd < 0) {
        return -1
    }
    return 0;
}

void start() {
    char name[MAXNLEN];
    printf("Input your name: ");
    scanf("%s", name);
    if (reg(socfd, name) < 0) {
        fprintf(stderr, "Failed to register, exit.");
        return;
    }
    loop();
}

void in_room(char *room) {
    if (inRoom(sockfd, room) < 0) {
        fprintf(stderr, "Failed to get into room.");
    }
}


int round() {
    
}

void loop() {
    
}


int main()
{
    const char *ip = "118.193.207.78";
    const int port = 9999;
    reg(sockfd, "hui");
    //sleep(1);
    inRoom(sockfd, "room1");
    startReadBuffer(sockfd);
    return 0;
}
