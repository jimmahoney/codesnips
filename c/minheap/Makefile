# orginally based on an example at
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

#dependencies
U=../utilities
INC = -I. -I$(U)

CC = gcc

ifdef DEBUG
  OPT = -g -O0
else
  OPT = -O2
endif

OBJ = test_minheap.o minheap.o $(U)/jims_utils.o

all: test_minheap

clean:
	rm *.o

$U/jims_utils.o: 
	cd $(U) ; make ; cd -

minheap.o: minheap.h minheap.c 
	$(CC) $(OPT) $(INC) -c minheap.c

test_minheap.o: test_minheap.c minheap.h minheap.c
	$(CC) $(OPT) $(INC) -c test_minheap.c

test_minheap: $(OBJ)
	$(CC) $(OPT) $(INC) $(OBJ) -o test_minheap

