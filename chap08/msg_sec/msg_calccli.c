#include "netprog2.h"
#include <pthread.h>
#include "msgcalc.h"

void *recv_and_print(void *arg);

int main(int argc, char *argv[]){

    struct sockaddr_in servaddr;
    int sock, addrlen = sizeof(servaddr);
    calcdata_t data;
    pthread_t tid;

    if(argc != 3){
        printf("usage: %s ipaddr port_number\n", argv[0]);
        exit(0);
    }

    if((sock=udp_client_socket(argv[1],atoi(argv[2]),&servaddr))<0)
        errquit("socket");
    pthread_create(&tid, NULL, recv_and_print, &sock);
    printf("1.2 + 2.3 형식으로 입력(지원연산 +  - * /)\n");
    while(1){
        data.type = REQUEST;
        scanf("%f %c %f", &data.op1, &data.op, &data.op2);
        if (data.op == '+' || data.op == '-' || data.op == '*' || data.op == '/'){
            if(sendto(sock, &data, sizeof(calcdata_t), 0, (struct sockaddr *)&servaddr, addrlen) < 0)
                errquit("sendto");
        }
        else
            printf("지원되지 않는 연산자입니다.\n");
    }
    return 0;
}

void *recv_and_print(void *arg){
    struct sockaddr_in servaddr;

    int sock = *((int)*arg), nbyte, addrlen = sizeof(servaddr);
    calcdata_t data;
    char op, flag;

    while(1){
        if((nbyte = recvfrom(sock, &data, sizeof(calcdata_t), 0, (struct sockaddr *)&servaddr, &addrlen))<0)
            errquit("recvfrom");
        if(nbyte == sizeof(calcdata_t)){
            if(data.flag == SUCCESS)
                printf("RESULT : %f %c %f = %f\n", data.op1, data.op, data.op2, data.result);
            else
                printf("RESULT : %f %c %f = ERROR\n", data.op1, data.op, data.op2);
        }
        else
            errquit("wrong message format\n");
    }
}