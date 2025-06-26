#include <stdio.h>

int main(int argc, char *argv[]){
    FILE *fp1, *fp2;
    char buf[1024];

    int nbyte;

    if(argc !=3 ){
        printf("UsageL %s src target\n", argv[0]);
        return 1;
    }

    if(!(fp1 = fopen(argv[1],"rb"))){
        printf("src file error\n");
        return 1;
    }

    if(!(fp2 = fopen(argv[2],"wb"))){
        printf("target file error\n");
        return 1;
    }

    while(!feof(fp1)){
        nbyte = fread(buf, 1, 1024, fp1);
        if(nbyte > 0)
            fwrite(buf, 1, nbyte, fp2);
    }

    fclose(fp1);
    fclose(fp2);
}