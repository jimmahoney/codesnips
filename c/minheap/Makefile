#
# based on example at
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
#

CC=gcc

# normal:
CFLAGS=-O2 -I.
# debug: 
#CFLAGS=-O0 -g -I.

DEPENDS = minheap.h
OBJ = test_minheap.o minheap.o

all: test_minheap

clean:
	rm *.o

%.o: %.c $(DEPENDS)
	$(CC) $(CFLAGS) $< -c -o $@ 

test_minheap: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

