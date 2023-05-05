/* 
 * A helper module for indexer.
 * 
 * Daisy Li, April 2023
*/

#include <ctype.h>
#include <string.h>
#include <stdio.h>


/**************** normalizeWord() ****************/
/* Normalizes a word in place. Converts all uppercase letters to lowercase. */
void normalizeWord(char* word);