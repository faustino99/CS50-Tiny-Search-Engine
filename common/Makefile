# Makefile for common library
#   Builds and compiles all these utility functions into a library.
#
# Faustino Cortina, April 2018
#

L = ../libcs50

# object files, and the target library
OBJS = word.o pagedir.o index.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(L)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
word.o: word.h
pagedir.o: $(L)/counters.h $(L)/webpage.h $(L)/memory.h $(L)/hashtable.h $(L)/file.h pagedir.h word.h
index.o: index.h $(L)/hashtable.h $(L)/counters.h $(L)/webpage.h $(L)/file.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o
