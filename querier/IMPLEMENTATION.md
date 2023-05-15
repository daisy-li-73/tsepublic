# CS50 TSE querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/CS50spring2023/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the querier.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use three data structures: an index created from the data in indexFile, an array of words from the query, and a counter set that holds matching documents found by the querier.

The size of the index is equal to the number of lines in indexFIle. The size of words is impossible to know in advance, so we assume 100.

## Control flow

The querier is implemented in one file `querier.c`, with four functions.

### main

The `main` function calls `parseArgs`, then enters a while loop to take in query input. 
For each query, it calls `tokenize`, `validateQuery`, `lookup`, and `rankResults`.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for pageDirectory, check that it exists and contains a .crawler file
* for indexFilename, check if it exists and is read-able

### tokenize

Break the query into null-terminated words. Print error msg if find any invalid characters.
Pseudocode:
    
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
### validateQuery

Check that input is valid according to syntax described in specs 
### lookup

Do the real work of looking up words in the index to answer the user's query
Pseudocode:
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

### rankResults
			  checks if result is empty
        if yes, print appropriate message and return
      iterate through result
        find item with maxscore
        print that item as specified in requirements 
        set that item's count to 0

## Other modules

### pagedir

We create a re-usable module `pagedir.c` to handles the *pagesaver*  mentioned in the design (writing a page to the pageDirectory), and marking it as a querier-produced pageDirectory (as required in the spec).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.

Pseudocode for `pagedir_init`:

	construct the pathname for the .querier file in that directory
	open the file for writing; on error, return false.
	close the file and return true.


Pseudocode for `pagedir_save`:

	construct the pathname for the page file in pageDirectory
	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file

### libcs50

We leverage the modules of libcs50, most notably `bag`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.
Indeed, `webpage_fetch` enforces the 1-second delay for each fetch, so our querier need not implement that part of the spec.

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
static void prompt();
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
index_t* loadIndex(FILE* indexFilename);
int tokenize(char* query, char* words[]);
bool validateQuery(char* words[], int numTokens);
void* lookup(index_t* index, char* words[], int numTokens);
void findMax(void* arg, const int docID, const int score);
static void counters_merge(counters_t* ctrA, counters_t* ctrB);
static void counters_merge_helper(void* arg, const int key, const int item);
static void counters_intersect(counters_t* ctrA, counters_t* ctrB);
static void counters_inter_helper(void* arg, const int key, const int item);
static void rankResults(counters_t* result, char* pageDirectory);
static void itemcount(void* arg, const int key, const int count);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the `.querier` file, allowing the querier to decide what to do; the `webpage` module returns false when URLs are not retrievable, and the querier does not treat that as a fatal error.

## Testing plan

Here is an implementation-specific testing plan.

### Integration/system testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a sequence of bad query syntaxes.
Third, random queries generated by fuzzquery.
Run that script with `bash -v testing.sh` so the output of querier is intermixed with the commands used to invoke the querier.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.