#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> 
#include <pthread.h>

#define MAX_THR 2
void *thrfunc(void *arg); //스레드 함수 선언
// 스레드 함수는 void*를 인자로 받고, void*를 반환하는 형태로 정의
// pthread_create() 함수는 스레드 함수를 실행하기 위해 이 형식을 요구
void prn_data(pthread_t who); //thread Id 출력
pthread_t who_run = 0; // 전역 변수로 스레드 ID를 저장
// 전역 변수는 모든 스레드에서 공유되어   // 스레드가 실행 중인 동안에만 유효하며, 스레드가 종료되면 값이 사라짐

int main(int argc, char **argv){
    pthread_t tid[MAX_THR]; // 스레드 ID를 저장하기 위한 배열
    // pthread_t는 스레드 ID를 나타내는 자료형으로, 스레드를 식별하는 데 사용됨
    // MAX_THR는 최대 스레드 개수를 정의한 상수로, 이 예제에서는 2개의 스레드를 생성함
    
    int i, status; // 스레드 생성 상태를 저장할 변수

    // status 변수는 pthread_create() 함수의 반환 값을 저장하여 스레드 생성 성공 여부를 확인하는 데 사용됨
    for (int i = 0; i < MAX_THR;i++){
        pthread_create(&tid[i], NULL, &thrfunc, NULL); // 스레드 생성
        // pthread_create() 함수는 새로운 스레드를 생성하는 함수로,
        // 첫 번째 인자는 생성된 스레드의 ID를 저장할 변수의 주소,
        // 두 번째 인자는 스레드 속성을 지정하는 포인터 (NULL은 기본 속성 사용),
        // 세 번째 인자는 실행할 스레드 함수의 포인터,
        // 네 번째 인자는 스레드 함수에 전달할 인자 (NULL은 인자가 없음)를 지정함
    }

    pthread_join(tid[0], NULL); // tid[0] 스레드가 종료되기 전까지 종료 x
    return 0;
}

// 이 프로그램은 두 개의 스레드를 생성하고, 각 스레드는 무한 루프에서 자신의 스레드 ID를 출력함
// 스레드가 실행 중인 동안에만 유효하며, 스레드가 종료되면 값이 사라짐
void *thrfunc(void *arg){
    while(1){
        prn_data(pthread_self());
    }
    return NULL;
}

// prn_data 함수는 현재 실행 중인 스레드의 ID를 출력하고,
void prn_data(pthread_t me){
    who_run = me;
    if(who_run != pthread_self())
        printf("Error: %d 스레드 실행중 who_run = %d\n", me, who_run);
    who_run = 0; //초기값 지정
}