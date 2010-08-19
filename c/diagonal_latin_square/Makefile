
# dependencies 
NQ=../n_queens
DL=../dancing_links
P=../permute

CC=gcc -I/opt/local/include -I$(NQ) -I$(P) -I$(DL) -L/opt/local/lib -O2

all: dls n_dls

dls: dls.o $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o
	$(CC) dls.o $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o -o dls

dls.o: dls.c $(NQ)/queens.h $(DL)/dancing_links.h
	$(CC) -c dls.c

n_dls: n_dls.o $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o
	$(CC) n_dls.o $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o -o n_dls

n_dls.o: n_dls.c $(NQ)/queens.h $(DL)/dancing_links.h
	$(CC) -c n_dls.c

clean:
	rm *.o

