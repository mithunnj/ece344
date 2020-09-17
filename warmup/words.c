// Owner: Mithun Jothiravi
// Class: ECE 344 (OS)

#include "common.h"
#include "stdio.h"
#include "stdlib.h"

int main( int argc, char *argv[] )
{
	// Loop through the arguments from the command line and print them to the console.
	for (int i=1; i<argc; i++) {
		printf("%s\n", argv[i]);
	}
	return 0;
}
