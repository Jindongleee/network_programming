#include "netprog2.h"
#include <pthread.h>

#define MAXLINE 511
#define MAX_SOCK 100

void addClient(int sock);
void removeClient(int sock);
void *recv_and_send(void *arg);

int socklist[MAX_SOCK];
int num_chat = 0;
pthread_mutex_t socklist_lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr;
    int listen_sock, sock;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    if (argc != 2) {
        printf("Usage: %s port\n", argv[0]);
        exit(1);
    }

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);

    while (1) {
        sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
        addClient(sock);
    }

    return 0;
}

void addClient(int sock) {
    pthread_t tid;

    pthread_mutex_lock(&socklist_lock);
    socklist[num_chat] = sock;
    num_chat++;
    pthread_mutex_unlock(&socklist_lock);

    pthread_create(&tid, NULL, recv_and_send, &sock);
}

void removeClient(int sock) {
    int i;

    pthread_mutex_lock(&socklist_lock);
    for (i = 0; i < num_chat; i++) {
        if (socklist[i] == sock) {
            if (i != num_chat - 1) {
                socklist[i] = socklist[num_chat - 1];
            }
            num_chat--;
            break;
        }
    }
    pthread_mutex_unlock(&socklist_lock);
}

void *recv_and_send(void *arg) {
    int i, nbyte, sock = *((int *)arg);
    char buf[MAXLINE + 1];

    printf("thread %lu started.\n", pthread_self());

    while (1) {
        nbyte = read(sock, buf, MAXLINE);
        if (nbyte <= 0) {
            close(sock);
            removeClient(sock);
            break;
        }

        buf[nbyte] = 0;

        pthread_mutex_lock(&socklist_lock);
        for (i = 0; i < num_chat; i++) {
            write(socklist[i], buf, nbyte);
        }
        pthread_mutex_unlock(&socklist_lock);
    }

    printf("thread %lu stopped.\n", pthread_self());
    return NULL;
}