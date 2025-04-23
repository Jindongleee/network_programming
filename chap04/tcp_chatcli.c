#include "netprog2.h"
#define MAXLINE 1000
#define NAME_LEN 20

char *EXIT_STRING = "exit";

int main(int argc, char *argv[]){

    char bufall[MAXLINE + NAME_LEN], *bufmsg;
    int maxfdp1, s, namelen;
    fd_set read_fds;

    if(argc != 4)
        {
            printf("사용법: %s <ip> <port> <name>\n", argv[0]);
            exit(0);
        }

    sprintf(bufall,"[%s] :", argv[3]);
    namelen = strlen(bufall);
    bufmsg = bufall + namelen;

    s = tcp_connect(AF_INET, argv[1], atoi(argv[2]));

    if (s==-1)
        errquit("tcp_connect fail");

    puts("서버에 접속되었습니다.");
    maxfdp1 = s + 1;
    FD_ZERO(&read_fds);

    while(1){
        FD_SET(0, &read_fds);
        FD_SET(s, &read_fds);
        if(select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
            errquit("select fail");
        if(FD_ISSET(s, &read_fds)) {
            int nbyte;
            if ((nbyte = recv(s, bufmsg, MAXLINE, 0)) > 0) {
                bufmsg[nbyte] = 0;
                printf("%s \n", bufmsg);
            }
        }

        if(FD_ISSET(0, &read_fds)){
            if(fgets(bufmsg, MAXLINE, stdin)){
                if(send(s, bufall, namelen + strlen(bufmsg),0) < 0)
                    puts("Error: Write error on socket.");
                if(strstr(bufmsg, EXIT_STRING) != NULL){
                    puts("Good bye");
                    close(s);
                    exit(0);
                }
            }
        }
    }
}