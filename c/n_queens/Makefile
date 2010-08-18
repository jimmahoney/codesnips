CC=gcc -I/opt/local/include -I../permute -L/opt/local/lib -O2

all: test

test: test.o permute.o queens.o
	$(CC) test.o permute.o queens.o -o test

permute.o: ../permute/permute.c ../permute/permute.h
	$(CC) -c ../permute/permute.c

queens.o: queens.c ../permute/permute.h queens.h
	$(CC) -c queens.c

test.o: test.c ../permute/permute.h
	$(CC) -c test.c

clean:
	rm *.o

