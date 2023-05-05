/* 
 * Companion module for indexer. Reads input from an index file, constructs an index_t, 
 * and outputs to a new index file.
 * 
 * Daisy Li, COSC 50, April 2023
*/

#include <stdio.h>
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include <dirent.h>
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../common/word.h"
#include <string.h>

static void loadIndex(FILE* input, FILE* output);

/* **************************************** */
int main(const int argc, char *argv[]) {
  char* oldFilename;
  char* newFilename;
  if (argc != 3) {
    fprintf(stderr, "usage: %s oldFilename newFilename \n", argv[0]);
    exit(1);
  }
  oldFilename = argv[1];
  newFilename = argv[2];

  FILE* oldIndexFile = fopen(oldFilename, "r");
  FILE* newIndexFile = fopen(newFilename, "w");
  loadIndex(oldIndexFile, newIndexFile);

  return 0;
} 

/**************** loadIndex ***********************************/
/* see indextest.h for description */

static void loadIndex(FILE* input, FILE* output) {
  int size = file_numLines(input);
  index_t* newIndex = index_new(size);
  char* word;
  // Extract data from index file into index struct
  while ((word = file_readWord(input)) != NULL) {
    counters_t* ctr = counters_new();
    int docID, count;

    //Extract (docID, count) pairs 
    while(fscanf(input, "%d %d", &docID, &count) == 2) {
      counters_set(ctr, docID, count);
    }
    index_insert(newIndex, word, ctr);
    mem_free(word);

  }

  // write index to newFilename
  index_save(newIndex, output);
  index_delete(newIndex);
}