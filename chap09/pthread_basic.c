#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


void *thrfunc(void *arg);

char who[10];
int main(int argc, char **argv){
    int status;
    pthread_t tid;
    pid_t pid;

    pid = fork();
    
    if(pid == 0)
        sprintf(who, "child");
    else
        sprintf(who, "parent");

    printf("(%s's main) Process ID = %d\n", who, getpid());
    printf("(%s's main) Init thread ID = %d\n", who, pthread_self());

    if((status=pthread_create(&tid, NULL, &thrfunc, NULL) != 0)){
        printf("thread create error: %s\n", strerror(status));
        exit(0);
    }

    pthread_join(tid, NULL);
    printf("\n(%s)[%d] 스레드가 종료했습니다\n", who, tid);
    return 0;

}

void *thrfunc(void *arg){
    printf("(%s' thread routine) Process ID = %d\n", who, getpid());
    printf("(%s' thread routine) Thread ID = %d\n",who,pthread_self());
}