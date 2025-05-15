CC = gcc
TARGETS = pthread_basic race race_mutex producer_consumer mth_chatserv mth_chatcli netprog2
all: $(TARGETS)

pthread_basic: pthread_basic.c
    $(CC) -o pthread_basic pthread_basic.c -lpthread

race: race.c
    $(CC) -o race race.c -lpthread

race_mutex: race_mutex.c
    $(CC) -o race_mutex race_mutex.c -lpthread

producer_consumer: producer_consumer.c netprog2.o
    $(CC) producer_consumer.c netprog2.o -lpthread -o producer_consumer

mth_chatserv: mth_chatserv.c netprog2.o
    $(CC) mth_chatserv.c netprog2.o -lpthread -o mth_chatserv

mth_chatcli: mth_chatcli.c netprog2.o
    $(CC) mth_chatcli.c netprog2.o -lpthread -o mth_chatcli

netprog2: netprog2.c netprog2.h
    $(CC) -c netprog2.c