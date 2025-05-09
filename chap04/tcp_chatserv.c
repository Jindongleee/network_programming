#include "netprog2.h"

#define MAXLINE 511
#define MAX_SOCK 1024

char *EXIT_STRING = "exit";
char *START_STRING = "COnnected to chat_server \n";

int maxfdp1;
int num_chat = 0;
int clisock_list[MAX_SOCK];
int listen_sock;
void addClient(int s, struct sockaddr_in *newcliaddr);
int getMax();
void removeClient(int s);

int main(int argc, char *argv[])
{

    struct sockaddr_in cliaddr;
    char buf[MAXLINE + 1];
    int i, j, nbyte, accp_sock, addrlen = sizeof(struct sockaddr_in);
    fd_set read_fds;

    if (argc != 2)
    {
        printf("사용법: %s <port>\n", argv[0]);
        exit(0);
    }

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(listen_sock, &read_fds);

        for (i = 0; i < num_chat; i++)
        {
            FD_SET(clisock_list[i], &read_fds);
        }

        maxfdp1 = getMax() + 1;
        if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
        {
            errquit("select fail");
        }

        if (FD_ISSET(listen_sock, &read_fds))
        {
            accp_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
            if (accp_sock == -1)
            {
                errquit("accept fail");
            }
            addClient(accp_sock, &cliaddr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("%d번째 사용자 추가.\n", num_chat);
        }

        for (i = 0; i < num_chat; i++)
        {
            if (FD_ISSET(clisock_list[i], &read_fds))
            {
                nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
                if (nbyte <= 0)
                {
                    removeClient(i);
                    printf("%d번째 사용자 삭제.\n", i);
                }
                buf[nbyte] = '\0';
                if (strstr(buf, EXIT_STRING) != NULL)
                {
                    removeClient(i);
                    continue;
                }

                for (j = 0; j < num_chat; j++)
                    send(clisock_list[j], buf, nbyte, 0);
                printf("%s\n", buf);
            }
        }
    }
    return 0;
}

void addClient(int s, struct sockaddr_in *newcliaddr)
{
    char buf[20];
    inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
    printf("new client: %s\n", buf);
    clisock_list[num_chat] = s;
    num_chat++;
}

void removeClient(int s)
{
    close(clisock_list[s]);
    if (s != num_chat - 1)
    {
        clisock_list[s] = clisock_list[num_chat - 1];
    }
    num_chat--;
    printf("채팅 참가자 1명 탈퇴. 현재 참가자 수  = %d\n", num_chat);
}

int getMax()
{
    int max = listen_sock;
    int i;

    for (i = 0; i < num_chat; i++)
    {
        if (clisock_list[i] > max)
        {
            max = clisock_list[i];
        }
    }
    return max;
}