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


/**************** loadIndex ***********************************/
/* Makes a new index struct. Extracts data from an input file and inserts it into the new index
 * Calls index_save to write the new index to an output file. 
*/
static void loadIndex(FILE* input, FILE* output);