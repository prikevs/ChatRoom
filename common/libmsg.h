#ifndef LIBMSG_H
#define LIBMSG_H
#include <stdint.h>
#include "linkedlist.h"
#include "message.h"


typedef struct User {
    int sockfd;
    char name[MAXNLEN]; 
} User;

LinkedList *parseMSG_list(const uint8_t *, uint32_t);
int genMSG_list(uint8_t*, uint32_t *, LinkedList *);

int parseMSG_in(const uint8_t*, uint32_t, char*, uint32_t*);
int genMSG_in(uint8_t*, uint32_t*, char*);
int parseMSG_reg(const uint8_t*, uint32_t, char*, uint32_t*);
int genMSG_reg(uint8_t*, uint32_t*, char*);
int genMSG_ret(uint8_t*, uint32_t*, int, const char*);

#endif
