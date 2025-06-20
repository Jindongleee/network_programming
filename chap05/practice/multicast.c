#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> 

#define MAXLINE 1023

// 멀티캐스트를 통한 통신
int main(int argc, char *argv[]){ // 파일이름 ip, port, 사용자 이름 [0] [1] [2] [3]
    int send_s; // 송신용 소켓
    int recv_s; // 수신용 소켓
    int n;
    int len;
    pid_t pid;

    unsigned int yes = 1;
    struct sockaddr_in mcast_group; // 멀티캐스트 그룹 주소 

    struct ip_mreq mreq; // 멀티캐스트 그룹 주소, 자신의 주소

    char name[10]; // 채팅 참가자 이름

    sprintf(name, "[%s]", argv[3]); //name 버퍼에 argv[3]의 int 를 문자열로 저장한다

    // 멀티 캐스트 "수신용" 소켓 개설 및 구조체 모든 바이트를 0으로 초기회
    memset(&mcast_group, 0, sizeof(mcast_group));
    mcast_group.sin_family = AF_INET; // Ipv4주소 체계를 사용하겠다는 의미
    mcast_group.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &mcast_group.sin_addr); // 문자열 형식의 IPv4 주소(argv[1])를 네트워크 바이트 순서의 이진 형식으로 변환하여 sin_addr에 저장

    recv_s = socket(AF_INET, SOCK_DGRAM, 0); // 수신용 소켓 생성 udp

    // 멀티캐스트 그룹에 가입 -> 멀티캐스트 그룹을 누군가가 만드는게 아니라 직접 가입하는 것임 !!
    mreq.imr_multiaddr = mcast_group.sin_addr; // 멀티캐스트 그룹 주소 생성
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); // 내가 수신할 ip를 지정하는 것 따라서 INADDR_ANY이므로 ip를 가리지 않고 받겠다.

    // IP 레벨이기 떄문에 IPROTO_IP이고 이중 가입이므로 IP_ADD_MEMBERSHIP
    setsockopt(recv_s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)); // 멀티캐스트 그룹에 해당 소켓을 가입
    
    // 멀티캐스트 수신용 소켓 바인딩
    bind(recv_s, (struct sockaddr *)&mcast_group, sizeof(mcast_group));

//--------------------------------------------------------------------
    
    // listen() 없음 소켓 생성
    send_s = socket(AF_INET, SOCK_DGRAM, 0);

    pid = fork();

    // fork() 실행: child는 수신 담당 parent는 송신 담당
    if ( pid == 0 ) {
        struct sockaddr_in from;
        char message[MAXLINE + 1];
        for (;;){
            printf("receiving message..\n");
            len = sizeof(from);
            n = recvfrom(recv_s, message, MAXLINE, 0, (struct sockaddr *)&from, &len);
            message[n] = 0;
        }
    }
    // 부모
    else{
        char message[MAXLINE + 1], line[MAXLINE + 1];
        while(fgets(message, MAXLINE, stdin) != NULL){ // NULL은 EOF
            sprintf(line, "%s %s", name, message);
            len = strlen(line);
            sendto(send_s, line, strlen(line), 0, (struct sockaddr *)&mcast_group, sizeof(mcast_group));
        }
    }
}