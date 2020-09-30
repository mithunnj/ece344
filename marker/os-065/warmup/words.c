// Owner: Mithun Jothiravi
// Class: ECE 344 (OS)
/* Description: Write the program words.c that prints out the words from the command line on different lines. The words in the command line are available using the argc and argv parameters of main() (see hi.c).

$ make words
...
$ ./words To be or not to be. That is the question.
To
be
or
not
to
be.
That
is
the
question.
You can test your program by running the tester again. You should get some more marks for passing this test. */

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
