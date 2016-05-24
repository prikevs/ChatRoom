#ifndef COMMON_H
#define COMMON

#include <stdint.h>
#define MSGBODYSIZE (2<<13)
#define MSGMINLEN (4+4)
#define MSGSIZE (MSGBODYSIZE+4+4)

// MSG_in:   MSG_in |room name, end with '/'
// MSG_out:  MSG_out, exit from current room, success return ok/, failed return failed/message
// MSG_list: MSG_list, ask server to return all users in the same room, return ok/user1/user2/...
// MSG_ret:  char 0 to 3, 'in' or 'out' or 'list', char 4 '/', results

enum MsgType{
    MSG_in,
    MSG_out,
    MSG_list,
    MSG_ret
};

typedef struct Msg {
    int msgtype;
    uint32_t bodylen;
    uint8_t msgbody[MSGBODYSIZE];
} Msg;

int registHandleFunc(int, void(*)(uint8_t*, uint32_t), int);
int handleMsg(uint8_t*, uint32_t);

#endif
