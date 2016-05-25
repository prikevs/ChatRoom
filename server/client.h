#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>

#include "common.h"
#include "network.h"

typedef struct Client {
    struct sockaddr_in client_addr;    
    User user;
    Session *session;
} Client;



#endif
