#ifndef MESSAGE_H 
#define MESSAGE_H

#include <stdint.h>
#define MAXNLEN 32
#define MSGBODYSIZE (1<<10)
#define MSGMINLEN (4+4)
#define MSGSIZE (MSGBODYSIZE+4+8+4+MAXNLEN)
#define BUFFSIZE (MSGSIZE+4)

// MSG_reg:  register on the server
// MSG_in:   MSG_in |room name, end with '/'
// MSG_out:  MSG_out, exit from current room, success return ok/, failed return failed/message
// MSG_list: MSG_list, ask server to return all users in the same room, return ok/user1/user2/...
// MSG_ret:  char 0 to 3, 'in' or 'out' or 'list', char 4 '/', results

enum MsgType{
    // MSG_temp,
    MSG_reg,
    MSG_in,
    MSG_out,
    MSG_list,
    MSG_msg,
    MSG_ret
};

typedef struct Msg {
    uint64_t msgid;
    int msgtype;
    uint32_t bodylen;
    uint8_t from[MAXNLEN];
    uint8_t msgbody[MSGBODYSIZE];
} Msg;

int registHandleFunc(int, void(*)(int, Msg*), int);
int handleMsg(int, uint8_t*, int);

#endif
