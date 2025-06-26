CC = gcc
TARGETS = msg_calccli msg_calcserv netprog2
all: $(TARGETS)

msg_calccli: msg_calccli.c netprog2.o msgcalc.h
    $(CC) msg_calccli.c netprog2.o msgclac.h -lpthread -o msg_calccli

msg_calcserv: msg_calcserv.c netprog2.o
    $(CC) msg_calcserv.c netprog2.o msgclac.h -lpthread -o msg_calcserv

netprog2: netprog2.c netprog2.h
    $(CC) -c netprog2.c