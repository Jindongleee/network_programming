#include "netprog2.h"
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msgcalc.h"

int msgqid, msgsize;
void *dequeue_and_send(void *arg);
void calculate(calcdata_t *data);

int main(int argc, char *argv[]){
    struct sockaddr_in cliaddr;
    int nbyte, sock, addrlen = sizeof(cliaddr);
    pthread_t tid;
    calcdata_t data;
    msg_t msg;
    msg.type = 1;
    msgsize = sizeof(msg_t) - sizeof(long);

    if(argc != 2){
        printf("usage : %s portn\n", argv[0]);
        exit(0);    
    }

    if((msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600)) < 0)
        errquit("msgget");
    if((sock =udp_server_socket(INADDR_ANY, atoi(argv[1])))<0)
        errquit("bind");
    pthread_create(&tid, NULL, dequeue_and_send, &sock);

    while(1){
        nbyte = recvfrom(sock, &data, sizoef(calcdata_t), 0, (struct *sockaddr) & cliaddr, &addrlen);
        if(nbyte == sizeof(calcdata_t)){
            char addr[80];
            msg.data = data;
            msg.addr = cliaddr;
            if(msgsnd(msgqid, &msg, msgsize, 0) < 0)
                errquit("msgsnd");
            inet_ntop(AF_INET, &msg.addr.sin_addr, addr, sizeof(addr));
            printf("recvfrom %s:%d (%f %c %f)\n", addr, ntohs(msg.addr.sin_port), data.op1, data.op, data.op2);
        }
        else
            errquit("wrong message format\n");
    }
    msgctl(msgqid, IPC_RMID, 0);
    return 0;
}

void *dequeue_and_send(void *arg){
    int sock = *((int *)arg), addrlen = sizeof(struct sockaddr_in);
    calcdata_t data;
    msg_t msg;
    char addr[80];

    data.type = RESPONSE;
    while(1){
        if(msgrcv(msgqid, (void *)&msg,msgsize, 0,0) < 0 )
            errquit("msgrcv");
        data = msg.data;
        calculate(&data);
        inet_ntop(AF_INET, &msg.addr.sin_addr, addr, sizeof(addr));
        printf("sendto %s: %d (%f %c %f = %f)\n", addr, ntohs(msg.addr.sin_port), data.op1, data.op, data.op2, data.result);
        if(sendto(sock, &data, sizeof(calcdata_t), 0, (struct sockaddr *)&msg.addr, addrlen)<0)
            errquit("sendto");
    }
}

void calculate(calcdata_t *data){
    (*data).flag = SUCCESS;
    if((*data).op == '+')
        (*data).result = (*data).op1 + (*data).op2;
    else if((*data).op == '-')
        (*data).result = (*data).op1 - (*data).op2;
    else if ((*data).op == '*')
        (*data).result = (*data).op1 * (*data).op2;
    else if ((*data).op == '/'){
        if((*data).op2 != 0)
            (*data).result = (*data).op1 / (*data).op2;
        else{
            (*data).flag = FAIL;
            (*data).result = 0;
        }
    }
        
}