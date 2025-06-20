#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>

#define MAX_CLIENTS 10

int client_socks[MAX_CLIENTS];
int client_count = 0;
int server_sock = -1;

void remove_client(int sock) {
    int i, j;
    for (i = 0; i < client_count; ++i)
    {
        if (client_socks[i] == sock)
        {
            for (j = i; j < client_count - 1; ++j)
                client_socks[j] = client_socks[j + 1];
            client_count--;
            break;
        }
    }
}

void broadcast(const char *msg, int except)
{
    int i;
    for (i = 0; i < client_count; ++i)
    {
        if (client_socks[i] != except)
            send(client_socks[i], msg, strlen(msg), 0);
    }
}

void send_user_list(int sock)
{
    int i;
    char buf[256] = "사용자 목록 :";
    for (i = 0; i < client_count; ++i)
    {
        char num[16];
        sprintf(num, " %d", client_socks[i]);
        strcat(buf, num);
    }
    strcat(buf, "\n");
    send(sock, buf, strlen(buf), 0);
}

void cleanup(int signo)
{
    int i;
    for (i = 0; i < client_count; ++i)
    {
        close(client_socks[i]);
    }
    if (server_sock != -1)
        close(server_sock);
    exit(0);
}

int main(int argc, char *argv[])
{
    int i;
    if (argc != 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }
    signal(SIGINT, cleanup);

    int port = atoi(argv[1]);
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(server_sock, MAX_CLIENTS);

    fd_set read_fds;
    int max_fd = server_sock;

    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(server_sock, &read_fds);
        for (i = 0; i < client_count; ++i)
            FD_SET(client_socks[i], &read_fds);

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(server_sock, &read_fds))
        {
            int new_sock = accept(server_sock, (struct sockaddr *)&cli_addr, &cli_len);
            if (client_count < MAX_CLIENTS)
            {
                client_socks[client_count++] = new_sock;
                if (new_sock > max_fd)
                    max_fd = new_sock;
                char num[16];
                sprintf(num, "%d\n", new_sock);
                send(new_sock, num, strlen(num), 0);
                printf("사용자 %d 연결\n", new_sock);
            }
            else
            {
                close(new_sock);
            }
        }

        for (i = 0; i < client_count; ++i)
        {
            int sock = client_socks[i];
            if (FD_ISSET(sock, &read_fds))
            {
                char buf[1024] = {0};
                int n = recv(sock, buf, sizeof(buf) - 1, 0);
                if (n <= 0)
                {
                    printf("사용자 %d 종료\n", sock);
                    close(sock);
                    remove_client(sock);
                    continue;
                }
                buf[n] = 0;
                if (strncmp(buf, "/m ", 3) == 0)
                {
                    char msg[1100];
                    sprintf(msg, "[%d] %s", sock, buf + 3);
                    broadcast(msg, -1);
                }
                else if (strncmp(buf, "/w", 2) == 0)
                {
                    send_user_list(sock);
                }
                else if (strncmp(buf, "/q", 2) == 0)
                {
                    printf("사용자 %d 종료\n", sock);
                    close(sock);
                    remove_client(sock);
                }
            }
        }
    }
    return 0;
}
