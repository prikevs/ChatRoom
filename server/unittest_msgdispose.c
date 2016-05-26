#include "session.h"
#include "client.h"
#include "common.h"
#include "msgdispose.h"

#include <stdio.h>
#include <string.h>

int main()
{
    char name[MAXNLEN], session[MAXNLEN];
    int op, sockfd;
    initClientList();
    initSessionList();
    printf("1, reg user [sockfd, user]\n");
    printf("2, get into room [user, session]\n");
    printf("3. get out of room [user]\n");
    printf("4. unreg user by sockfd [sockfd]\n");
    printf("5. unreg user by name [name]\n");
    while(~scanf("%d", &op)) {
        if (op == 1) {
            memset(name, 0, sizeof(name));
            scanf("%d %s", &sockfd, name);
            reg(name, sockfd);
        }
        else if (op == 2) {
            memset(name, 0, sizeof(name));
            memset(session, 0, sizeof(session));
            scanf("%s %s", name, session);
            getIntoRoom(name, session);
        }
        else if (op == 3) {
            memset(name, 0, sizeof(name));
            scanf("%s", name); 
            outOfRoom(name);
        }
        else if (op == 4) {
            scanf("%d", &sockfd);
            unRegBySockfd(sockfd);
        }
        else if (op == 5) {
            memset(name, 0, sizeof(name));
            scanf("%s", name); 
            unRegByName(name);
        }
        dumpClientList();
        dumpSessionList();
    }
    return 0;
}
