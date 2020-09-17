// Owner: Mithun Jothiravi
// Class: ECE 344 (OS)

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] )
{
	int user_int = atoi(argv[1]); // Convert user input from string to integer
	float user_float = atof(argv[1]); // Convert user input from string to float

	// Check if the first argument passed is a positive integer (e.g., 0, negative value, decimal value, or non-integral value).
	if ((0 < user_int && user_int <= 12) && (user_int == user_float)) {
		int factorial = 1;
		for (int i=user_int; i>1; i--) {
			factorial *= i;
		}
		printf("%d\n", factorial); // Return factorial result to the user
	}
	else if (user_int > 12) { // User input is too large for factorial processing.
		printf("Overflow\n");
	}
	else {
		printf("Huh?\n"); // Default error message.
	}

	return 0;
}
