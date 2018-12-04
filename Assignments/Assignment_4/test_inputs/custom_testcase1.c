#include <stdio.h>
#include <stdlib.h>
#include "537malloc.h"
#include "redblack_tree.h"

int main() {
	printf("Calling malloc with size 0\n");
	void *ptr = malloc537(0);
	printf("Malloced 0 bytes at %p\n", ptr);

	// printf("Freeing the ptr\n");
	// free537(ptr);
	// printf("Successfully freed.\n");

	// printf("Freeing the ptr\n");
	// free537(ptr);
	// printf("Successfully freed.\n");

	printf("Reallocing the ptr\n");
	ptr = realloc537(ptr, 0);
	printf("Successfully realloced at %p.\n", ptr);

	memcheck537(ptr, 0);
	printf("Memcheck successful\n");

	return 0;
}