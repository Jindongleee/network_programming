CC = gcc
TARGETS: udp_chatserv udp_chatcli
all: $(TARGETS)


udp_chatserv: udp_chatserv.c netprog2.h netprog2.c
	$(CC) -o udp_chatserv udp_chatserv.c

udp_chatcli: ud_chatcli.c netprog2.h netprog2.c
	$(CC) -o udp_chatcli udp_chatcli.c