#include "netprog.h"
#define MAXLINE 511

struct calcmsg
{

    char type;
    char status;
    char op;
    char padding;
    int32_t op1, op2, result;
};

int main(int argc, char *argv[])
{

    struct sockaddr_in servaddr, cliaddr;            // struct 생성
    int s, nbyte, addrlen = sizeof(struct sockaddr); // 주소길이 초기화
    char buf[MAXLINE + 1];                           // buf 생성
    struct calcmsg rspmsg, reqmsg;                   // reqmsg = client

    if (argc != 2)
    {
        printf("usage: %s prot\n", argv[0]);
        exit(0);
    }

    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        errquit("socket fail");
    }

    // Initialize server address with null character ('\0')
    bzero((char *)&servaddr, addrlen);
    bzero((char *)&cliaddr, addrlen);

    // servaddr setting
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    // calling bind()
    if (bind(s, (struct sockaddr *)&servaddr, addrlen) < 0)
    {
        errquit("bind fail");
    }

    // do iterative echo service
    while (1)
    {
        puts("Server : waiting request.");
        nbyte = recvfrom(s, &reqmsg, sizeof(struct calcmsg), 0, (struct sockaddr *)&cliaddr, &addrlen);

        char op;
        int32_t op1, op2, result;
        op = reqmsg.op;
        op1 = ntohl(reqmsg.op1);
        op2 = ntohl(reqmsg.op2);

        switch (reqmsg.op)
        {
        case '+':
            result = op1 + op2;
            break;
        case '-':
            result = op1 - op2;
            break;
        case '*':
            result = op1 * op2;
            break;
        case '/':
            if (op2 == 0)
                printf("calc fail");
            else
                result = op1 / op2;
        };

        rspmsg.status = 0;
        rspmsg.result = htonl(result);

        if (sendto(s, &rspmsg, sizeof(struct calcmsg), 0, (struct sockaddr *)&cliaddr, addrlen) < 0)
        {
            errquit("sendto fail");
        }

        puts("sendto complete");
    }
}