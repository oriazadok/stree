CC = gcc
CFLAGS = -g -Wall


all: stree

stree: stree.o
	$(CC) $(CFLAGS) -o stree stree.o

stree.o: stree.c
	$(CC) $(CFLAGS) -c stree.c


.PHONY: all clean

clean:
	-rm stree *.o 2>/dev/null