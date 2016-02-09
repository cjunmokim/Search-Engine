# Filename: Makefile
# Description: The make file is to build the static library for TSE.

CC=gcc
CFLAGS= -Wall -pedantic -std=c11

UTILDIR=../crawler/src/
UTILDIR2=../indexer/src/
UTILDIR3=../query/src/
UTILLIB=./libtseutil.a

UTILC=$(UTILDIR)cweb.c $(UTILDIR)list.c $(UTILDIR)chashtable.c
UTILH=$(UTILC:.c=.h)
UTILC2=$(UTILDIR2)iweb.c $(UTILDIR2)file.c $(UTILDIR2)ihashtable.c
UTILH2=$(UTILC2:.c=.h)
UTILC3 =$(UTILDIR3)qweb.c $(UTILDIR3)qhashtable.c
UTILH3=$(UTILC3:.c=.h)


OBJS = cweb.o list.o chashtable.o
SRCS = $(UTILC) $(UTILH)
OBJS2 = iweb.o file.o ihashtable.o
SRCS2 = $(UTILC2) $(UTILH2)
OBJS3 = qweb.o qhashtable.o
SRCS3 = $(UTILC3) $(UTILH3)

$(UTILLIB):	$(OBJS) $(OBJS2) $(OBJS3)
		ar -rv $(UTILLIB) $(OBJS) $(OBJS2) $(OBJS3)

$(OBJS) $(OBJS2) $(OBJS3):	$(SRCS) $(SRCS2) $(SRCS3)
		$(CC) $(CFLAGS) -c $(SRCS) $(SRCS2) $(SRCS3)
		
clean:
		rm -f *~
		rm -f *#
		rm -f *.o
		rm -f $(UTILDIR)*.gch
		rm -f $(UTILDIR2)*.gch
		rm -f $(UTILDIR3)*.gch
		rm -f $(UTILLIB)