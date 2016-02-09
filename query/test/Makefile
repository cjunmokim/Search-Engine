# Query Testing Makefile
all: query queryengine_test

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 
CFILES = ./queryengine_test.c ../src/query_func.c ../src/file.c ../src/qhashtable.c ../src/qweb.c

UTILDIR=../../util/
UTILFLAG = -ltseutil


# my project details
query:
	cd ..; make query; cd ./test/;

queryengine_test:
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)


		
clean:
	cd ..; make clean;
	rm -f *~
	rm -f *#
	rm -f ./queryengine_test
	rm -f *.o
