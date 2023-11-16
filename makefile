CC = gcc
CFLAGS = -Wall

all: s-talk

s-talk.o: s-talk.c
	gcc -Wall -c s-talk.c

list.o: list.c list.h
	gcc -Wall -c list.c


s-talk: s-talk.o list.o 
	gcc -Wall -o s-talk s-talk.o list.o -lpthread

clean:
	rm -f *.o s-talk

.PHONY: all clean
