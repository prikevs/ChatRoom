#include "common.h"
#include "session.h"
#include "client.h"
#include "network.h"
#include "msgdispose.h"
#include "message.h"

#include <stdio.h>
#include <pthread.h>

int outOfRoom(char *name)
{
    char sename[MAXNLEN];
    memset(sename, 0, sizeof(sename));
    if (getClientSessionName(name, sename) != 0 || strlen(sename) == 0)
        return -1;
    if (modifySessionOfClient(name, NULL) != 0)
        return -1;
    if (delUserFromSession(name, sename) != 0)
        return -1;
    return 0;
}

int getIntoRoom(int sockfd, char *name, char *se)
{
    char sename[MAXNLEN];
    memset(sename, 0, sizeof(sename));
    getClientSessionName(name, sename);
    if (modifySessionOfClient(name, se) != 0)
        return -1;
    if (strlen(sename) != 0)
        delUserFromSession(name, sename);
    if (addUserToSession(sockfd, name, se) != 0)
        return -1;
    return 0;
}

int reg(char *name, int sockfd)
{
    User user;
    memset(&user, 0, sizeof(user));
    strncpy(user.name, name, MIN(strlen(name), MAXNLEN-1));
    if (addClientToList(sockfd, &user, NULL) != 0)
        return -1;
    return 0;
}

int unRegByName(char *name)
{
    outOfRoom(name);
    if (delClientFromListByName(name) != 0)
        return -1;
    return 0;
}

int outOfRoomBySockfd(int sockfd)
{
    char name[MAXNLEN]; 
    if (getClientNameBySockfd(sockfd, name) < 0 || strlen(name) == 0)
        return -1;
    return outOfRoom(name);
}

int getIntoRoomBySockfd(int sockfd, char *se)
{
    char name[MAXNLEN]; 
    memset(name, 0, sizeof(name));
    if (getClientNameBySockfd(sockfd, name) < 0 || strlen(name) == 0)
        return -1;
    return getIntoRoom(sockfd, name, se);
}

int unRegBySockfd(int sockfd)
{
    char name[MAXNLEN];
    if (getClientNameBySockfd(sockfd, name) < 0 || strlen(name) == 0)
        return -1;
    return unRegByName(name);
}

// Send Msg
int sendMsg(int sockfd, Msg *msg)
{
    int nwrite;
    nwrite = write(sockfd, (char *)msg, sizeof(Msg));
    if (nwrite == -1) {
        perror("write error:"); 
        return -1;
    }
    return 0;
}

int sendMsgRetSuccess(int sockfd, const char *hint)
{
    Msg msg;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_ret;
    genMSG_ret(msg.msgbody, &msg.bodylen, 1, hint);
    return sendMsg(sockfd, &msg);
}

int sendMsgRetFailed(int sockfd, const char *error)
{
    Msg msg;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_ret;
    genMSG_ret(msg.msgbody, &msg.bodylen, 0, error);
    return sendMsg(sockfd, &msg);
}

void sendMsgMsg(Msg *initmsg, char *from, LinkedList *list)
{
    Msg msg;
    ListNode *p;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_msg;
    msg.bodylen = initmsg->bodylen;
    strncpy((char *)msg.from, from, MAXNLEN-1);
    strncpy((char *)msg.msgbody, (char *)(initmsg->msgbody), initmsg->bodylen);
    p = list->head; 
    while(p != NULL) {
        sendMsg(((User *)(p->data))->sockfd, &msg);
        p = p->next;
    } 
}

// Handle Msg types

static void handleMSG_reg(int sockfd, Msg *msg)
{
    char name[MAXNLEN];
    int len;

    memset(name, 0, sizeof(name));
    if (parseMSG_reg(msg->msgbody, msg->bodylen, name, (uint32_t*)&len) != 0) {
        sendMsgRetFailed(sockfd, "Reg user name illegal.");
        return;
    }
    if (reg(name, sockfd) == -1) {
        sendMsgRetFailed(sockfd, "User name already exists.");
        return;
    }
    sendMsgRetSuccess(sockfd, NULL);
    dumpClientList();
}

static void handleMSG_in(int sockfd, Msg *msg)
{
    char session[MAXNLEN];
    int len;
    memset(session, 0, sizeof(session));
    if (parseMSG_in(msg->msgbody, msg->bodylen, session, (uint32_t*)&len) != 0) {
        sendMsgRetFailed(sockfd, "Session name illegal.");
        return;
    }
    if (getIntoRoomBySockfd(sockfd, session) != 0) {
        sendMsgRetFailed(sockfd, "Failed to get into room");
        return;
    }
    sendMsgRetSuccess(sockfd, NULL);
    dumpSessionList();
}

static void handleMSG_out(int sockfd, Msg *msg)
{
    if (outOfRoomBySockfd(sockfd) != 0) {
        sendMsgRetFailed(sockfd, "Server Error");
        return;
    }
    sendMsgRetSuccess(sockfd, NULL);
}

static void handleMSG_msg(int sockfd, Msg *msg)
{
    char sename[MAXNLEN];
    char name[MAXNLEN]; 
    LinkedList *list;

    if (getClientNameBySockfd(sockfd, name) < 0 || strlen(name) == 0) {
        sendMsgRetFailed(sockfd, "You have not registered");
        return;
    }
    memset(sename, 0, sizeof(sename));
    if (getClientSessionName(name, sename) != 0 || strlen(sename) == 0) {
        sendMsgRetFailed(sockfd, "You are not in any session."); 
        return;
    }
    list = getUsersFromSession(sename); 
    if (list == NULL) {
        sendMsgRetFailed(sockfd, "You are not in any session."); 
        return;
    }
    sendMsgMsg(msg, name, list);

    destroyClientListInSession(list); 
}

static void handleMSG_list(int sockfd, Msg *msg)
{

}

int registHandleFuncs()
{
    if (registHandleFunc(MSG_reg, handleMSG_reg, 0) != 0)
        return -1;
    if (registHandleFunc(MSG_in, handleMSG_in, 0) != 0)
        return -1;
    if (registHandleFunc(MSG_out, handleMSG_out, 0) != 0)
        return -1;
    if (registHandleFunc(MSG_list, handleMSG_list, 0) != 0)
        return -1;
    if (registHandleFunc(MSG_msg, handleMSG_msg, 0) != 0)
        return -1;
    return 0;
}

static void handleBuffer(int sockfd, char *buf, int len)
{
    handleMsg(sockfd, (uint8_t *)buf, len);
}

// Msg handler thread

static void *msgHandle_thread(void *data)
{
    ThreadArg *args;
    args = (ThreadArg*)data;
    do {
        if (args->status == STATUS_offline) {
            printf("%d offline:", args->sockfd);
            unRegBySockfd(args->sockfd);
            break;
        }
        if (args->status == STATUS_handle) {
            printf("%d handle:", args->sockfd);
            handleBuffer(args->sockfd, args->buf, args->len);
            break;
        }
    } while(0);
    printf("%s\n", args->buf);
    return ((void *)0);
}

static pthread_t main_tid;

void msgHandler(int status, int sockfd, char *buf, int len)
{
    int err;
    ThreadArg args;
    args.status = status;
    args.sockfd = sockfd;
    args.len = len;
    memcpy(args.buf, buf, BUFFSIZE*sizeof(char));
    msgHandle_thread((void *)(&args)); 
    // err = pthread_create(&main_tid, NULL, msgHandle_thread, (void *)(&args));
    // if (err != 0) {
    //    perror("handle message, create thread:"); 
    //     sendMsgRetFailed(sockfd, "Service unavilable."); 
    //}
}
