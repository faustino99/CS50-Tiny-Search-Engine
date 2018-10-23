# Makefile for the "crawler" module
#
# Faustino Cortina - April 2018

L = ../libcs50
L2 = ../common

CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L -I$(L2)
PROG = crawler
OBJS = crawler.o
LIBS = -lm
LLIBS = $(L2)/common.a $(L)/libcs50.a

.PHONY: all test clean

all: $(PROG)

# executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LLIBS) -o $(PROG)

# object files depend on include files
crawler.o: $L/bag.h $L/hashtable.h $L/webpage.h $L/memory.h $L/counters.h $(L2)/pagedir.h

test:
	./crawlertest.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
