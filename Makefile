CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g
LIB=ar rcs

all: main cleaner

main: main.c
	$(CC) $(CFLAGS) -o main main.c

cleaner: cleaner.c libcleanerlib.a cleanerlib.h
	$(CC) $(CFLAGS) -o cleaner cleaner.c -L. -lcleanerlib

indent.o: indent.c
	$(CC) $(CFLAGS) -c indent.c

remover.o: remover.c
	$(CC) $(CFLAGS) -c remover.c

libcleanerlib.a: indent.o remover.o
	$(LIB) libcleanerlib.a indent.o remover.o && rm -f indent.o remover.o

clean:
	rm -f main cleaner *.o *.a