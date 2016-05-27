#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "session.h"

#define ENTRY() pthread_mutex_lock(&lock)
#define EXIT(ret) pthread_mutex_unlock(&lock); return ret

static LinkedList *sessionlist;
static pthread_mutex_t lock;

int initSessionList()
{
    if (sessionlist == NULL) {
        sessionlist = initList(MAXNLEN);
        if (sessionlist == NULL)
            return -2;
    }
    if (pthread_mutex_init(&lock, NULL) != 0)
        return -1;
    return 0;
}

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

int delSession(char *se)
{
    return deleteNode(sessionlist, (uint8_t *)se, cleanSession);
}

void destroyClientListInSession(LinkedList *list)
{
    destroyList(&list, cleanClient);
}

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
