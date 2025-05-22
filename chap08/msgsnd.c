#include "netprog2.h"
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFSZ 512

typedef struct _msg{
    long msg_type;
    char msg_text[BUFSZ];
} msg_t;

void message_send(int qid, long type, const char *text);

int main(int argc, char **argv){
    if(argc!=2){
        printf("Usage: %s msqkey\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    key_t key = atoi(argv[1]);

    int qid = msgget(key, IPC_CREAT | 0600);
    if(qid<0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    char text[BUFSZ];
    puts("Enter Message to post: ");
    fgets(text, BUFSZ, stdin);
    message_send(qid, 3, text);

    puts("Enter Message to post: ");
    fgets(text, BUFSZ, stdin);
    message_send(qid, 2, text);
    puts("Enter Message to post: ");

    fgets(text, BUFSZ, stdin);
    message_send(qid, 1, text);
}

void message_send(int qid, long type, const char *text){
    msg_t pmsg;
    pmsg.msg_type = type;
    int len = strlen(text);
    strncpy(pmsg.msg_text, text, len);
    pmsg.msg_text[len] = 0;
    msgsnd(qid, &pmsg, len, 0);
}