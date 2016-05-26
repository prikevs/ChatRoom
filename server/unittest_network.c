#include "network.h"
#include "common.h"
#include "msgdispose.h"

#include <stdio.h>
#include <string.h>

int main()
{
    char *ip = "127.0.0.1";
    int port = 9999;
    initNetwork(ip, port, msgHandler);
    return 0;
}
