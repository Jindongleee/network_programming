#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THR 2

pthread_t who_run = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *thrfunc(void *arg);
void prn_data(pthread_t who);

int main(int argc, char **argv){

    pthread_t tid[MAX_THR];
    int i, status;

    for (i = 0; i < MAX_THR;i++){
        if((status = pthread_create(&tid[i],NULL,&thrfunc, NULL))!=0){
            printf("thread create error: %s\n", strerror(status));
            exit(0);
        }
    }

    pthread_join(tid[0], NULL);
    return 0;
}

//------------------------------------------------------------------------

void *thrfunc(void *arg){
    while(1){
		pthread_mutex_lock(&m);
        prn_data(pthread_self());
		pthread_mutex_unlock(&m);
    }

    return NULL;
}

/*
    void *thrfunc(void *arg){
    while (1)
    {
        prn_data(pthread_self());
    }
    return NULL;
    }
*/

void prn_data(pthread_t me){
    who_run = me;
    if(who_run != pthread_self()){
        printf("Error: %lu 스레드 실행중 who_run = %d\n", me, who_run);
    }
	else
		printf("%lu\n",who_run);
    who_run = 0;
}
