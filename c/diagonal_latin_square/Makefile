
# dependencies 
U=../utilities
P=../permute
NQ=../n_queens
DL=../dancing_links
INC = -I$(NQ) -I$(P) -I$(DL) -I$(U)

CC = gcc

ifdef DEBUG
  OPT = -g -O0 
else
  OPT = -O2 
endif

OBJS = $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o $(U)/jims_utils.o

all: dls n_dls

$(U)/jims_utils.o: 
	cd $(U) ; make ; cd -

$(P)/permute.o: 
	cd $(P) ; make ; cd -

$(NQ)/n_queens.o: 
	cd $(NQ) ; make ; cd -

$(DL)/dancing_links.o: 
	cd $(DL) ; make ; cd -

dls: dls.o $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o
	$(CC) $(OPT) $(INC) dls.o $(OBJS) -o dls

dls.o: dls.c $(NQ)/queens.h $(DL)/dancing_links.h
	$(CC) $(OPT) $(INC) dls.c -c -o dls.o

n_dls: n_dls.o $(P)/permute.o $(NQ)/queens.o $(DL)/dancing_links.o $(U)/jims_utils.o
	$(CC) $(OPT) $(INC) n_dls.o $(OBJS) -o n_dls

n_dls.o: n_dls.c $(NQ)/queens.h $(DL)/dancing_links.h
	$(CC) $(OPT) $(INC) n_dls.c -c -o n_dls.o

clean:
	rm *.o

