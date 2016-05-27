#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "common.h"

#define EPOLLEVENTS 100
#define FDSIZE 1000
#define LISTENQ 5

enum statusType{
    STATUS_offline,
    STATUS_handle, 
};

int startNetwork(const char*, const int, void(*)(int, int,char*,int));

#endif
