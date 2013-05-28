
SUBDIRS = utilities mergesort minheap permute n_queens dancing_links diagonal_latin_square

all:
	for dir in $(SUBDIRS); do  \
	  echo - $$dir - ; cd $$dir ; make ; cd .. ; \
	done

clean:
	for dir in $(SUBDIRS); do  \
	  echo - $$dir clean - ; cd $$dir ; make clean ; cd .. ; \
	done
