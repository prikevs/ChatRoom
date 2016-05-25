#include "message.h"
#include "libmsg.h"
#include "linkedlist.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BUFFSIZE 4096

void showData(void *data)
{
    printf("%s", ((User *)data)->name);
}

void testMSG_list()
{
    uint8_t *msg = "xiaoming\\jie\\kevince\\卜杰\\";
    LinkedList *list;
    list = parseMSG_list(msg, strlen((char *)msg)); 
    dumpList(list, showData);
    uint8_t buffer[BUFFSIZE];
    uint32_t len;
    memset(buffer, 0, sizeof(buffer));
    genMSG_list(buffer, &len, list);
    printf("%s\n", buffer);
    int i;
    printf("len=%d\n", len);
    for(i = 0; i < len; ++i)
        printf("%c", buffer[i]);
    printf("\n");
}

void testMSG_in()
{
    uint8_t *msgbody = "room1";
    int len = strlen((char *)msgbody);
    char buffer[BUFFSIZE];
    int size = BUFFSIZE;
    //parseMSG_in(msgbody, len, buffer, &size);
    //printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); 
    genMSG_in(buffer, &size, msgbody);
    printf("%s\n", buffer);
}

int main()
{
    testMSG_in(); 
    return 0;
}
