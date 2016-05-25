#ifndef LIBMSG_H
#define LIBMSG_H
#include <stdint.h>
#include "linkedlist.h"

#define MAXNLEN 32

typedef struct User {
    char name[MAXNLEN]; 
} User;

LinkedList *parseMSG_list(const uint8_t *, uint32_t);
int genMSG_list(uint8_t*, uint32_t *, LinkedList *);

int parseMSG_in(const uint8_t*, uint32_t, char*, uint32_t*);
int genMSG_in(uint8_t*, uint32_t*, char*);

#endif
