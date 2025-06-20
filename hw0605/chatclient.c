#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s [server_ip] [port]\n", argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("서버연결 끊어짐\n");
        return 1;
    }

    char buf[1024] = {0};
    int n = recv(sock, buf, sizeof(buf) - 1, 0);
    if (n <= 0)
    {
        printf("서버연결 끊어짐\n");
        close(sock);
        return 1;
    }
    buf[n] = 0;
    int mynum = atoi(buf);
    printf("채팅 입장. 이름은 [%d]\n", mynum);

    fd_set read_fds;
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds); // stdin
        FD_SET(sock, &read_fds);

        int max_fd = sock > 0 ? sock : 0;
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(0, &read_fds))
        {
            char input[1024];
            if (!fgets(input, sizeof(input), stdin))
                break;
            send(sock, input, strlen(input), 0);
            if (strncmp(input, "/q", 2) == 0)
            {
                printf("채팅 종료\n");
                break;
            }
        }
        if (FD_ISSET(sock, &read_fds))
        {
            char msg[1024];
            int n = recv(sock, msg, sizeof(msg) - 1, 0);
            if (n <= 0)
            {
                printf("서버연결 끊어짐\n");
                break;
            }
            msg[n] = 0;
            printf("%s", msg);
        }
    }
    close(sock);
    return 0;
}
