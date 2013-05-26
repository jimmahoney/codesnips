
# dependencies
P=../permute
INC = -I$(P) 

CC = gcc

ifdef DEBUG
  OPT = -g -O0
else
  OPT = -O2
endif

all: test

test: test.o $(P)/permute.o queens.o
	$(CC) $(OPT) $(INC) test.o queens.o $(P)/permute.o -o test

$(P)/permute.o: $(P)/permute.c $(P)/permute.h
	cd $(P) ; make ; cd -

queens.o: queens.h queens.c $(P)/permute.h
	$(CC) $(OPT) $(INC) queens.c -c -o queens.o

test.o: test.c
	$(CC) $(OPT) $(INC) test.c -c -o test.o

clean:
	rm -f *.o

