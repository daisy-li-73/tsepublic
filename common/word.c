/* 
 * A helper module for indexer.
 * 
 * Daisy Li, April 2023
*/

#include <ctype.h>
#include <string.h>
#include <stdio.h>


/**************** normalizeWord() ****************/
/* see word.h for description */

void
normalizeWord(char* word) {
  for (int i = 0; word[i] != '\0'; i++) {
    word[i] = tolower(word[i]); 
  }
}
