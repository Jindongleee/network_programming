#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>

void errquit(const char *msg);
int udp_client_socket(const char *servip, unsigned short port, struct sockaddr_in *pservaddr);
int udp_server_socket(int host, unsigned short port);