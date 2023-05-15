#!/bin/bash
# test command-line argument parsing 

# Various invalid arguments for indexer ---
# No arguments
# ./querier

# # 1 argument
# ./querier ../data/crawldata/letters3

# # 3 arguments
# ./querier ../data/crawldata/letters3 ../data/indices/letters3.ndx hi

# # Invalid pageDirectory (path does not exist)
# ./querier ../data/letters3 ../data/indices/letters3.ndx hi

# # Invalid pageDirectory (not a crawler-produced directory)
# ./querier ../data/indices/testDir ../data/indices/letters3.ndx hi

# # Invalid indexFile (non-existent path)
# ./querier ../data/crawldata/letters3 data/newIndex

# # Invalid indexFile (read-only directory)
# ./querier ../data/crawldata/letters3 ../data/indices/testDir

# # Invalid indexFile (existing, read-only file)
# ./querier ../data/crawldata/letters3 ../data/indices/readOnly

# Valid command-line arguments
# Various invalid queries ---
 ./querier ../data/crawldata/letters3 ../data/indices/letters3.ndx < badQuery.txt

# Fuzzquery 
# pdir="../data/crawldata/toscrape2"
# indx="../data/indices/toscrape2.ndx"
# ./fuzzquery $indx 10 0 | ./querier $pdir $indx