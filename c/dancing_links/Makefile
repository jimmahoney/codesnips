
# dependencies
U=../utilities
INC = -I$(U) 

CC = gcc

ifdef DEBUG
  OPT = -g -O0
else
  OPT = -O2
endif

all: test

clean:
	rm -f *.o

$U/jims_utils.o:
	cd $(U) ; make ; cd -

test: test.o dancing_links.o 
	$(CC) $(OPT) $(INC) test.o $(U)/jims_utils.o dancing_links.o -o test

test.o: test.c dancing_links.h
	$(CC) $(OPT) $(INC) -c test.c

dancing_links.o: dancing_links.c dancing_links.h $U/jims_utils.o
	$(CC) $(OPT) $(INC) -c dancing_links.c

