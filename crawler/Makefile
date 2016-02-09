# crawler Makefile
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 
UTILDIR=./src/
UTILFLAG=-lcurl
UTILC=$(UTILDIR)crawler.c $(UTILDIR)cweb.c $(UTILDIR)list.c $(UTILDIR)chashtable.c
UTILH=$(UTILDIR)cweb.h $(UTILDIR)list.h $(UTILDIR)chashtable.h

# my project details
EXEC = crawler
OBJS = crawler.o cweb.o list.o chashtable.o
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
