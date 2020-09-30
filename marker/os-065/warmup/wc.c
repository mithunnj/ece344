// Owner: Mithun Jothiravi
// Course: ECE 344
/* Description: Change the wc.c file so that it counts how often words occur in an array. The interface to this program is defined in the wc.h header file. You must use a hash table to implement this program because your hash table implementation can then be used in future labs.

Furthermore, we will test your code on a large input file, and your program must run in less than 30 seconds. To speed up your hash table implementation, you can use any good hash key function, including an implementation of the hash key function available from elsewhere. However, the rest of the hashing code should be your own implementation. You can choose any hash table size, but a good rule of thumb is to use a size that is roughly twice the total number of elements that are expected to be stored in the hash table.

The simple test in test_wc.c should now run. To check if your implementation works, run the run_small_test_wc script.

Now test the efficiency of your hash table implementation with a large input file by running the run_big_test_wc script. This script will work correctly if your program takes less than 30 seconds. Otherwise, you will need to think about ways to improve your hash table performance.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "wc.h"

#define MAX_STRING 5000

unsigned int hash(const char* str, unsigned int length) {
	unsigned int hash_value = 0;

	for (int i=0; i<length; i++) {
		hash_value += str[i];
		//hash_value = hash_value * str[i];
	}

	return hash_value;
}


struct wc {
	char *word; // Store the word
	int count; // Store the word count
};

struct wc *
wc_init(char *word_array, long size)
{
	int token_hash;
	struct wc *wc = (struct wc *)malloc(sizeof(struct wc)*MAX_STRING);
	assert(wc);

	// Initialize default values for wc struct
	for (int i=0; i<MAX_STRING; i++) {
		wc[i].word = "";
		wc[i].count = 0;
	}

	// Copy input string
	char copy_input[MAX_STRING] = "";
	strcpy(copy_input, word_array);

	// String split setup using strtok
	char delimeter[] = " "; // Define delimeter for string split
	char *token = strtok(copy_input, delimeter);
	
	// Loop through split string
	while (token != NULL) {

		// Calculate hash for word
		token_hash = hash(token, strlen(token));
	
		if (strcmp(wc[token_hash].word, "") == 0) { // If there is no word stored in token_hash slot within hash table, initialize slot
			wc[token_hash].word = token;
			wc[token_hash].count = 1;

		} else { // If the word was already detected and stored, increment the counter
			wc[token_hash].count++;
		}

		token = strtok(NULL, delimeter); // Increment token 
	}


	return wc;
}

void
wc_output(struct wc *wc)
{
	for (int i=0; i<MAX_STRING; i++) {
		if (strcmp(wc[i].word, "") != 0) {
			printf("%s:%d\n", wc[i].word, wc[i].count);
		}
	}
	return;
}

void
wc_destroy(struct wc *wc)
{
	free(wc);
}