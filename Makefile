CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g -D_POSIX_C_SOURCE
LIB=ar rcs

all: main cleaner

main: main.c libheaphandler.a hhlib.h log.h
	$(CC) $(CFLAGS) -o main main.c log.c -L. -lheaphandler

cleaner: cleaner.c libcleanerlib.a libheaphandler.a hhlib.h cleanerlib.h log.h
	$(CC) $(CFLAGS) -o cleaner cleaner.c log.c -L. -lcleanerlib -lheaphandler

indent.o: indent.c
	$(CC) $(CFLAGS) -c indent.c -lheaphandler

hhlib.o: hhlib.c
	$(CC) $(CFLAGS) -c hhlib.c

remover.o: remover.c hhlib.c
	$(CC) $(CFLAGS) -c remover.c -lheaphandler

libheaphandler.a: hhlib.o
	$(LIB) libheaphandler.a hhlib.o && rm -f hhlib.o

libcleanerlib.a: indent.o remover.o
	$(LIB) libcleanerlib.a indent.o remover.o && rm -f indent.o remover.o

clean:
	rm -f main cleaner *.o *.a *.log *.clean