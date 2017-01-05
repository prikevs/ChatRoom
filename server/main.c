#include "common.h"
#include "session.h"
#include "client.h"
#include "msgdispose.h"
#include "network.h"

#include <stdio.h>

int init()
{
    if (initClientList() != 0) {
        fprintf(stderr, "Failed to init client list\n");
        return -1;
    }
    if (initSessionList() != 0) {
        fprintf(stderr, "Failed to init session list\n");
        return -1;
    }
    if (registHandleFuncs() != 0) {
        fprintf(stderr, "Failed to regist msg handlers\n");
        return -1;
    }
    return 0;
}

int main()
{
    char *ip = "0.0.0.0";
    int port = 9999;

    if (init() != 0) {
        fprintf(stderr, "Failed to init.\n");
    }
    startNetwork(ip, port, msgHandler); 
    return 0;
}
