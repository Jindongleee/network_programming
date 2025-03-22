CC = gcc
TARGETS = tcp_echoserv tcp_echocli udp_echoserv udp_echocli
all: $(TARGETS)
tcp_echoserv: tcp_echoserv.c netprog.h
	$(CC) -o tcp_echoserv tcp_echoserv.c
tcp_echocli: tcp_echocli.c netprog.h
	$(CC) -o tcp_echocli tcp_echocli.c

udp_echoserv: udp_echoserv.c netprog.h
	$(CC) -o udp_echoserv udp_echoserv.c
udp_echocli: udp_echocli.c netprog.h
	$(CC) -o udp_echocli udp_echocli.c

clean:
	rm -f $(TARGETS)
