
U=../utilities

CC = gcc
INC = -I$(U)

ifdef DEBUG
  OPT = -g -O0 
else
  OPT = -O2 
endif

all: test

clean:
	rm *.o

$(U)/jims_utils.o:
	cd $(U) ; make ; cd -

test: test.o permute.o $(U)/jims_utils.o
	$(CC) $(OPT) $(INC) test.o permute.o $(U)/jims_utils.o -o test

test.o: test.c permute.h $(U)/jims_utils.h 
	$(CC) $(OPT) $(INC) test.c -c -o test.o

permute.o: permute.c permute.h $(U)/jims_utils.h 
	$(CC) $(OPT) $(INC) permute.c -c -o permute.o

