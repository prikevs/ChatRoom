#include "common.h"
#include "session.h"
#include "client.h"
#include "msgdispose.h"
#include "network.h"

#include <stdio.h>

int init()
{
    if (initClientList() != 0) {
        fprintf(stderr, "Failed to init client list");
        return -1;
    }
    if (initSessionList() != 0) {
        fprintf(stderr, "Failed to init session list");
        return -1;
    }
    if (registHandleFuncs() != 0) {
        fprintf(stderr, "Failed to regist msg handlers");
        return -1;
    }
}
