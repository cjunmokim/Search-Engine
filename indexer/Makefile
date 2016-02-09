# indexer Makefile
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 
UTILDIR=./src/
UTILC=$(UTILDIR)indexer.c $(UTILDIR)iweb.c $(UTILDIR)file.c $(UTILDIR)ihashtable.c
UTILH=$(UTILDIR)iweb.h $(UTILDIR)file.h $(UTILDIR)ihashtable.h

# my project details
EXEC = indexer
OBJS = indexer.o iweb.o file.o ihashtable.o
SRCS = $(UTILC) $(UTILH)


$(EXEC):	$(OBJS) $(UTILLIB)
		$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJS):	$(SRCS)
		$(CC) $(CFLAGS) -c $(SRCS)

debug:	$(SRCS)
		$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
		$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)

clean:
		rm -f *~
		rm -f *#
		rm -f ./indexer
		rm -f *.o
		rm -f $(UTILDIR)*.gch