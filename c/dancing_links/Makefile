
CC=gcc -O2

all: test

clean:
	rm *.o

test: test.o dancing_links.o
	$(CC) test.o dancing_links.o -o test

test.o: test.c dancing_links.h
	$(CC) -c test.c

dancing_links.o: dancing_links.c dancing_links.h
	$(CC) -c dancing_links.c

