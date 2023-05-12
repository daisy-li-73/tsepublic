/*
* A crawler for TSE. Requirements, specifications, and 
* implementation described in assigment.
*
* Daisy Li, April 2023
*/

#include "hashtable.h"
#include "bag.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "pagedir.h"
#include "mem.h"

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const int depth, const char *word, const char *url);

/* **************************************** */
int main(const int argc, char *argv[]) {
  char* seedURL;
  char* pageDirectory;
  int maxDepth;

  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
  crawl(seedURL, pageDirectory, maxDepth);

  return 0;
}

/**************** parseArgs ***********************************/
/* Given arguments from the command line, extract them into the function parameters;
 * return only if successful. */
static void 
parseArgs(const int argc, char* argv[],
char** seedURL, char** pageDirectory, int* maxDepth) {
  // check number of arguments
  if (argc == 4) {
    *seedURL = argv[1];
    *pageDirectory = argv[2];
    sscanf(argv[3], "%d", maxDepth);

  // for seedURL, normalize the URL and check it is an internal URL
    char* normalized = normalizeURL(*seedURL);
    if (normalized == NULL) {
      fprintf(stderr, "Error: invalid URL \n");
      //mem_free(normalized);
      exit(3);
    }
    *seedURL = strcpy(*seedURL, normalized);
    mem_free(normalized);

    if (!isInternalURL(*seedURL)) {
      fprintf(stderr, "Error: external URL \n");
      exit(3);
    }

    // for pageDirectory, call pagedir_init()
    DIR* dir = opendir(*pageDirectory);
    // check that directory exists 
    if (dir != NULL) {
    // check that .crawler file can be created
      if (!pagedir_init(*pageDirectory)) {
          fprintf(stderr, "Error: unable to create a file of form %s/.crawler \n", *pageDirectory);
          mem_free(dir);
          exit(3);
      }
    }
    else {
      fprintf(stderr, "Error: %s does not exist \n", *pageDirectory);
      exit(3);
    }
    mem_free(dir);
    
    // for maxDepth, check that it is in range
    if (*maxDepth < 0 || *maxDepth > 10) {
      fprintf(stderr, "Error: maxDepth out of range \n");
      exit(3);
    }
  }
  else {
    fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth \n", argv[0]);
    exit(1); 
  }
}

/**************** crawl ***********************************/
/* Do the real work of crawling from seedURL to maxDepth and saving pages in pageDirectory*/

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
  hashtable_t* pagesSeen = hashtable_new(200); 
  hashtable_insert(pagesSeen, seedURL, "seen");

  // initialize the bag and add a webpage representing the seedURL at depth 0
  bag_t* pagesToCrawl = bag_new();
  char* webpage = malloc(strlen(seedURL)+1);
  strcpy(webpage, seedURL);

  webpage_t* seedPage = webpage_new(webpage, 0, NULL);
  bag_insert(pagesToCrawl, seedPage);
  int docID = 1;

  // while bag is not empty, pull a webpage from the bag
  webpage_t* page;
  while ((page = bag_extract(pagesToCrawl)) != NULL) {
  // fetch the HTML for that webpage and save it 
    if (webpage_fetch(page)) {
      logr(webpage_getDepth(page), "Fetched", webpage_getURL(page));
      char* pathname = pagedir_save(pageDirectory, docID);
      pagedir_write(page, pathname);
      if (webpage_getDepth(page) < maxDepth) {
        logr(webpage_getDepth(page), "Scanning", webpage_getURL(page));
        pageScan(page, pagesToCrawl, pagesSeen);
      }
      mem_free(pathname);
      docID++;
    }
    else {
      logr(webpage_getDepth(page), "Failed to fetch", webpage_getURL(page));
    }
    webpage_delete(page);
  }

  hashtable_delete(pagesSeen, NULL);
  bag_delete(pagesToCrawl, webpage_delete);
}

/**************** pageScan ***********************************/
/* Given a webpage, scan the given page to extract any links (URLs), 
 * ignoring non-internal URLs; for any URL not already seen before 
 * (i.e., not in the hashtable), sdd the URL to both the hashtable pages_seen 
 * and to the bag pages_to_crawl */

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    int pos = 0;
    char* nextURL;
    char* normalized;
    webpage_t* newPage;

    // while there is another URL in the page 
    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) {
      normalized = normalizeURL(nextURL);
      logr(webpage_getDepth(page), "Found", normalized);
      mem_free(nextURL);
      if (isInternalURL(normalized)) {
        if(hashtable_insert(pagesSeen, normalized, "seen")) {
    // create a webpage_t for the next URL; will fetch html later
          newPage = webpage_new(normalized, webpage_getDepth(page)+1, NULL);
          bag_insert(pagesToCrawl, newPage);
          logr(webpage_getDepth(page), "Added", normalized);
        }
        else {
        // already seen this page 
          logr(webpage_getDepth(page), "IgnDupl", normalized);
        }
      }
      else {
      // external URL
      logr(webpage_getDepth(page), "IngExtrn", normalized);
      }
    }
}

/*
 * logs status of pages as they are fetched, scanned, and added to the bag
*/
static void logr(const int depth, const char *word, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}
