#include "netprog2.h"

void errquit(const char *msg)
{
    perror(msg);
    exit(1);
}

// socket() 생성 및 server 주소 담기
int udp_client_socket(const char *servip, unsigned short port,
                      struct sockaddr_in *pservaddr)
{
    int s;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    bzero((char *)pservaddr, sizeof(struct sockaddr_in));
    pservaddr->sin_family = AF_INET;
    inet_pton(AF_INET, servip, &(pservaddr->sin_addr));
    pservaddr->sin_port = htons(port);

    return s;
}

// socket() 
int udp_server_socket(int host, unsigned short port)
{
    struct sockaddr_in servaddr;
    int s, addrlen = sizeof(servaddr);

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);

    if (bind(s, (struct sockaddr *)&servaddr, addrlen) < 0)
        return -1;
    return s;
}