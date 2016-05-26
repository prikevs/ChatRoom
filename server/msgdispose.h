#ifndef MSGDISPOSE_H
#define MSGDISPOSE_H

typedef struct ThreadArg {
    int status;
    int sockfd;
    int len;
    char *buf;
} ThreadArg;


int outOfRoom(char*);
int getIntoRoom(char*, char*);
int reg(char*, int sockfd);
int unRegByName(char*);
int unRegBySockfd(int sockfd);
void msgHandler(int, int, char*, int);
int registHandleFuncs();

#endif
