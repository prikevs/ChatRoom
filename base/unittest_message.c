#include "message.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void handle_in(uint8_t *body, uint32_t len)
{
    int i;
    printf("in handle_in\n"); 
    for(i = 0; i < len; ++i) {
        printf("%c ", body[i]); 
    }
    printf("\n");
}

void handle_out(uint8_t *body, uint32_t len)
{
    printf("in handle_out\n"); 
}

int main()
{
    uint8_t *buf;
    char str[] = "hello";
    int len;
    Msg msg;
    msg.msgtype = MSG_in;
    msg.bodylen = 4;
    len = 4 + 4 + 4;
    strncpy(msg.msgbody, str, 4);
    buf = (uint8_t *)&msg;
    registHandleFunc(MSG_in, handle_in, 0);
    registHandleFunc(MSG_out, handle_out, 0);
    handleMsg(buf, len);
    return 0;
}
