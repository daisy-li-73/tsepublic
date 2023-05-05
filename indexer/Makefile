# Makefile for 'crawler' module
#
# Daisy Li, April 2023

OBJS = ../libcs50/hashtable.o ../libcs50/hash.o ../common/pagedir.o ../common/index.o ../common/word.o ../libcs50/mem.o
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

all: indexer indextest

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: indexer 
	$(VALGRIND) ./indextest  ../data/indices/letters3.ndx ../data/indices/letters3.ndx2

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core
	rm -f testing.out
	rm -f indexer
	rm -f indextest