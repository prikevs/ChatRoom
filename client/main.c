#include "msghandler.h"
#include "network.h"

#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int sockfd;

void in_room(const char *room) {
    if (inRoom(sockfd, room) < 0) {
        fprintf(stderr, "Failed to get into room.\n");
    }
}

void list_users() {
    if (listUsers(sockfd) < 0) {
        fprintf(stderr, "Failed to list users.\n");
    }
}

void chat(const char *msg) {
    if (sendMsgMsg(sockfd, msg) < 0) {
        fprintf(stderr, "Failed to send room.\n");
    }
}


void cmdline() {
    int ch;
    char room[MAXNLEN], msg[MAXNLEN];
    while (1) {
        printf("1.in session\n2.chat\n");
        scanf("%d", &ch);
        switch(ch) {
        case 1:
            printf("room name:");
            scanf("%s", room);
            in_room(room);
            break;
        case 2:
            printf("chat:");
            scanf("%s", msg);
            chat(msg);
            break;
        case 3:
            list_users();
            break;
        }
    }
}


int init(const char *ip, int port) {
    srand(time(NULL));
    if (registHandleFuncs() < 0)
        return -1;
    sockfd = initNetwork(ip, port);
    if (sockfd < 0) {
        return -1;
    }
    return 0;
}

void start() {
    char name[MAXNLEN];
    printf("Input your name: ");
    scanf("%s", name);
    if (reg(sockfd, name) < 0) {
        fprintf(stderr, "Failed to register, exit.\n");
        return;
    }
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "failed to fork, exit.\n");
        return;
    }
    if (pid == 0) {
        cmdline();
    } else {
        startReadBuffer(sockfd);
    }
}


int main()
{
    const char *ip = "118.193.207.78";
    const int port = 9999;
    if (init(ip, port) < 0) {
        fprintf(stderr, "failed to init network. exit\n"); 
        return -1;
    }
    start();
    return 0;
}
