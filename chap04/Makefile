CC = gcc
TARGETS = tcp_chatserv tcp_chatcli
OBJS = netprog2.o tcp_chatserv.o tcp_chatcli.o
all: $(TARGETS)
tcp_chatserv: tcp_chatserv.o netprog2.o
	$(CC) -o tcp_chatserv tcp_chatserv.o netprog2.o  # compile 과 linking을 따로하자
tcp_chatserv.o: tcp_chatserv.c netprog2.h
	$(CC) -c tcp_chatserv.c
tcp_chatcli: tcp_chatcli.o netprog2.o
	$(CC) -o tcp_chatcli tcp_chatcli.o netprog2.o
tcp_chatcli.o: tcp_chatcli.c netprog2.h
	$(CC) -c tcp_chatcli.c
netprog2.o: netprog2.c netprog2.h
	$(CC) -c netprog2.c
clean:
	rm -f $(TARGETS) $(OBJS)