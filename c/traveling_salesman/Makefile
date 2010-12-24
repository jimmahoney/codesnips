CC=gcc -I/opt/local/include -I../permute -L/opt/local/lib -O2

all: test

test: test.o permute.o tsp.o
	$(CC) test.o permute.o tsp.o -o test

permute.o: ../permute/permute.c ../permute/permute.h
	$(CC) -c ../permute/permute.c

tsp.o: tsp.c ../permute/permute.h tsp.h
	$(CC) -c tsp.c

test.o: test.c ../permute/permute.h
	$(CC) -c test.c

clean:
	rm *.o

