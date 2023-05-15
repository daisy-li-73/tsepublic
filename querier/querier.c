/*
 * Answers caller's search queries as described in the specs.
 *
 * Daisy Li, April 2023, COSC 50
*/

#include <stdio.h>
#include "../common/index.h"
#include <dirent.h>
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../common/word.h"
#include <string.h>
#include "../common/pagedir.h"
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h> 

#define max_query 1000
#define max_tokens 100
#define MIN(a, b) ((a) < (b) ? (a) : (b))

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

/* **************************************** */
int main(const int argc, char *argv[]) {
  char* pageDirectory;
  char* indexFilename;
  char* query = mem_malloc(max_query);
  int numTokens;
  counters_t* answer;
  char* words[max_query];

  parseArgs(argc, argv, &pageDirectory, &indexFilename);
  FILE* input = fopen(indexFilename, "r");
  index_t* index = loadIndex(input);

  // get initial query
  prompt();
    // get user input from stdin
  query = fgets(query, max_query, stdin);
  
  while (query != NULL) {
    // parse and clean query
    numTokens = tokenize(query, words);
    // check validity of query
    if (numTokens > 0) {
      if (validateQuery(words, numTokens)) {
      printf("Query:");
      for (int i = 0; i < numTokens; i++) {
        printf(" %s", words[i]);
      }
      printf("\n");
    // search for matches
      answer = lookup(index, words, numTokens);
    // print out ranked results
      rankResults(answer, pageDirectory);
    // reset
      counters_delete(answer);
      } 
    }
    prompt();
    // get user input from stdin
    query = fgets(query, max_query, stdin);
    printf("%s", query);
  } 
  // clean up
  free(query);
  index_delete(index);
}
/**************** parseArgs ***********************************/
/* Given arguments from the command line, extract them into the function parameters;
 * return only if successful. */
static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename) {
  if (argc == 3) {
    *pageDirectory = argv[1];
    *indexFilename = argv[2];

    // for pageDirectory, check that it exists and contains a .crawler file
    DIR* dir = opendir(*pageDirectory);
    if (dir != NULL) {
      if (pagedir_validate(*pageDirectory) == false) {
        fprintf(stderr, "Error: %s is not a crawler-produced directory \n", *pageDirectory);
        exit(2);
      }
    mem_free(dir);
    }
    else {
      fprintf(stderr, "Error: %s does not exist \n", *pageDirectory);
      mem_free(dir);
      exit(2);
    }

  // for indexFilename, check if it exists and is read-able
    FILE* indexFile = fopen(*indexFilename, "r");
    if (indexFile != NULL) {
      fclose(indexFile);
    }
    else {
      fprintf(stderr, "Error: cannot read %s \n", *indexFilename);
      exit(2);
    }
  }
  else {
    fprintf(stderr, "usage: ./%s pageDirectory indexFilename \n", argv[0]);
    exit(1);
  }
}

/**************** loadIndex ***********************************/
/* Create a new index struct and fill it using data from the indexFilename file;
 * return the index. */
index_t* loadIndex(FILE* input) {
  int size = file_numLines(input);
  index_t* newIndex = index_new(size);
  if (newIndex != NULL) {
    char* word;
  // Extract data from index file into index struct
  while ((word = file_readWord(input)) != NULL) {
    counters_t* ctr = counters_new();
    int docID, count;

    if (ctr != NULL) {
        //Extract (docID, count) pairs
        while(fscanf(input, "%d %d", &docID, &count) == 2) {
          counters_set(ctr, docID, count);
        }
        index_insert(newIndex, word, ctr);
        mem_free(word);
      }
   }
  }
  return newIndex;
}

/**************** tokenize ***********************************/
/* Clean the query according to the syntax described in the specs.
 * Return the cleaned query. Return null if the query syntax is invalid.
 */
int tokenize(char* query, char* words[]) {
  int numTokens = 0;
  //two sliding pointers starting at beginning of query
  char* word = query;
  char* rest = query;

while (*word != '\0') {
  // check if query contains any invalid characters
    if (!(isalpha(*word))&&!(isspace(*word))) {
      fprintf(stderr, "Error: bad character '%c' in query \n", *word);
      return -1;
    }
  // slide rest pointer to start of word
    if (isalpha(*word)) {
      rest = word;
  // slide rest pointer to end of word
      while (isalpha(*rest)) {
        rest++;
      }
  // if bad character
      if (!(isspace(*rest))) {
        fprintf(stderr, "Error: bad character '%c' in query \n", *rest);
        return -1;
      }
  // squash rest pointer with null terminator
      *rest = '\0';
      numTokens++;
      normalizeWord(word);
  // add word to words[]
      words[numTokens-1] = word;
  // reset word pointer to start of next word
      word = rest+1;
    }
    else {
      word++;
      if (*word == '\0') {
        return numTokens;
      }
    }
  }
  return numTokens;
}
/**************** validateQuery ***********************************/
/* Validate query according to requirements specs. Print any applicaple error messages.
 */
