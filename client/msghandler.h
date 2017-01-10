#ifndef MSGHANDLER_H
#define MSGHANDLER_H

int registHandleFuncs();
void handleBuffer(int, char*, int);
int reg(int, const char*);
int inRoom(int, const char*);
int sendMsgMsg(int, const char*);

#endif
