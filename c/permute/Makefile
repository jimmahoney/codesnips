
CC = gcc

ifdef DEBUG
  OPT = -g -O0 
else
  OPT = -O2 
endif

all: test

clean:
	rm *.o

test: test.o permute.o
	$(CC) $(OPT) test.o permute.o -o test

test.o: test.c permute.h
	$(CC) $(OPT) -c test.c

permute.o: permute.c permute.h
	$(CC) $(OPT) -c permute.c

