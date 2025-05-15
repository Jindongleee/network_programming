#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_CONSUMER 10

int resource_count = 0;
pthread_mutex_t resource_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t resource_cond = PTHREAD_COND_INITIALIZER;

void *consumer_func(void *arg);
void *producer_func(void *arg);

int main(int argc, char *argv[])
{
    int i;
    int id[NUM_CONSUMER];
    pthread_t consumer_thread[NUM_CONSUMER];
    pthread_t producer_thread;

    for (i = 0; i < NUM_CONSUMER; i++)
    {
        id[i] = i;
        pthread_create(&consumer_thread[i], NULL, consumer_func, &id[i]);
    }

    pthread_create(&producer_thread, NULL, producer_func, NULL);
    pthread_join(producer_thread, NULL);

    return 0;
}

void *consumer_func(void *arg)
{
    int trial = 0, success = 0;
    int id = *((int *)arg);

    while (1)  // lock -> 자원 -> unlock
    {
        pthread_mutex_lock(&resource_lock); // mutex lock

        // 자원이 없으면 기다림
        // while (resource_count == 0)
        // {
        //     pthread_cond_wait(&resource_cond, &resource_lock); 
        // }

        trial++;

        if (resource_count > 0)
        {
            resource_count--;  // 자원 하나 사용
            success++; // 처리 횟수
        }

        pthread_mutex_unlock(&resource_lock);

        printf("Consumer %2d: trial = %d, success = %d\n", id, trial, success);
    }
    return NULL;
}

void *producer_func(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&resource_lock);

        resource_count++;

        sleep(10);

        pthread_cond_broadcast(&resource_cond); // 자원 생산했어요 깨어나셔도돼요

        pthread_mutex_unlock(&resource_lock);
    }
    return NULL;
}