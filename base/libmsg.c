#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

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
    memset(room, 0, sizeof(char)*(*len)); 
    for(i = 0; i < msglen; ++i) {
        if (msgbody[i] == '\\') {
            if (i > (*len)-1)
                return -1;
            strncpy(room, (char *)msgbody, i); 
            *len = i;
            return 0;
        }
    }
    return -1;
}

int genMSG_in(uint8_t *buffer, uint32_t *len, char *room)
{
    int rlen;

    rlen = strlen(room);
    if (rlen > MSGBODYSIZE-1)
        return -1;
    sprintf((char *)buffer, "%s\\", room);
    (*len) = rlen + 1;
    return 0;
}

