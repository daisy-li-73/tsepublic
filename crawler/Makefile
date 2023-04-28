# Makefile for 'crawler' module
#
# Daisy Li, April 2023

OBJS = ../libcs50/bag.o ../libcs50/hashtable.o ../libcs50/hash.o ../common/pagedir.o ../libcs50/mem.o
LIBS =

# uncomment the following to turn on output logging
TESTING=-DAPPTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
C = ../common
L = ../libcs50
LLIBS = $C/common.a $L/libcs50.a

CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: crawler

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: crawler 
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ pgdir 1

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core
	rm -f ./pgdir/[0-9]*