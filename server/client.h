#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>

#include "common.h"
#include "network.h"
#include "session.h"

typedef struct Client {
    // struct sockaddr_in client_addr;    
    int sockfd;
    User user;
    char sessionname[MAXNLEN];
} Client;

int initClientList();
ListNode *findClientBySockfd(int);
Client *makeClient(int, User*, char*);
int addClientToList(int, User*, char*);
int delClientFromListByName(char*);
int delClientFromListBySockfd(int);
int modifySessionOfClient(char*, char*);
int getClientSessionName(char*, char*);
int getClientNameBySockfd(int, char*);
void dumpClientList();


#endif
