
CC = gcc

ifdef DEBUG
  OPT = -g -O0 
else
  OPT = -O2 
endif

all: test

clean:
	rm -f *.o

test: test.o jims_utils.o
	$(CC) $(OPT) test.o jims_utils.o -o test

jims_utils.o:
	$(CC) $(OPT) jims_utils.c -c -o jims_utils.o

test.o: test.c jims_utils.h
	$(CC) $(OPT) -c test.c

