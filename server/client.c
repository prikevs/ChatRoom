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

// 初始化客户端列表
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

// 根据sockfd寻找对应Client节点
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

// 根据传入的参数生成一个Client节点
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

// 将一个Client节点加入Client链表
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

// 根据名字从链表中删除某个节点
int delClientFromListByName(char *name)
{
    int ret;
    ENTRY();
    ret = deleteNode(clientlist, (uint8_t *)name, cleanClient);
    EXIT(ret);
}

// 根据sockfd从链表中删除某个节点
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

// 获取Client所属的Session名
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

// 根据sockfd获取对应client的名字
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

// 修改某个Client的session名
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
