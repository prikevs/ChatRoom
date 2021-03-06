#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

// 解析MSG_list信息，传入解码前的字符流，输出解码后的链表结构
LinkedList *parseMSG_list(const uint8_t *msgbody, uint32_t msglen)
{
    int i, f;
    User *user;
    LinkedList *userlist;

    userlist = initList(MAXNLEN);

    user = NULL;
    for(i = f = 0; i < msglen; ++i) {
        if (msgbody[i] == '\\') {
            if (i - f > 0) {
                user = (User*)calloc(1, sizeof(User)); 
                strncpy(user->name, (char*)msgbody+f, MIN(MAXNLEN-1, i-f));
                addNodeToList(userlist, makeNode((void *)user, (uint8_t *)user->name, userlist->key_len));
                f = i + 1;
            }                
        }
    }
    return userlist;
}

// 根据给定的链表生成对应的字符流用于网络传输
int genMSG_list(uint8_t *buffer, uint32_t *len, LinkedList *list)
{
    ListNode *node;
    int cnt;

    node = list->head;
    cnt = 0;
    while(node != NULL && cnt + strlen(((User *)(node->data))->name) + 1 < MSGBODYSIZE) {
        //printf("name: %s\n", ((User *)(node->data))->name);
        sprintf((char *)buffer+cnt, "%s\\", ((User *)(node->data))->name);
        //printf("%s\n", buffer);
        cnt += strlen(((User *)(node->data))->name) + 1;
        node = node->next; 
    }
    (*len) = cnt;
    return 0;
}

int parseMSG_in(const uint8_t *msgbody, uint32_t msglen, char *room, uint32_t *len)
{
    int i;
    memset(room, 0, sizeof(char)*MAXNLEN); 
    for(i = 0; i < msglen; ++i) {
        if (msgbody[i] == '\\') {
            if (i > (MAXNLEN)-1)
                return -1;
            strncpy(room, (char *)msgbody, i); 
            // printf("room: %s\n", room);
            *len = i;
            return 0;
        }
    }
    return -1;
}

int genMSG_in(uint8_t *buffer, uint32_t *len, const char *room)
{
    int rlen;

    rlen = strlen(room);
    if (rlen > MSGBODYSIZE-1)
        return -1;
    sprintf((char *)buffer, "%s\\", room);
    (*len) = rlen + 1;
    return 0;
}

int parseMSG_reg(const uint8_t *msgbody, uint32_t msglen, char *name, uint32_t *len)
{
    int i;
    memset(name, 0, sizeof(char)*MAXNLEN); 
    for(i = 0; i < msglen; ++i) {
        if (msgbody[i] == '\\') {
            if (i > (MAXNLEN-1))
                return -1;
            strncpy(name, (char *)msgbody, i); 
            *len = i;
            return 0;
        }
    }
    return -1;
}


int genMSG_reg(uint8_t *buffer, uint32_t *len, const char *name)
{
    int rlen;

    rlen = strlen(name);
    if (rlen > MSGBODYSIZE-1)
        return -1;
    sprintf((char *)buffer, "%s\\", name);
    (*len) = rlen + 1;
    return 0;
}

int genMSG_ret(uint8_t *buffer, uint32_t *len, int success, const char *hint)
{
    // printf("gen ret\n");
    if (success == 1)
        sprintf((char *)buffer, "success\\"); 
    else
        sprintf((char *)buffer, "failed\\");
    if (hint != NULL)
        sprintf((char *)(buffer+strlen((char *)buffer)), "%s\\", hint);
    // printf("ret: %s\n", (char *)buffer);
    *len = strlen((char *)buffer);
    return 0;
}

int parseMSG_ret(uint8_t *msgbody, uint32_t msglen, char *hint)
{
    int i;
    char success[] = "success";
    char failed[] = "failed";
    int ret = 0;
    for(i = 0; i < msglen; ++i) {
        if (msgbody[i] == '\\') {
            if (strncmp(success, (char *)msgbody, i) == 0)
                ret = 1;
            else if (strncmp(failed, (char *)msgbody, i) == 0)
                ret = 0;
            else {
                return -1;
            }
            break;
        }
    }
    msgbody[msglen-1] = 0;
    // printf("msgbody:%s\n", msgbody);
    strncpy(hint, (char *)(msgbody+i+1), strlen((char *)(msgbody+i+1)));
    // printf("hint:%s\n", hint);
    if (hint[strlen(hint)-1] == '\\') {
        hint[strlen(hint)-1] = 0;
    }
    return ret;
}
