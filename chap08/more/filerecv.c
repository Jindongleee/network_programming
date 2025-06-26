#include "netprog2.h"
#define BUFSIZE 1024

int main(int argc, char *argv[]){
    struct sockaddr_in servaddr;
    int sock, nbyte;
    char buf[BUFSIZE];
    FILE *fp;

    if(argc!=3){
        printf("usage: %s ipaddr port\n", argv[0]);
        exit(0);
    }

    sock = tcp_connect(AF_INET, argv[1], atoi(argv[2]));

    if(sock == -1)
        errquit("Connection fail\n");

    printf("Filename to get:");
    fgets(buf, sizeof(buf), stdin);
    nbyte = strlen(buf);
    buf[nbyte - 1];
    write(sock, buf, nbyte);

    fp = fopen(buf, "wb");
    if(!fp){
        close(sock);
        printf("Cannot open %s file.\n", buf);
        return 0;
    }

    while(1){
        nbyte = read(sock, buf, BUFSIZE);
        if(nbyte <=0){
            close(sock);
            fclose(fp);
            break;
        }

        fwrite(buf, 1, nbyte, fp);
    }

    return 0;
}