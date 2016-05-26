#include "common.h"
#include "session.h"
#include "client.h"

#include <stdio.h>

int outOfRoom(char *name)
{
    char sename[MAXNLEN];
    memset(sename, 0, sizeof(sename));
    if (getClientSessionName(name, sename) != 0 || strlen(sename) == 0)
        return -1;
    if (modifySessionOfClient(name, NULL) != 0)
        return -1;
    if (delUserFromSession(name, sename) != 0)
        return -1;
    return 0;
}

int getIntoRoom(char *name, char *se)
{
    if (addUserToSession(name, se) != 0)
        return -1;
    if (modifySessionOfClient(name, se) != 0)
        return -1;
    return 0;
}

int reg(char *name, int sockfd)
{
    User user;
    memset(&user, 0, sizeof(user));
    strncpy(user.name, name, MIN(strlen(name), MAXNLEN-1));
    if (addClientToList(sockfd, &user, NULL) != 0)
        return -1;
    return 0;
}

int unRegByName(char *name)
{
    outOfRoom(name);
    if (delClientFromListByName(name) != 0)
        return -1;
    return 0;
}

int unRegBySockfd(int sockfd)
{
    char name[MAXNLEN];
    if (getClientNameBySockfd(sockfd, name) < 0 || strlen(name) == 0)
        return -1;
    return unRegByName(name);
}

