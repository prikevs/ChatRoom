#include "common.h"
#include "client.h"
#include <stdio.h>
#include <string.h>

int main()
{
    User user;
    char session[MAXNLEN];
    int ret, sockfd, op;
    initClient();
    memset(user.name, 0, sizeof(user.name));
    while(~scanf("%d %s %d %s", &op, user.name, &sockfd, session)) {
        if (op == 1) {
            ret = addClientToList(sockfd, &user, session); 
            printf("%d\n", ret);
        }
        else {
            ret = delClientFromListByName(user.name); 
            printf("%d\n", ret);
        }
        dumpClientList();
        memset(user.name, 0, sizeof(user.name));
    }
    return 0;
}
