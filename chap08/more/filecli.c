#include "netprog2.h"
#include "filemsg.h"
#include <sys/stat.h>
void list_dir(int sock);
void get_file(int sock);
void put_file(int sock);

int main(int argc, char *argv[]){
    struct sockaddr_in servaddr;

    int sock, menu;

    if(argc!=3){
        printf("usage: %s ipaddr port\n", argv[0]);
        exit(0);
    }

    sock = tcp_connect(AF_INET, argv[1], atoi(argv[2]));

    if(sock == -1)
        errquit("Connection fial");
    while(1){
        printf("1.dir 2.get file 3.put file 4.exit:");
        scanf("%d", &menu);
        getchar();

        if(menu == 1)
            list_dir(sock);
        else if(menu == 2)
            get_file(sock);
        else if(menu == 3)
            put_file(sock);
        else if(menu == 4)
            break;
        
    }
    close(sock);

}

void list_dir(int sock){
    char buf[BUFSIZE];
    int len, nbyte, count;

    filemsg sendmsg;
    filemsg recvmsg;

    sendmsg.type = REQ_DIR;

    sendmsg.length = htonl(0);
    write(sock, &sendmsg, sizeof(sendmsg));
    read(sock, &recvmsg, sizeof(recvmsg));

    if(recvmsg.type == RSP_DIR && recvmsg.flag == EXIT_SUCCESS){
        nbyte = read(sock, buf, BUFSIZE);
        if(nbyte == ntohl(recvmsg.length)){
            len = 0;
            count = 0;
            while(len < nbyte){
                printf("%s \n", buf + len);
                len += strlen(buf + len) + 1;
                count++;
            }
        }
        printf("Total %d files ... \n", count);

    }
    else
        printf("Directory request failed...\n");
}

void get_file(int sock){
    chat buf[BUFSIZE];
    chat filename[256];
    int len;
    int nbyte;
    FILE *fp;
    filemsg sendmsg;
    filemsg recvmsg;

    printf("Filename to get:");
    fgets(filename, sizeof(filename), stdin);
    len = strlen(filename);
    filename[len-1] = 0;
    sendmsg.type = REQ_GETFILE;
    strncpy(sendmsg.filename, filename, 256);
    sendmsg.length = htonl(len);

    write(sock, &sendmsg, sizeof(sendmsg));
    read(sock, &recvmsg, sizeof(recvmsg));
    
    if(recvmsg.type == RSP_GETFILE && recvmsg.flag == SUCCESS){
        if(fp = fopen(filename, "wb")){
            int filelen = ntohl(recvmsg.length);
            int byteRcvd = 0;

            do{
                nbyte = read(sock, buf, BUFSIZE);
                fwrite(buf, 1, nbyte, fp);
                byteRcvd += nbyte;
            } while (byteRcvd < filelen);

            fclose(fp);
            printf("File transfer completed.\n");
        }
    }
    else
        printf("Get request failed. \n");
}

void put_file(int sock){
    char buf[BUFSIZE], filename[256];
    int len, nbyte;
    FILE *fp;

    struct stat filestat;

    filemsg sendmsg;
    filemsg recvmsg;

    printf("Filename to put:");
    fgets(filename, sizeof(filename), stdin);

    len = strlen(filename);

    filename[len - 1] = 0;

    if( fp = fopen(filename,"rb")){
        lstat(filename, *filestat);
        sendmsg.type = REQ_PUTFILE;
        strncpy(sendmsg.filename, filename, 256);
        sendmsg.length = htonl(filestat.st_size);
        write(sock, &sendmsg, sizeof(sendmsg));
        while(!feof(fp)){
            nbyte = fread(buf, 1, BUFSIZE, fp);
            if(nbyte>0)
                write(sock, buf, nbyte);
        }
        read(sock, &recvmsg, sizeof(recvmsg));
        if(recvmsg.type == RSP_PUTFILE && recvmsg.flag == EXIT_SUCCESS)
            printf("File transfer completed \n");
        else
            printf("Put request failed.\n");
        fclose(fp);
    }
    else
        printf("Cannot open file.\n");
        
}