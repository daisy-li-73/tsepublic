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

/***************** global types ****************/
typedef struct index index_t;  // opaque to users of the module

/**************** functions ****************/
/**************** hashtable_new ****************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
*/
index_t* index_new(const int numSlots);

/**************** index_insert ****************/
/* Insert item, where the key is a word, into the index. 
 * The item is a counterset of (docID, count) pairs.
 * See hashtable.h for more details
*/
bool index_insert(index_t* idx, const char* key, void* item);

/**************** index_find ****************/
/* Return the item associated with the given key.
 * See hashtable.h for more details 
*/
void* index_find(index_t* idx, const char* key);

/**************** countsave ***********************************/
/* Helper function for counters_iterate. 
 * Formats a single (key, item) pair in a counterset to the indexFile
 */
void
countsave(void* indexFile, int docID, int count);

/**************** wordsave ***********************************/
/* Helper function for hashtable_iterate. 
 */
void wordsave(void* indexFile, const char* key, void* item);

/**************** index_save ****************/
/* Writes the given index to a file given as arg. Uses hashtable_iterate,
 * counters_iterate, and two helper methods. 
*/
void 
index_save(index_t* idx, void* arg);

/**************** index_delete ****************/
/* Delete index, calling a delete function on each item in its hashtable.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call hashtable_delete with itemfunc as a parameter.
 */
void index_delete(index_t* idx);