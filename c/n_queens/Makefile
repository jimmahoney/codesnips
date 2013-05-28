
# dependencies
P=../permute
U=../utilities
INC = -I$(P) -I$(U)

CC = gcc

ifdef DEBUG
  OPT = -g -O0
else
  OPT = -O2
endif

all: test

$(P)/permute.o: $(P)/permute.c $(P)/permute.h
	cd $(P) ; make ; cd -

$(U)/jims_utils.o: $(U)/jims_utils.c $(U)/jims_utils.h 
	cd $(U) ; make ; cd -

test: test.o queens.o $(P)/permute.o $(U)/jims_utils.o 
	$(CC) $(OPT) $(INC) test.o queens.o $(P)/permute.o $(U)/jims_utils.o -o test

queens.o: queens.h queens.c $(P)/permute.h $(U)/jims_utils.h 
	$(CC) $(OPT) $(INC) queens.c -c -o queens.o

test.o: test.c
	$(CC) $(OPT) $(INC) test.c -c -o test.o

clean:
	rm -f *.o

