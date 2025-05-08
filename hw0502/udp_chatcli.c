#include "netprog2.h"

#define MAXLINE 1000
#define NAME_LEN 20

int main(int argc, char argv[])
{

    char bufall[MAXLINE + NAME_LEN], ;

    struct sockaddr_in servaddr, cliaddr;
    int addrlen = sizeof(servaddr);

    // socket()
    int s = socket(PF_INET, SOCK_DGRAM, 0);

    // 서버 소켓 연결 준비
    bzero((char *)&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_port = htons(atio(argv[2]));


    while(1){
    scanf("%s", chat);
    // sendto()
    sendto(s, chat, sizeof(chat), 0, (struct sockaddr_in *)&servaddr, &addrlen);
    // receiv()

    recvfrom(s, chat, sizeof())
    }
}