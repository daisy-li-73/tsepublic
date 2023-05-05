/*
* A helper module for crawler. Initializes and validates a pageDirectory. 
* Handles saving webpage to a file in the pageDirectory, as specified in design. 
*
* Daisy Li, April 2023
*/

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"
#include <string.h>

/**************** pagedir_init() ****************/
/* Creates .crawler file in pageDirectory 
* If .crawler file cannot be created, return false.
* Otherwise, return true. 
*/
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save() ****************/
/* Constructs a file pathname in pageDirectory to store a web page.
* Returns pathname. 
*/

char* pagedir_save(const char* pageDirectory, const int docID);

/**************** pagedir_write() ****************/
/* Writes a webpage to a given output file. 
 * First line is page's URL, second line is depth at which page was found,
* third line is contents of webpage. */
void pagedir_write(const webpage_t* page, const char* pathname);

#endif // _PAGEDIR_H