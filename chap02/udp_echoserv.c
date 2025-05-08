#include "netprog.h"
#define MAXLINE 511

int main(int argc, char*argv[]) {

	struct sockaddr_in servaddr, cliaddr; // struct 생성
	int s, nbyte, addrlen = sizeof(struct sockaddr); // 주소길이 초기화
	char buf[MAXLINE+1]; // buf 생성

	if (argc != 2){
		printf("usage: %s prot\n", argv[0]);
		exit(0);
	}

	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		errquit("socket fail");
	}

	// Initialize server address with null character ('\0')
	bzero((char *)&servaddr, addrlen);
	bzero((char *)&cliaddr, addrlen);

	// servaddr setting
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	// calling bind()
	if (bind(s, (struct sockaddr *)&servaddr, addrlen) < 0) {
		errquit("bind fail");
	}

	// do iterative echo service
	while(1) {
		puts("Server : waiting request.");
		nbyte = recvfrom(s, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &addrlen);

		if (nbyte < 0) {
			errquit("recvfrom fail");
		}

		buf[nbyte] = 0;
		printf("%d byte recv: %s\n", nbyte, buf);

		if(sendto(s, buf, nbyte, 0, (struct sockaddr *)&cliaddr, addrlen) < 0) {
			errquit("sendto fail");
		}

		puts("sendto complete");
	}
}
