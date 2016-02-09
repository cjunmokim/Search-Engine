# crawler Makefile
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 
UTILDIR=./crawler/
UTILFLAG=-lcurl
UTILC=$(UTILDIR)crawler.c $(UTILDIR)web.c $(UTILDIR)list.c $(UTILDIR)hashtable.c
UTILH=$(UTILDIR)web.h $(UTILDIR)list.h $(UTILDIR)hashtable.h

# my project details
EXEC = Crawler
OBJS = crawler.o web.o list.o hashtable.o
SRCS = $(UTILC) $(UTILH)


$(EXEC):	$(OBJS) $(UTILLIB)
		$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(UTILFLAG)

$(OBJS):	$(SRCS)
		$(CC) $(CFLAGS) -c $(SRCS)

debug:	$(SRCS)
		$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
		$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) $(UTILFLAG)

clean:
		rm -f *~
		rm -f *#
		rm -f ./crawler
		rm -f *.o
		rm -f $(UTILDIR)*.gch


# indexer Makefile
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 
UTILDIR=./indexer/src/
UTILC=$(UTILDIR)indexer.c $(UTILDIR)web.c $(UTILDIR)file.c $(UTILDIR)hashtable.c
UTILH=$(UTILDIR)web.h $(UTILDIR)file.h $(UTILDIR)hashtable.h

# my project details
EXEC = Indexer
OBJS = indexer.o web.o file.o hashtable.o
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