# Makefile for the "querier" module
#
# Faustino Cortina - May 2018

L = ../libcs50
L2 = ../common

CC=gcc
CFLAGS=-Wall -pedantic -std=gnu11 -ggdb -I$L -I$(L2)

PROG = querier
OBJS = querier.o
LIBS = -lm
LLIBS = $(L2)/common.a $L/libcs50.a
LLIBS2 = $L/libcs50.a

PROG2 = fuzzquery
OBJS2 = fuzzquery.o


all: $(PROG) $(PROG2)

# executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

$(PROG2): $(OBJS2) $(LLIBS2)
	$(CC) $(CFLAGS) $(OBJS2) $(LLIBS2) -o $(PROG2)

# object files depend on include files
querier.o: $L/file.h $L/hashtable.h $L/counters.h $(L2)/pagedir.h
fuzzquery.o: $L/file.h $L/memory.h

.PHONY: all test clean

test:
	./queriertest.sh

clean:
	rm -f $(PROG) $(PROG2)
	rm -f *~ *.o
	rm -rf *.dSYM
