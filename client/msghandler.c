#include "message.h"
#include "common.h"

#include <stdio.h>
#include <unistd.h>

static void handleMSG_msg(int sockfd, Msg *msg)
{
    printf("From %s: %s\n", msg->from, msg->msgbody);
}

static void handleMSG_ret(int sockfd, Msg *msg) {
    printf("Ret: %s\n", msg->msgbody);
}

static void handleMSG_list(int sockfd, Msg *msg) {
    printf("List: %s\n", msg->msgbody);
}

int registHandleFuncs()
{
    if (registHandleFunc(MSG_msg, handleMSG_msg, 0) != 0)
        return -1;
    if (registHandleFunc(MSG_ret, handleMSG_ret, 0) != 0)
        return -1;
    if (registHandleFunc(MSG_list, handleMSG_list, 0) != 0)
        return -1;
    return 0;
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

int listUsers(int sockfd) {
    Msg msg;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_list;
    strcpy((char *)msg.msgbody, "users");
    if (sendMsg(sockfd, &msg) < 0) {
        fprintf(stderr, "Failed to send message.\n"); 
        return -1; 
    }
    return 0;
}

int sendMsgMsg(int sockfd, const char *smsg)
{
    Msg msg;
    memset(&msg, 0, sizeof(Msg));
    msg.msgtype = MSG_msg;
    strncpy((char *)msg.msgbody, (char *)smsg, strlen(smsg));
    msg.bodylen = strlen(smsg) + 1;
    if (sendMsg(sockfd, &msg) < 0) {
        fprintf(stderr, "Failed to send message.\n"); 
        return -1; 
    }
    return 0;
}
