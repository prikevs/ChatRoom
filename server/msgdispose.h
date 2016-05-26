#ifndef MSGDISPOSE_H
#define MSGDISPOSE_H

int outOfRoom(char*);
int getIntoRoom(char*, char*);
int reg(char*, int sockfd);
int unRegByName(char*);
int unRegBySockfd(int sockfd);

#endif
