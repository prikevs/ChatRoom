#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "session.h"
#include "client.h"

#define ENTRY() pthread_mutex_lock(&lock)
#define EXIT(ret) pthread_mutex_unlock(&lock); return ret

static LinkedList *clientlist;
static pthread_mutex_t lock;

int initClientList()
{
    if (clientlist == NULL) {
        clientlist = initList(MAXNLEN); 
        if (clientlist == NULL)
            return -1;
    }
    if (pthread_mutex_init(&lock, NULL) != 0)
        return -1;
    return 0;
}

ListNode *findClientBySockfd(int sockfd)
{
    ListNode *p;

    p = clientlist->head;
    while(p != NULL) {
        if (((Client *)(p->data))->sockfd == sockfd)
            return p;
        p = p->next;
    }
    return NULL;
}

Client *makeClient(int sockfd, User *user, char *session)
{
    Client *client;

    client = (Client *)calloc(1, sizeof(Client));
    //memcpy(&(client->client_addr), addr, sizeof(client->client_addr));
    client->user = *user;
    client->sockfd = sockfd;
    if (session != NULL)
        memcpy(client->sessionname, session, MAXNLEN);
    return client;
}

int addClientToList(int sockfd, User *user, char *session)
{
    int ret;

    ENTRY();
    if (findNode(clientlist, (uint8_t *)user->name) != NULL) {
        EXIT(-1); 
    }
    ret = addNodeToList(clientlist, makeNode((void *)makeClient(sockfd, user, session), (uint8_t *)user->name, clientlist->key_len));
    EXIT(ret);
}

void cleanClient(void *data)
{
    free(data);
}

int delClientFromListByName(char *name)
{
    int ret;
    ENTRY();
    ret = deleteNode(clientlist, (uint8_t *)name, cleanClient);
    EXIT(ret);
}

int delClientFromListBySockfd(int sockfd)
{
    ListNode *node;
    int ret;
    ENTRY();
    node = findClientBySockfd(sockfd); 
    if (node == NULL) {
        EXIT(-1);
    }
    ret = deleteNodeByNode(clientlist, node, cleanClient);
    EXIT(ret);
}

int getClientSessionName(char *name, char *buf)
{
    ListNode *clinode;
    ENTRY();
    clinode = findNode(clientlist, (uint8_t *)name); 
    if (clinode == NULL) {
        EXIT(-1);
    }
    if (strlen(((Client *)(clinode->data))->sessionname) == 0) {
        EXIT(-2); 
    }
    memcpy(buf, ((Client *)(clinode->data))->sessionname, MAXNLEN);
    EXIT(0);
}

int getClientNameBySockfd(int sockfd, char *buf)
{
    ListNode *clinode;
    ENTRY();
    clinode = findClientBySockfd(sockfd);
    if (clinode == NULL) {
        EXIT(-1);
    }
    strncpy(buf, ((Client *)(clinode->data))->user.name, MAXNLEN-1);
    EXIT(0);
}

int modifySessionOfClient(char *name, char *session)
{
    ListNode *clinode;
    ENTRY();
    clinode = findNode(clientlist, (uint8_t *)name); 
    if (clinode == NULL) {
        EXIT(-1);
    }
    if (session == NULL)
        memset(((Client *)(clinode->data))->sessionname, 0, MAXNLEN);
    else
        memcpy(((Client *)(clinode->data))->sessionname, session, MAXNLEN);
    EXIT(0);
}

static void showData(void *data)
{
    Client *client;
    client = (Client *)data;
    printf("client->name: %s, client->sockfd: %d, client->session: %s", client->user.name, client->sockfd, client->sessionname);
}

void dumpClientList()
{
    dumpList(clientlist, showData); 
}
