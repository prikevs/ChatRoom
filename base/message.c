#include "message.h"
#include "linkedlist.h"
#include <stdint.h>
#include <stdio.h>

static LinkedList *list;

void cleanData(void *data) {}

int registHandleFunc(int msgtype, void(*handle)(int, Msg*), int override)
{
    uint8_t key[1];

    if (list == NULL)
        list = initList(1);
    key[0] = msgtype; 
    if (findNode(list, key) == NULL)
        addNodeToList(list, makeNode((void*)(handle), key, list->key_len));
    else {
        if (override == 1)
            updateNode(list, key, (void*)(handle), cleanData);
        else
            return 1;
    }
    return 0;
}

void(*findProperHandler(int msgtype))(int, Msg*)
{
    uint8_t key[1];
    key[0] = msgtype;
    ListNode *node = findNode(list, key);
    if (node == NULL)
        return NULL;
    return node->data;
}

int handleMsg(int sockfd, uint8_t *buf, int len)
{
    Msg *msg;
    void(* handler)(int, Msg*);

    if (len <= MSGMINLEN)
        return -1;
    msg = (Msg*)buf;
    do {
        if (msg->msgtype == MSG_reg)
            printf("MSG_reg\n");
        else if (msg->msgtype == MSG_in)
            printf("MSG_in\n");
        else if (msg->msgtype == MSG_out)
            printf("MSG_out\n");
        else if (msg->msgtype == MSG_list)
            printf("MSG_list\n");
        else if (msg->msgtype == MSG_ret)
            printf("MSG_ret\n");
        else if (msg->msgtype == MSG_msg)
            printf("MSG_msg\n");
        else {
            printf("others\n"); 
        }
    } while(0);

    handler = findProperHandler(msg->msgtype);
    if (handler == NULL)
        return -1;
    handler(sockfd, msg);
    return 0;
}
