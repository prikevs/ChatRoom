#include "message.h"
#include "common.h"

#include <stdio.h>
#include <unistd.h>

static uint64_t MsgId;
static uint64_t RegId;

static void handleMSG_msg(int sockfd, Msg *msg)
{
    printf("From %s: %s\n", msg->from, msg->msgbody);
}

static void handleMSG_ret(int sockfd, Msg *msg) {
    char hint[MSGBODYSIZE] = {0};
    int ret = 0;
    ret = parseMSG_ret(msg->msgbody, msg->bodylen, hint);
    if (ret < 0) {
        fprintf(stderr, "failed to parse ret message\n");
    } else if (ret == 1) {
        printf(ANSI_COLOR_GREEN "[v] success" ANSI_COLOR_RESET);
    } else if (ret == 0) {
        printf(ANSI_COLOR_RED "[x] failure" ANSI_COLOR_RESET);
    }
    if (strlen(hint) == 0) {
        printf("\n");
    } else {
        printf(": %s\n", hint);
    }
    if (RegId == msg->msgid && ret != 1) {
        fprintf(stderr, "failed to register. exit");
        exit(1);
    }
}

static void handleMSG_list(int sockfd, Msg *msg) {
    LinkedList *list = NULL;
    const ListNode *node;
    int cnt = 0;

    list = parseMSG_list(msg->msgbody, msg->bodylen);
    node = list->head;
    printf("------------\n");
    printf("List:\n");
    while(node != NULL) {
        printf("%d: %s\n", cnt++, ((User *)(node->data))->name);
        node = node->next;
    }
    printf("------------\n");
}

int registHandleFuncs()
{
    // init message id
    MsgId = rand();
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
    msg->msgid = MsgId++;
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
    RegId = msg.msgid;
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
