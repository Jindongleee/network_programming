CC = gcc
TARGETS = filecopy directory netprog2 filerecv filesend
all : $(TARGETS)

filecopy: filecopy.c
	$(CC) -o filecopy filecopy.c

directory: directory.c
	$(CC) -o directory directory.c

filemsg:

filerecv: filerecv.c netprog2.o
	$(CC) -o filerecv filerecv.c netprog2.o

filesend: filesend.c netprog2.o
	$(CC) -o filesend filesend.c netprog2.o

netprog2: netprog2.h netprog2.c
	$(CC) -c netprog2.c
