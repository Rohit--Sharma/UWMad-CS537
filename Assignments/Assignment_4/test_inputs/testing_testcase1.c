#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "537malloc.h"
#include "redblack_tree.h"

struct testNode {
	int val;
	char name;
};

int main()
{
	int *ptr1 = (int *) malloc537(sizeof(int) * 10);
	printf("Mem allocated at %p\n", (void *)ptr1);
	int *ptr2 = (int *) malloc537(sizeof(int) * 10);
	printf("Mem allocated at %p\n", (void *)ptr2);
	int *ptr3 = (int *) malloc537(sizeof(int) * 10);
	printf("Mem allocated at %p\n", (void *)ptr3);

	printf("Freeing all\n");
	free537(ptr1);
	free537(ptr2);
	free537(ptr3);

	printf("Reallocating at %p\n", (void *)ptr1);
	ptr1 = realloc537(ptr1, 4);
	printf("Realloc done at %p\n", (void *)ptr1);

	ptr1 = realloc537(ptr1 + 2, 4);
	printf("This should not be printed.\n");

	return 0;
}
