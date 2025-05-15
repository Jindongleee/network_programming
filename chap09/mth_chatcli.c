#include "netprog2.h"
#include <pthread.h>

#define MAXLINE 1000
#define NAME_LEN 20

char *EXIT_STRING = "exit";
void *recv_and_print(void *arg);

int main(int argc, char *argv[]) {
    char bufall[MAXLINE + NAME_LEN], *bufmsg;
    int sock, namelen;
    pthread_t tid;

    if (argc != 4) {
        printf("usage: %s server_ip port name\n", argv[0]);
        exit(0);
    }

    sprintf(bufall, "[%s]:", argv[3]);
    namelen = strlen(bufall);
    bufmsg = bufall + namelen;

    if ((sock = tcp_connect(AF_INET, argv[1], atoi(argv[2]))) < 0)
        errquit("connect");

    pthread_create(&tid, NULL, recv_and_print, &sock);

    puts("Connected to server.");

    while (1) {
        if (fgets(bufmsg, MAXLINE, stdin)) {
            if (send(sock, bufall, namelen + strlen(bufmsg), 0) < 0)
                puts("Write error on socket.");

            if (strstr(bufmsg, EXIT_STRING) != NULL) {
                puts("Good bye.");
                close(sock);
                exit(0);
            }
        }
    }

    return 0;
}

void *recv_and_print(void *arg) {
    int nbyte, sock = *((int *)arg);
    char buf[MAXLINE + 1];

    while (1) {
        if ((nbyte = recv(sock, buf, MAXLINE, 0)) > 0) {
            buf[nbyte] = 0;
            printf("%s\n", buf);
        }
    }

    return NULL;
}