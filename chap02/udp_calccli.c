#include "netprog.h"

#define MAXLINE 511

struct calcmsg
{
    char type;   // 0:REQ, 1:RSP
    char status; // 0:SUCCESS, 1: DIVIDE_BY_ZERO, 2: OVERFLOW
    char op;     // 0:ADD,1 1: SUBTRACT, 2:MULTPLY, 3:DIVIDE
    char padding;
    int32_t op1, op2, result;
};

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    int s, nbyte, addrlen = sizeof(servaddr);
    char buf[MAXLINE + 1];

    char op;
    int32_t op1, op2;
    struct calcmsg reqmsg;
    struct calcmsg rspmsg;

    if (argc != 3)
    {

        printf("usage: %s ip_address port_number\n", argv[0]);
        exit(0);
    }

    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {

        errquit("socket fail");
    }

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_port = htons(atoi(argv[2]));
    printf("입력: ");
    scanf("%d %c %d", &op1, &op, &op2);

    // 구조체에 넣기
    reqmsg.op = op;
    reqmsg.op1 = htonl(op1);
    reqmsg.op2 = htonl(op2);

    // 구조체 변수명 or 타입명
    if (sendto(s, &reqmsg, sizeof(struct calcmsg), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        errquit("sendto fail");

    //
    if ((nbyte = recvfrom(s, &rspmsg, sizeof(struct calcmsg), 0, (struct sockaddr *)&servaddr, &addrlen)) < 0)
    {
        errquit("recevfrom fail");
    }

    if (rspmsg.status == 0)
    {

        int32_t result = ntohl(rspmsg.result);

        printf("계산 결과: %d", result);
    }
}