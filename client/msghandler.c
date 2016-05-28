#include "message.h"
#include "common.h"

#include <stdio.h>

static void handleMSG_msg(int sockfd, Msg *msg)
{
    printf("From %s: %s\n", msg->from, msg->msgbody);
}

int registHandleFuncs()
{
    if (registHandleFunc(MSG_msg, handleMSG_msg, 0) != 0)
        return -1;
}

void handleBuffer(int sockfd, char *buf, int len)
{
    handleMsg(sockfd, (uint8_t *)buf, len);
}

static int sendMsg(int sockfd, Msg *msg)
{
    int nwrite;
    nwrite = write(sockfd, (char *)msg, sizeof(Msg));
    if (nwrite == -1) {
        perror("write error:");
        return -1;
    }
    return 0;
}

int reg(int sockfd, const char *name)
{
    Msg msg;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_reg;
    if (genMSG_reg(msg.msgbody, &msg.bodylen, name) < 0) {
        fprintf(stderr, "Failed to gen MSG_reg.\n"); 
        return -1;
    }
    if (sendMsg(sockfd, &msg) < 0) {
        fprintf(stderr, "Failed to send message.\n"); 
        return -1; 
    }
    return 0;
}

int inRoom(int sockfd, const char *room)
{
    Msg msg;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_in;
    if (genMSG_in(msg.msgbody, &msg.bodylen, room) < 0) {
        fprintf(stderr, "Failed to gen MSG_in.\n"); 
        return -1;
    }
    if (sendMsg(sockfd, &msg) < 0) {
        fprintf(stderr, "Failed to send message.\n"); 
        return -1; 
    }
    return 0;
}