bool validateQuery(char* words[], int numTokens) {
  if (strcmp(words[0], "or") == 0) {
    fprintf(stderr, "Error: 'or' cannot be first \n");
    return false;
  }
  if (strcmp(words[0], "and") == 0) {
    fprintf(stderr, "Error: 'and' cannot be first \n");
    return false;
  }
  if (strcmp(words[numTokens-1], "or") == 0) {
    fprintf(stderr, "Error: 'or' cannot be last \n");
    return false;
  }
  if (strcmp(words[numTokens-1], "and") == 0) {
    fprintf(stderr, "Error: 'and' cannot be last \n");
    return false;
  }
  for (int i = 0; i < numTokens-1; i++) {
    if ((strcmp(words[i], "or") == 0) && (strcmp(words[i+1], "or") == 0)) {
      fprintf(stderr, "Error: 'or' and 'or' cannot be adjacent \n");
      return false;
    }
    if ((strcmp(words[i], "or") == 0) && (strcmp(words[i+1], "and") == 0)) {
      fprintf(stderr, "Error: 'or' and 'and' cannot be adjacent \n");
      return false;
    }
    if ((strcmp(words[i], "and") == 0) && (strcmp(words[i+1], "or") == 0)) {
      fprintf(stderr, "Error: 'and' and 'or' cannot be adjacent \n");
      return false;
    }
    if ((strcmp(words[i], "and") == 0) && (strcmp(words[i+1], "and") == 0)) {
      fprintf(stderr, "Error: 'and' and 'and' cannot be adjacent \n");
      return false;
    }
  }
  return true;
}


/**************** lookup ***********************************/
/* Search index for documents containing query words. Return a counter set of 
 * valid documents.
 */
 void* lookup(index_t* index, char* words[], int numTokens) {
   // cumulative counterset of docs which match query
   counters_t* result = counters_new();
   if (result != NULL) {
    counters_t* temp = index_find(index, words[0]);

   for (int i = 1; i <= numTokens; i++) {
    // if at end of query, merge temp into result
    if (i == numTokens) {
      counters_merge(result, temp);
      //counters_delete(temp);
      return result;
    }
    else if (strcmp(words[i], "and") == 0) {
      continue;
    }
    else if (strcmp(words[i], "or") == 0) {
   //merge temp into result
        counters_merge(result, temp);
   //jump temp ahead to next (non-literal) word
        temp = index_find(index, words[i+1]);
     }
    else {
    // find intersection of curr and temp
      counters_t* curr = index_find(index, words[i]);
      counters_intersect(temp, curr);
    }
    }
  }
  return result;
}

/* Print out a ranked list of matching documents, from high to low scores. 
*/

static void
rankResults(counters_t* result, char* pageDirectory) {
  int numItems = 0;
  counters_iterate(result, &numItems, itemcount);
  if (numItems == 0) {
    printf("No documents match. \n");
    return;
  }

  int maxScore;
  int maxID;

  do {
    maxScore = 0;
    maxID = 0;
    int maxDoc[] = {maxID, maxScore};
    counters_iterate(result, maxDoc, findMax);
    maxID = maxDoc[0];
    maxScore = maxDoc[1];
    if (maxScore > 0) {
    // print doc info
    char* docname = pagedir_save(pageDirectory, maxID);
    char* url;
    FILE* fp = fopen(docname, "r");
    if (fp != NULL) {
      url = file_readLine(fp);
      fclose(fp);
    }
    printf("score   %d doc  %d: %s\n", maxScore, maxID, url);
    free(docname);
    free(url);
    counters_set(result, maxID, 0);
    // reset
    maxDoc[0] = 0;
    maxDoc[1] = 0;
    }

  } while(maxScore > 0);
}


/* count the non-zero items in the counter set.
 */
static void itemcount(void* arg, const int key, const int count)
{
  int* nitems = arg;
  if (count != 0) {
    (*nitems)++;
  }
}

/* Find the intersection of the two counter sets..
 */
static void
counters_intersect(counters_t* ctrA, counters_t* ctrB)
{
  struct twosets { counters_t *ctr1, *ctr2; } ;
  struct twosets ctrs = {ctrA, ctrB};
  counters_iterate(ctrA, &ctrs, counters_inter_helper);
}

/* Consider one item in ctrB for addition to ctrA. If it exists, set key in ctrA to min of count in ctrA and ctrB.
 * If it doesn't exist in ctrB, drop item from ctrA by setting key in ctr to count = 0. 
 */
static void
counters_inter_helper(void* arg, const int key, const int item)
{
  struct twosets { counters_t *ctr1, *ctr2; } ;
  struct twosets* ctrs = arg;
  counters_t* ctrA = ctrs->ctr1;
  counters_t* ctrB = ctrs->ctr2;

  int countB = counters_get(ctrB, key);
  int countA = item;
  if (countB == 0) {
    counters_set(ctrA, key, 0);
  }
  else {
    counters_set(ctrA, key, MIN(countA, countB));
  }
}

/* Merge the second counter set into the first counters set;
 * the second counters set is unchanged.
 */
static void
counters_merge(counters_t* ctrA, counters_t* ctrB)
{
  counters_iterate(ctrB, ctrA, counters_merge_helper);
}

/* Consider one item for insertion into the other set.
 */
static void
counters_merge_helper(void* arg, const int key, const int item)
{
  counters_t* ctrA = arg;
  int docID = key;
  int countB = item;
  int countA = counters_get(ctrA, docID);
  counters_set(ctrA, docID, countA+countB);
}

/* helper function for printing out ranked results. Finds max score, prints score, docID, url 
 * as specified in specs. Update maxScore and sets count of docID to 0.  */

void findMax(void* arg, const int docID, const int score) {
  int* maxDoc = arg;
  int maxScore = maxDoc[1];
  if (score >= maxScore) {
    maxDoc[0] = docID;
    maxDoc[1] = score;
  }
}

/* The fileno() function is provided by stdio,
 * but for some reason not declared by stdio.h, so declare here.
 */
int fileno(FILE *stream);

static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}


