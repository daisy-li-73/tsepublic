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
#include "../common/pagedir.h"

static void 
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
index_t* indexBuild(char* pageDirectory);
void indexPage(index_t* index, webpage_t* page, int docID);

/* **************************************** */
int main(const int argc, char *argv[]) {
  char* pageDirectory;
  char* indexFilename;

  parseArgs(argc, argv, &pageDirectory, &indexFilename);
  index_t* index = indexBuild(pageDirectory);
  FILE* indexFile = fopen(indexFilename, "w");
  index_save(index, indexFile);
  index_delete(index);

  exit(0);
}

/**************** parseArgs ***********************************/
/* Given arguments from the command line, extract them into the function parameters;
 * return only if successful. */

static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename) {
  //check number of arguments
  if (argc == 3) {
    *pageDirectory = argv[1];
    *indexFilename = argv[2];

  // for pageDirectory, check that it exists and contains a .crawler file
    DIR* dir = opendir(*pageDirectory);
    if (dir != NULL) {
    //construct .crawler file pathname
      char* crawler = ".crawler";
      char filename[strlen(*pageDirectory) + strlen(crawler)+2];
      sprintf(filename, "%s/%s", *pageDirectory, crawler);

    //check if .crawler file exists in pageDirectory
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
      fprintf(stderr, "Error: %s is not a crawler produced directory \n", *pageDirectory);
      mem_free(dir);
      mem_free(fp);
      exit(2);
    }
    mem_free(dir);
    fclose(fp);
    }
    else {
      fprintf(stderr, "Error: %s does not exist \n", *pageDirectory);
      mem_free(dir);
      exit(2);
    }

  // for indexFilename, check if it exists and is write-able
    FILE* indexFile = fopen(*indexFilename, "w");
    if (indexFile != NULL) {
      fclose(indexFile);
      }
    else {
      fprintf(stderr, "Error: cannot write to %s \n", *indexFilename);
      exit(2);
      }
    }
  else {
    fprintf(stderr, "usage: %s pageDirectory indexFilename \n", argv[0]);
    exit(1);
  }
}

/**************** indexBuild ***********************************/
/* see indexer.h for description */

index_t* 
indexBuild(char* pageDirectory) {
  //create a new 'index' struct
  int size = 900;
  index_t* index = index_new(size);

  //loop through document ID numbers, counting from 1
  int docID = 1;
  while (docID != 0) {
  //construct pathname of docID
    char* pathname = pagedir_save(pageDirectory, docID);
  //check if file exists 
    FILE* fp = fopen(pathname, "r");
    if (fp != NULL) {
  // create a new webpage from information in file 
      char* url = file_readLine(fp);
      int depth = 0; // TODO: get depth from second line of file
      webpage_t* page = webpage_new(url, depth, NULL);
      if (webpage_fetch(page)) {
        indexPage(index, page, docID);
      }
      webpage_delete(page);
    }
    else {
      break;
    }
    docID++;
  }
  return index;
}

/**************** indexPage ***********************************/
/* see indexer.h for description */
void
indexPage(index_t* index, webpage_t* page, int docID) {
  int pos = 0;
  char* word;
  while ((word = webpage_getNextWord(page, &pos)) != NULL) {
    if (strlen(word) >= 3) {
      char normalized[100]; //ASSUMPTION: word can be maximum length of 100
      strcpy(normalized, word);
      normalizeWord(normalized);
  // if word is in index
      counters_t* ctr;
      if ((ctr = index_find(index, normalized)) != NULL) {
  // increment appropriate counter
        counters_add(ctr, docID);
      }
  // if word is not in index
      else {
        ctr = counters_new();
        counters_add(ctr, docID);
        index_insert(index, normalized, ctr);
      }
    }
    mem_free(word);
  }
}