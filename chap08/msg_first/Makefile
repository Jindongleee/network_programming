CC = gcc
TARGETS = msgsnd msgrcv netprog2
all: $(TARGETS)

msgsnd: msgsnd.c netprog2.o
    $(CC) msgsnd.c netprog2.o -lpthread -o msgsnd

msgrcv: msgrcv.c netprog2.o
    $(CC) msgrcv.c netprog2.o -lpthread -o msgrcv

netprog2: netprog2.c netprog2.h
    $(CC) -c netprog2.c