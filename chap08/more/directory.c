#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>

int main(){
    char buf[1024];
    int len = 0;
    int count = 0;
    
    DIR *dir;
    struct dirent *ent;

    dir = opendir("./");
    if(dir != NULL){
        while((ent = readdir(dir)) != NULL){
            strcpy(buf + len, ent->d_name);
            len += strlen(ent->d_name);
            buf[len++] = '\n';
            count++;
        }
        closedir(dir);
        printf("%s \n Total %d files. String length = %d\n", buf, count, len);
    }
}