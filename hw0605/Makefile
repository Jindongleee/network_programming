CC = gcc
TARGETS = chatclient chatserver netprog2
all: $(TARGETS)

chatclient: chatclient.c netprog2.o 
	$(CC) chatclient.c netprog2.o -lpthread -o chatclient

chatserver: chatserver.c netprog2.o 
	$(CC) chatserver.c netprog2.o -lpthread -o chatserver

netprog2: netprog2.c netprog2.h
	$(CC) -c netprog2.c