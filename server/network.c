#include "network.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void add_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

int socket_bind(const char *ip, int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket error:"); 
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind error:");
        exit(1);
    }
    return listenfd;
}

void handle_accept(int epollfd, int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
    if (clifd == -1)
        perror("accept error:");
    else {
        printf("accept a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port); 
        add_event(epollfd, clifd, EPOLLIN);
    }
}

void do_read(int epollfd, int fd, void(* msgHandler)(int, int, char*, int))
{
    int nread;
    int status;
    char *buf;

    buf = (char *)calloc(BUFFSIZE, sizeof(char));
    nread = read(fd, buf, BUFFSIZE);
    if (nread == -1) {
        perror("read error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
        status = STATUS_offline;
    }
    else if (nread == 0) {
        fprintf(stderr, "client close.\n");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
        status = STATUS_offline;
    }
    else {
        status = STATUS_handle;
        // printf("buffer: %s\n", buf);
    }
    // Do not forget to free buf
    msgHandler(status, fd, buf, nread);
}

void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, void(* msgHandler)(int, int, char*, int))
{
    int i;
    int fd;
    for(i = 0; i < num; ++i) {
        fd = events[i].data.fd;
        
        if (events[i].events & EPOLLERR || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
            perror("epoll error");
            close(events[i].data.fd);
            continue;
        }
        if ((fd == listenfd) && (events[i].events & EPOLLIN)) 
            handle_accept(epollfd, listenfd);
        else if (events[i].events & EPOLLIN)
            do_read(epollfd, fd, msgHandler);
    }
}


void do_epoll(int listenfd, void (* msgHandler)(int, int, char*, int))
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;

    epollfd = epoll_create(FDSIZE);
    add_event(epollfd, listenfd, EPOLLIN);

    for(;;) {
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1); 
        handle_events(epollfd, events, ret, listenfd, msgHandler);
    }
    close(epollfd);
}

int initNetwork(const char *ip, const int port, void(* msgHandler)(int, int, char*, int))
{
    int sockfd;
    sockfd = socket_bind(ip, port);
    listen(sockfd, LISTENQ);
    do_epoll(sockfd, msgHandler);
    return 0;
}

