# CS50 TSE querier
## Design Spec

According to the [querier Requirements Spec](REQUIREMENTS.md), the TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.


### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

```
querier pageDirectory indexFilename
```

For example, if `letters` is a pageDirectory in `../data`,

``` bash
$ querier ../data/letters ../data/letters.index
```

### Inputs and outputs

**Input**: the querier reads files from a directory by constructing file pathnames from the `pageDirectory` parameter followed by a numeric document ID (as described in the Requirements).

The querier reads data from the indexFIile, then loads the data into a new index using the indextest function from indexer.

**Output**: Querier searches through the index for documents matching the query. It saves matching documents in a counter set. It then prints out the documents from high to low score.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which initializes other modules;
 2. *parseArgs*, which validates arguments;
 3. *loadIndex*, which builds an in-memory index from webpage files it finds in the pageDirectory;
 4. *tokenize*, which parses and validates the query syntax.
 5. *lookUp*, which adds matching documents to a counter set
 6. *countersMerge*, which implements the AND literal functionality
 7. *countersIntersect*, which implements the OR literal functionality
 8. *rankResults*, which ranks the results from high to low score


And some helper modules that provide data structures:

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
 1. *webpage*, a module providing the data structure to represent webpages, and to scan a webpage for words;
 2. *pagedir*, a module providing functions to load webpages from files in the pageDirectory;
 4. *word*, a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    call loadIndex, with indexFile
    call tokenize, with query input from user 
    call lookup, with output from tokenize
    call rankResults, with output from lookup

where *loadIndex:*

      creates a new 'index' object
      loops through indexFile
        adds (docID, count) pairs to index
      returns new index

where *tokenize:*

      initializes two pointers to the start of query
      while first pointer is not null
      slides first pointer to first letter in query
        slides second pointer to meet first pointer
        slides second pointer to first space
        squashes second pointer with null terminator 
        normalizes word betwen first and second pointer
        adds word to array
        increments number of words
      returns number of words

      *if finds an invalid character at any point, print error msg and return -1

where *lookup*:
      creates a new result and temp counter
      iterates through words array
      if reach end of array
        merge temp and result
      if find "and"
        intersect temp and result
      if find "or"
        merge temp and result
        clear temp
      return result

where *rankResults*
      checks if result is empty
        if yes, print appropriate message and return
      iterate through result
        find item with maxscore
        print that item as specified in requirements 
        set that item's count to 0

### Major data structures

*index* is a new index created from the data in indexFile
*words* is a char array that stores the words found in query
*result* is a counter set that stores the documents that match the words in query

### Testing plan

*Unit testing*. tokenize will be tested with various invalid input

*Integration testing*.  The *querier*, as a complete program, will be tested by building an index from a indexFile, and then various queries will be tested. They will be compared against known good output 

1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	5. invalid `pageDirectory` (not a crawler directory)
	6. invalid `indexFile` (non-existent path)
	7. invalid `indexFile` (read-only directory)
	7. invalid `indexFile` (existing, read-only file)
  8. various invalid query syntaxes 
2. Use fuzztest to generate various valid queries 
3. Run *valgrind* on *querier* to ensure no memory leaks or errors.