#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "session.h"

// #define ENTRY() pthread_mutex_lock(&lock)
// #define EXIT(ret) pthread_mutex_unlock(&lock); return ret
#define ENTRY()
#define EXIT(ret) return ret

static LinkedList *sessionlist;
// static pthread_mutex_t lock;

// 初始化Sesion列表
int initSessionList()
{
    if (sessionlist == NULL) {
        sessionlist = initList(MAXNLEN);
        if (sessionlist == NULL)
            return -2;
    }
    return 0;
}

LinkedList *getSessionList() {
    return sessionlist;
}

// 根据session名查找某个session节点
ListNode *findSession(char *se)
{
    return findNode(sessionlist, (uint8_t *)se);
}

ListNode *findClient(LinkedList *clientlist, char *user)
{
    return findNode(clientlist, (uint8_t *)user); 
}

Session *makeSession(char *se)
{
    Session *session;
    session = (Session *)calloc(1, sizeof(Session));
    session->clientlist = initList(MAXNLEN);
    session->count = 0;
    strncpy(session->name, se, MIN(MAXNLEN-1,strlen(se)));
    return session;
}

User *makeUser(int sockfd, char *name)
{
    User *user;
    
    user = (User *)calloc(1, sizeof(User)); 
    strcpy(user->name, name);
    user->sockfd = sockfd;
    return user;
}

// 将某个user加入某个session，如果不存在对应Session就自动创建一个
int addUserToSession(int sockfd, char *name, char *se)
{
    ListNode *session;
    LinkedList *clientlist;
    Session *senode;
    User *suser;

    // lock
    ENTRY();
    session = findSession(se); 
    if (session == NULL) {
        senode = makeSession(se);
        addNodeToList(sessionlist, makeNode((void *)senode, (uint8_t *)se, sessionlist->key_len));
    }
    else {
        senode = (Session *)(session->data);
    }
    clientlist = senode->clientlist;
    if (findClient(clientlist, name) != NULL) {
        // unlock
        EXIT(-1);
    }

    suser = makeUser(sockfd, name);
    addNodeToList(clientlist, makeNode((void *)suser, (uint8_t*)name, clientlist->key_len));
    senode->count++;
    // unlock
    EXIT(0);
}

static void cleanClient(void *data)
{
    free(data); 
}

static void cleanSession(void *data)
{
    Session *session = (Session *)data;
    destroyList(&(session->clientlist), cleanClient);
}

// 根据session名删除某个session
int delSession(char *se)
{
    return deleteNode(sessionlist, (uint8_t *)se, cleanSession);
}

void destroyClientListInSession(LinkedList *list)
{
    destroyList(&list, cleanClient);
}

// 将某个用户从某个session中删除
int delUserFromSession(char *user, char *se)
{
    ListNode *session;
    LinkedList *clientlist;
    Session *senode;

    // lock
    ENTRY();
    session = findSession(se);
    if (session == NULL) {
        // unlock
        EXIT(-1);
    }
    senode = (Session *)(session->data);
    clientlist = senode->clientlist;
    if (deleteNode(clientlist, (uint8_t *)user, cleanClient) != 0) {
        // unlock
        EXIT(-1);
    }
    senode->count--; 
    if (senode->count == 0)
        delSession(se);             
    // unlock
    EXIT(0);
}

// 从session中根据session名获取对应session中所有的用户，并用链表返回
LinkedList *getUsersFromSession(char *se)
{
    ListNode *session;
    LinkedList *clientlist, *ret;
    Session *senode;

    // lock
    ENTRY();
    session = findSession(se); 
    if (session == NULL) {
        // unlock
        EXIT(NULL);
    }
    senode = (Session *)(session->data);
    clientlist = senode->clientlist;
    ret = deepCopyList(clientlist, sizeof(User));
    // unlock
    EXIT(ret);
}

static void showClient(void *data)
{
    printf("%s", ((User *)data)->name);
}

static void showSession(void *data)
{
    Session *senode = (Session *)data; 
    LinkedList *clientlist = senode->clientlist; 
    dumpList(clientlist, showClient);
}

void dumpSessionList()
{
    dumpList(sessionlist, showSession);
}
