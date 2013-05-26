
#dependencies
U=../utilities
INC = -I$(U)

CC = gcc

ifdef DEBUG
  OPT = -g -O0
else
  OPT = -O2
endif

all: test_merge time_merge

clean:
	rm -f *.o

$(U)/jims_utils.o: 
	cd $(U) ; make ; cd -

test_merge.o: test_merge.c 
	$(CC) $(OPT) $(INC) test_merge.c -c -o test_merge.o

time_merge.o: time_merge.c 
	$(CC) $(OPT) $(INC) -lm time_merge.c -c -o time_merge.o

mergesort.o: mergesort.c
	$(CC) $(OPT) $(INC) mergesort.c -c -o mergesort.o

test_merge: mergesort.o test_merge.o $(U)/jims_utils.o 
	$(CC) $(OPT) $(INC) $(U)/jims_utils.o mergesort.o test_merge.o -o test_merge

time_merge: mergesort.o time_merge.o $(U)/jims_utils.o 
	$(CC) $(OPT) $(INC) -lm $(U)/jims_utils.o mergesort.o time_merge.o -o time_merge




