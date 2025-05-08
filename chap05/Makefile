CC=gcc
TARGETS=mth_echoserv multicast tcp_echocli
all = ${TARGETS}

mth_echoserv: mth_echoserv.c netprog2.h netprog2.c
	$(CC) -o mth_echoserv mth_echoserv.o netprog2.o -lpthread

mth_echoserv.o: mth_echoserv.c netprog2.h
	$(CC) -c tcp_chatserv.c

multicast: multicast.c
	$(CC) -o multicast multicast.c

tcp_echocli: tcp_echocli.c netprog2.h
	$(CC) -o tcp_chatserv tcp_echocli.c