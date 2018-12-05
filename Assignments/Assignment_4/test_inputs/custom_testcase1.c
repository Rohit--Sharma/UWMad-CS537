#include <stdio.h>
#include <stdlib.h>
#include "537malloc.h"
#include "redblack_tree.h"

int main() {
	printf("Calling malloc with size 10\n");
	void *ptr = malloc537(10);
	printf("Malloced 10 bytes at %p\n", ptr);

	printf("Reallocing the ptr with size 0\n");
	ptr = realloc537(ptr, 0);
	
	printf("Trying to free the ptr at %p. It should fail\n", ptr);
	free537(ptr);
	printf("This should not be printed.\n");

	return 0;
}