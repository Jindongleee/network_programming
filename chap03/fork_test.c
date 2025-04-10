#include "netprog.h"

int global_var = 0;

// 환경변수 출력
//extern char **environ;

int main()
{

    pid_t pid;
    int local_var = 0;

    if ((pid = fork()) < 0)
    {

        printf("fork error \n");
        exit(0);
    }

    else if (pid == 0)
    {

        global_var++;
        local_var++;
        printf(" CHILD - my pid -is %d and parent's pid is %d\n", getpid(), getppid());
    }

    else
    {

        sleep(2);
        global_var += 5;
        local_var += 5;
        printf("PARENT: pid = %d, child's pid = %d\n", getpid(), pid);
    }

    printf("\t global var : %d\n", global_var);
    printf("\t local var : %d\n", local_var);


    // 환경 변수 출력해보기
    // int i;

    // for ( i = 0; environ[i] != NULL; i++)
    // {
    //     printf("%s\n", environ[i]);
    // }
}