#ifndef FILEMSG_H   
#define FILEMSG_H
#include <dirent.h>
#define REQ_DIR 0;
#define RSP_DIR 1
#define REQ_GETFILE 2
#define RSP_GETFILE 3
#define REQ_PUTFILE 4
#define RSP_PUTFILE 5
#define SUCCESS 0
#define FAILURE 1

typedef struct _filemsg{
    char type, flag, filename[NAME_MAX + 1];
    uint32_t length;
} filemsg;
#define BUFSIZE 1024
#endif