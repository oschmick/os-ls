#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 3) {
		printf("./ls2Sol <path> [exact-match-pattern]\n");
		return 1;
	}

	char *input = argv[1];

	if (isDir(input)) {
		if (argc == 2) {
			printf("%s/ (directory)\n", input);
			recurseDir(input, "    ");
		} else if (argc == 3) {
			printf("3 arguments! Oh no!");
		}
	} else {
		printf("%s is not a directory path! Exiting...\n", input);
		return 1;
	}





	/*
	// stack stores the lines to print out
	stack_t *s = initstack();

	push(s, "Hello1");
	push(s, "Hello2");
	push(s, "Hello3");

	// print stack
	printstack(s);

	char *top = pop(s);
	printf("Top of stack: %s\n", top);

	// print stack
	printstack(s);

	// free stack
	freestack(s);
	return 0;
	*/
}
