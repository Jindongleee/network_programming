#include "netprog2.h"
#include <dirent.h>
#include <pthread.h>
#include "filemsg.h"
#include <sys/stat.h>

void *handle_client(void *arg);
void send_dir(int sock);
void send_file(int sock, const char *filename);
void recv_file(int sock, const char *filename, uint32_t filelen);

int main(int argc, char *argv[]){
    struct sockaddr_in cliaddr;
    int listen_sock, sock, addrlen = sizeof(struct sockaddr_in), nbyte;

    if(argc!=2){
        printf("Usage")
    }
}