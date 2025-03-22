#include <stdio.h>

int main(int argc, char *argv[]) {
	if(argc < 2){
		printf("Usage: %s name\n", argv[0]);
		return 0;
	}
	printf("Hello! %s\n",argv[1]);
}
