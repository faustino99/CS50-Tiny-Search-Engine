# Makefile for the "crawler" module
#
# Faustino Cortina - April 2018

L = ../libcs50
L2 = ../common

CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L -I$(L2)

PROG = indexer
OBJS = indexer.o
LIBS = -lm
LLIBS = $(L2)/common.a $L/libcs50.a

PROG2 = indextest
OBJS2 = indextest.o


all: $(PROG) $(PROG2)

# executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS2) $(LLIBS) -o $(PROG2)

# object files depend on include files
indexer.o: $L/hashtable.h $L/counters.h $(L2)/pagedir.h $(L2)/index.h
indextest.o: $L/hashtable.h $L/counters.h $(L2)/index.h $L/file.h

.PHONY: all test clean

test:
	./indextest.sh

clean:
	rm -f $(PROG) $(PROG2)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f *.sorted
	rm -f file2
	rm -f file
