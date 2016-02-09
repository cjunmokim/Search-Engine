# Query Makefile
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 
CFILES = ./src/query.c ./src/qweb.c ./src/file.c ./src/qhashtable.c

UTILDIR=../util/
UTILFLAG = -ltseutil
UTILLIB = $(UTILDIR)libtseutil.a

# my project details
query: $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)

$(UTILLIB):
	cd $(UTILDIR); make;
		
clean:
	rm -f *~
	rm -f *#
	rm -f ./query
	rm -f *.o
	cd $(UTILDIR); make clean;
