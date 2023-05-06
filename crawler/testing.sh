# test command-line argument parsing

# invalid number of arguments
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ pgdir 1 2
# ./crawler
# ./crawler 1

# maxDepth out of range
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ pgdir 20

# pageDirectory does not exist 
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ hello 2

# seedURL is invalid 
# ./crawler https://www.amazon.com/ pgdir 2

# valid arguments; letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawldata/letters3 3

# valid arguments; letters
#./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ pgdir 10

# valid arguments; letters
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ 1

# valid arguments; toScrape 
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ pgdir 2