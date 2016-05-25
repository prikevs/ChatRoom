#include <stdio.h>
#include <string.h>

#include "common.h"
#include "session.h"

static LinkedList *sessionlist;

void initSessionList()
{
    if (sessionlist == NULL)
        sessionlist = initList(MAXNLEN);
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
    return session;
}

int addUserToSession(char *user, char *se)
{
    ListNode *session;
    LinkedList *clientlist;
    Session *senode;

    // lock
    session = findSession(se); 
    if (session == NULL) {
        senode = makeSession(se);
        addNodeToList(sessionlist, makeNode((void *)senode, (uint8_t *)se, sessionlist->key_len));
    }
    else
        senode = (Session *)(session->data);
    clientlist = senode->clientlist;
    if (findClient(clientlist, user) != NULL) {
        // unlock
        return -1;
    }
    addNodeToList(clientlist, makeNode(NULL, (uint8_t*)user, clientlist->key_len));
    senode->count++;
    // unlock
    return 0;
}

static void cleanClient(void *data)
{
    
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

int delUserFromSession(char *user, char *se)
{
    ListNode *session;
    LinkedList *clientlist;
    Session *senode;

    // lock
    session = findSession(se);
    if (session == NULL) {
        // unlock
        return -1;
    }
    senode = (Session *)(session->data);
    clientlist = senode->clientlist;
    if (deleteNode(clientlist, (uint8_t *)user, cleanClient) != 0) {
        // unlock
        return -1;
    }
    senode->count--; 
    if (senode->count == 0)
        delSession(se);             
    // unlock
    return 0;
}

LinkedList *getUsersFromSession(char *se)
{
    ListNode *session;
    LinkedList *clientlist, *ret;
    Session *senode;

    // lock
    session = findSession(se); 
    if (session == NULL) {
        return NULL;
        // unlock
    }
    senode = (Session *)(session->data);
    clientlist = senode->clientlist;
    ret = deepCopyList(clientlist, 0);
    // unlock
    return ret;
}

static void showClient(void *data)
{}

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
