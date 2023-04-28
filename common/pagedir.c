/*
* A helper module for crawler. Initializes and validates a pageDirectory. 
* Handles saving webpage to a file in the pageDirectory, as specified in design. 
*
* Daisy Li, April 2023
*/

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include <string.h>

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool
pagedir_init(const char* pageDirectory) {
    char* crawler = ".crawler";
    char pathname[strlen(pageDirectory) + strlen(crawler)+1];
    // create pageDirectory/.crawler pathname 
    sprintf(pathname, "%s/%s", pageDirectory, crawler);

    FILE* fp = fopen(pathname, "w");
    // check if .crawler file is successfully opened for writing
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    char id[12];
    sprintf(id, "%d", docID); 
    char pathname[strlen(pageDirectory) + strlen(id)+1];
    // create pageDirectory/id pathname
    sprintf(pathname, "%s/%s", pageDirectory, id);

    FILE* fp = fopen(pathname, "w");
    // check if file is successfully opened for writing 
    if (fp != NULL) {
        fprintf(fp, "%s\n", webpage_getURL(page));
        fprintf(fp, "%d\n", webpage_getDepth(page));
        fprintf(fp, "%s\n", webpage_getHTML(page));
        fclose(fp);
    }
}
