
CC=gcc -O2

all: test

clean:
	rm *.o

test: test.o permute.o
	$(CC) test.o permute.o -o test

test.o: test.c permute.h
	$(CC) -c test.c

permute.o: permute.c permute.h
	$(CC) -c permute.c

