CC=gcc
CFLAGS=-Wall -pedantic -std=c99
LIB=ar rcs

all: main

main: main.o libcleanerlib.a cleanerlib.h
	$(CC) $(CFLAGS) -o main main.o -L. -lcleanerlib

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

indent.o: indent.c
	$(CC) $(CFLAGS) -c indent.c

remover.o: remover.c
	$(CC) $(CFLAGS) -c remover.c

libcleanerlib.a: indent.o remover.o
	$(LIB) libcleanerlib.a indent.o remover.o

clean:
	rm -f main *.o *.a