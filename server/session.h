#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include "common.h"

typedef struct Session {
    LinkedList *clientlist;
    int count;
} Session;

void initSessionList();
int addUserToSession(char*, char*);
int delUserFromSession(char*, char*);
void dumpSessionList();


#endif
