#!/bin/bash
# test command-line argument parsing 

# Various invalid arguments for indexer ---
# No arguments
./indexer 
echo $?

# One argument
./indexer ../data/crawldata/letters3
echo $?

# Three arguments 
./indexer ../data/crawldata/letters3 ../data/indices/newIndex hi
echo $?

# Invalid pageDirectory (non-existent path)
./indexer crawldata ../data/indices/newIndex 
echo $?

# Invalid pageDirectory (not a crawler directory)
./indexer ../data/indices ../data/indices/newIndex
echo $?

# Invalid indexFile (non-existent path)
./indexer ../data/crawldata/letters3 data/newIndex
echo $?

# Invalid indexFile (read-only directory)
./indexer ../data/crawldata/letters3 ../data/indices/testDir
echo $?

# Invalid indexFile (existing, read-only directory)
./indexer ../data/crawldata/letters3 ../data/indices/readOnly
echo $?

# Valid arguments: pageDirectory = letters3
./indexer ../data/crawldata/letters3 ../data/indices/letters3.ndx


# Valid arguments: pageDirectory = letters0
./indexer ../data/crawldata/letters0 ../data/indices/letters0.ndx

# Invalid arguments for indextest (zero arguments)
./indextest

# Valid arguments for indextest: oldIndexFile = letters3
./indextest  ../data/indices/letters3.ndx ../data/indices/letters3.ndx2
