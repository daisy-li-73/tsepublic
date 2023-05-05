/*
 * A helper module that provides the data structure of an index, implemented
 * as a hashtable of counter sets. Also includes functions to read and write
 * index files
 * 
 * Daisy Li, April 2023
*/

#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** global types ****************/
typedef struct index { //array of pointers to sets
  hashtable_t* ht;
} index_t;

/**************** functions ****************/
/**************** index_new ****************/
/* see index.h for description */
index_t* 
index_new(const int numSlots) {
  index_t* idx = mem_malloc(sizeof(index_t));
  // defensive programming
  if (idx == NULL) {
    mem_free(idx);
    return NULL;
  }
    idx->ht = hashtable_new(numSlots);
  return idx;
}

/**************** index_insert ****************/
/* see index.h for description */

bool
index_insert(index_t* idx, const char* key, void* item) {
  return hashtable_insert(idx->ht, key, item);
}

/**************** index_find ****************/
/* see index.h for description */

void*
index_find(index_t* idx, const char* key) {
  return hashtable_find(idx->ht, key);
}
/**************** countsave ***********************************/
/* Helper function for counters_iterate. 
 * Formats a single (key, item) pair in a counterset to the indexFile
 */
void
countsave(void* indexFile, int docID, int count) {
  fprintf(indexFile, "%d %d ", docID, count);
}

/**************** wordsave ***********************************/
/* Helper function for hashtable_iterate. 
 */
void wordsave(void* indexFile, const char* key, void* item) {
  fprintf(indexFile, "%s ", key);
  counters_iterate(item, indexFile, countsave);
  fprintf(indexFile, "\n");
}

/**************** index_save ****************/
/* see index.h for description */

void 
index_save(index_t* idx, void* arg) {
  hashtable_iterate(idx->ht, arg, wordsave);
}

/**************** counterdel ***********************************/
/* Helper function for index_delete. Deletes an item (counter set) if not null */
void
counterdel(void* item) {
  if (item != NULL) {
    counters_delete(item);
  }
}

/**************** index_delete ****************/
/* see index.h for description */
void 
index_delete(index_t* idx) {
  if (idx != NULL) {
    hashtable_delete(idx->ht, counterdel);
  }
mem_free(idx);
}
