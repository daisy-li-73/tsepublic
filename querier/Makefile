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

all: querier fuzzquery 

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: querier 
	$(VALGRIND) ./querier ../data/crawldata/letters3 ../data/indices/letters3.ndx

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f testing.out
	rm -f querier
	rm -f fuzzquery
