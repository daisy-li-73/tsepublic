# CS50 TSE Indexer
## Implementation Spec

## Data structures 

I created a new 'index' data structure. It is a hashtable where the key is a word and the item is a counter set. The key in the counter set is the docID. 
The size of the index (total number of words) is impossible to determine in advance, so I use 900.

## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### main

The `main` function calls `parseArgs` and initializes the pageDirectory and indexFile. It then calls `indexBuild` and `index_save`. Finally, it cleans up by calling `index_delete`.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, validate it is a valid path and that it is a crawler-produced directory
* for `indexFilename`, check that is a valid path and that it is write-able
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Do the real work of building an index from the files in `pageDirectory`.
Pseudocode:

	creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage

### indexPage

Given a `webpage`, scan the given page to extract and normalize words; if the word has not been seen before, insert it into the index with the appropriate docID. If the word appears multiple times in the same webpage, increment the count. 

Pseudocode:

steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID


## Other modules

### pagedir

pagedir has been extended to include pagedir_save and pagedir_write. pagedir_save constructs and returns the path name for the page file in pageDirectory. pagedir_write prints the URL, depth, and contents of the webpage as specified in the crawler specs.

Pseudocode for `pagedir_save`:

  convert the int docID to a string
  concatenate the pageDirectory and id together to make a pathname
  return pathname 

Pseudocode for `pagedir_write`:

	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file

### libcs50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `indexer.h` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
void indexBuild(char* pageDirectory);
void indexPage(index_t* index, webpage_t* page, int docID);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
char* pagedir_save(const char* pageDirectory, const int docID);
void pagedir_write(const webpage_t* page, const char* pathname);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code uses defensive-programming tactics to catch and exit, e.g., if a function receives bad parameters.

## Testing plan

Here is an implementation-specific testing plan.

### Testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case (such as `letters` at depth 3).
Third, run the same test case with indextest to verify that the index file can be loaded. 

Run that script with `bash -v testing.sh` so the output of indexer is intermixed with the commands used to invoke the indexer.
