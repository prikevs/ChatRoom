#include <stdint.h>
#include <string.h>

#include "common.h"
#include "message.h"
#include "linkedlist.h"

LinkedList *parseMSG_list(uint8_t *msgbody, uint32_t msglen)
{
    int i, f;
    User *user;
    LinkedList *userlist;

    userlist = initList(MAXNLEN);

    msglist = NULL;
    for(i = f = 0; i < msglen; ++i) {
        if (msgbody[i] == '/') {
            if (i - f > 0) {
                msglist = (MSG_List *)calloc(1, sizeof(MSG_List)); 
                strncpy(user->name, msgbody+f, MIN(MAXNLEN-1, i-f));
                addNodeToList(list, makeNode((void *)user, user->name, list->key_len));
                f = i + 1;
            }                
        }
    }
    return userlist;
}
