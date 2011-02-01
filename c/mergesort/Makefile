
CC=gcc -O2

all: test_utilities test_merge time_merge

clean:
	rm *.o

%.o: %.c
	$(CC) -c $< -o $@
# That confusing mumbo-jumbo just says to do this :
#  foo.o : foo.c
#        $(CC) -c foo.c -o foo.o
# to make any *.o from a *.c ; see for example
# http://www.gnu.org/software/make/manual/make.html#Pattern-Rules

test_utilities: jims_utilities.o test_utilities.o
	$(CC) jims_utilities.o test_utilities.o -o test_utilities

test_merge: jims_utilities.o mergesort.o test_merge.o
	$(CC) jims_utilities.o mergesort.o test_merge.o -o test_merge

time_merge: jims_utilities.o mergesort.o time_merge.o
	$(CC) jims_utilities.o mergesort.o time_merge.o -o time_merge




