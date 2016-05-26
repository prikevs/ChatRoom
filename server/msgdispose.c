#include "common.h"
#include "session.h"
#include "client.h"
#include "network.h"
#include "msgdispose.h"

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

int getIntoRoom(char *name, char *se)
{
    if (addUserToSession(name, se) != 0)
        return -1;
    if (modifySessionOfClient(name, se) != 0)
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

int unRegBySockfd(int sockfd)
{
    char name[MAXNLEN];
    if (getClientNameBySockfd(sockfd, name) < 0 || strlen(name) == 0)
        return -1;
    return unRegByName(name);
}

// Handle Msg types

static void handleMSG_reg(int sockfd, uint8_t *body, uint32_t msglen)
{
    char name[MAXNLEN];
    int len;

    memset(name, 0, sizeof(name, 0, sizeof(name)));
    if (parseMSG_reg(body, msglen, name, &len) == 0) {
        reg(name, sockfd);
        // Send Msg TBD
    }
}

static void handleMsg_in(int sockfd, uint8_t *body, uint32_t msglen)
{

}

static void handleMsg_out(int sockfd, uint8_t *body, uint32_t msglen)
{

}

static void handleMsg_list(int sockfd, uint8_t *body, uint32_t msglen)
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
    return 0;
}

static void handleBuffer(char *buf)
{
    handleMsg(buf);
}

// Msg handler thread

static void *msgHandler_thread(void *data)
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
            handleBuffer(args->buf);
            break;
        }
    } while(0);
    printf("%s\n", args->buf);
    free(args->buf);
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
    args.buf = buf;
    err = pthread_create(&main_tid, NULL, msgHandle_thread, (void *)(&args));
    if (err != 0) {
        perror("handle message, create thread:"); 
        // Message return TBD
    }
}
