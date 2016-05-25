#include "common.h"
#include "session.h"
#include <stdio.h>

int main()
{
    char user[MAXNLEN];
    char se[MAXNLEN];
    int op;

    initSessionList();
    memset(user, 0, sizeof(user));
    memset(se, 0, sizeof(se));
    while(~scanf("%d %s %s", &op, user, se)) {
        if (op == 1)
            addUserToSession(user, se);
        else
            delUserFromSession(user, se);
        dumpSessionList();
        memset(user, 0, sizeof(user));
        memset(se, 0, sizeof(se));
    }
    return 0;
}
