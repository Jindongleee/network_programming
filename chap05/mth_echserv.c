#include "netprog2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAXLINE 511

void *echo(void *arg);

int main(int argc, char *argv[])
{
    struct sockaddr_in cliaddr;
    int listen_sock, sock,
        addrlen = sizeof(struct sockaddr_in);
    pthread_t tid;

    if (argc != 2)
    {
        printf("Usage: %s port\n", argv[0]);
        exit(0);
    }

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
    while (1)
    {
        sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
        pthread_create(&tid, NULL, echo, &sock);
    }
    return 0;
}

void *echo(void *arg)
{
    int nbyte, sock = *((int *)arg);
    char buf[MAXLINE + 1];
    printf("thread %lu started. \n", pthread_self());
    while (1)
    {
        nbyte = read(sock, buf, MAXLINE);
        if (nbyte <= 0)
        {
            close(sock);
            break;
        }
        buf[nbyte] = 0;
        printf("%lu: %s\n", pthread_self(), buf);
        write(sock, buf, nbyte);
    }
    printf("thread %lu stopped. \n", pthread_self());
}
    