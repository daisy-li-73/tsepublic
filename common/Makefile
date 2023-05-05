# Makefile for common library
#   Builds and compiles all these utility functions into a library.
#
# Daisy Li, April 2023

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50.a
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
index.o: index.h

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o