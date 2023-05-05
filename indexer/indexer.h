/*
 * Builds inverted-index data structure according to the specs
 * described in Lab 5.
 * 
 * Daisy Li, April 2023, COSc 50
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

/**************** parseArgs ***********************************/
/* Given arguments from the command line, extract them into the function parameters;
 * return only if successful. */

static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);

/**************** indexBuild ***********************************/
/* Given a pageDirectory, creates a new index struct based on the data files in that directory.
*/
void 
indexBuild(char* pageDirectory);

/**************** indexPage ***********************************/
/* Loops through a given webpage and adds words into the index if needed.
*/
void
indexPage(index_t* index, webpage_t* page, int docID);


