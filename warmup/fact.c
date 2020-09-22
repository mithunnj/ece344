// Owner: Mithun Jothiravi
// Class: ECE 344 (OS)
/* Description: Write the program fact.c that uses recursion to calculate and print the factorial of the positive integer value (e.g., 5, 10, 030) passed in as an argument to the program, or prints the line "Huh?" if no argument is passed in, or if the first argument passed is not a positive integer (e.g., 0, negative value, decimal value, or non-integral value). If the value passed in exceeds 12, it prints "Overflow".

$ make fact
$ ./fact one
Huh?
$ ./fact 5
120
$ ./fact 5.1
Huh? */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int FACTORIAL_RESULT = 1;

int calc_fact(int fact_index) {

	FACTORIAL_RESULT *= fact_index;
	fact_index--; // Decrement index

	return fact_index;
}

int main( int argc, char *argv[] )
{
	int user_int = atoi(argv[1]); // Convert user input from string to integer
	float user_float = atof(argv[1]); // Convert user input from string to float

	// Check if the first argument passed is a positive integer (e.g., 0, negative value, decimal value, or non-integral value).
	if ((0 < user_int && user_int <= 12) && (user_int == user_float)) {

		int index = user_int; // Set the user input as the starting index for the factorial calculations.

		while(index > 1) { // Iterate through the factors, and everytime the factorial is calculated, decrement the index
			index = calc_fact(index);
		}

		printf("%d", FACTORIAL_RESULT);
		
	}
	else if (user_int > 12) { // User input is too large for factorial processing.
		printf("Overflow\n");
	}
	else {
		printf("Huh?\n"); // Default error message.
	}

	return 0;
}
