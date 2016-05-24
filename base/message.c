#include "message.h"
#include "linkedlist.h"
#include <stdint.h>
#include <stdio.h>

static LinkedList *list;

void cleanData(void *data) {}

int registHandleFunc(int msgtype, void(*handle)(uint8_t*, uint32_t), int override)
{
    uint8_t key[1];

    if (list == NULL)
        list = initList(1);
    key[0] = msgtype; 
    if (findNode(list, key) == NULL)
        addNodeToList(list, makeNode((void*)(handle), key, list->key_len));
    else {
        if (override == 1) {
            updateNode(list, key, (void*)(handle), cleanData);
            return 0;
        }
    }
    return 1;
}

void(*findProperHandler(int msgtype))(uint8_t *buf, uint32_t len)
{
    uint8_t key[1];
    key[0] = msgtype;
    ListNode *node = findNode(list, key);
    if (node == NULL)
        return NULL;
    return node->data;
}

int handleMsg(uint8_t *buf, uint32_t len) 
{
    Msg *msg;
    void(* handler)(uint8_t*, uint32_t);

    if (len <= MSGMINLEN)
        return -1;
    msg = (Msg*)buf;
    do {
        if (msg->msgtype == MSG_in)
            printf("MSG_in\n");
        else if (msg->msgtype == MSG_out)
            printf("MSG_out\n");
        else if (msg->msgtype == MSG_list)
            printf("MSG_list\n");
        else if (msg->msgtype == MSG_ret)
            printf("MSG_ret\n");
    } while(0);

    handler = findProperHandler(msg->msgtype);
    if (handler == NULL)
        return -1;
    handler(msg->msgbody, msg->bodylen);
    return 0;
}
