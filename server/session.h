#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include "common.h"

typedef struct Session {
    LinkedList *clientlist;
    int count;
    char name[MAXNLEN];
} Session;

int initSessionList();
int addUserToSession(int, char*, char*);
int delUserFromSession(char*, char*);
LinkedList *getUsersFromSession(char*);
void destroyClientListInSession(LinkedList*);
void dumpSessionList();


#endif
