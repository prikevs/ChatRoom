#include "msghandler.h"
#include "network.h"
#include <stdlib.h>


int main()
{
    int sockfd;
    const char *ip = "127.0.0.1";
    const int port = 9999;
    if (registHandleFuncs() < 0)
        return -1;
    sockfd = initNetwork(ip, port);
    reg(sockfd, "hui");
    sleep(1);
    inRoom(sockfd, "room1");
    startReadBuffer(sockfd);
    return 0;
}
