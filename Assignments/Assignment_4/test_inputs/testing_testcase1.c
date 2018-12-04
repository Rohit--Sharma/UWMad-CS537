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
	printf("Allocating 100 bytes of Memory\n");
	char *this_ptr1 = malloc537(sizeof(char) * 100);
	if(this_ptr1 == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 100 bytes of Memory at %p\n", this_ptr1);

	printf("Allocating 50 bytes of Memory\n");
	char *this_ptr2 = malloc537(sizeof(char) * 50);
	if(this_ptr2 == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 50 bytes of Memory at %p\n", this_ptr2);
	printf("difference in pointers is %d\n", (int)(this_ptr2-this_ptr1));
	
    printf("Allocating 200 bytes of Memory\n");
	char *this_ptr3 = malloc537(sizeof(char) * 200);
	if(this_ptr3 == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 200 bytes of Memory at %p\n", this_ptr3);

	
	printf("Freeing memory : %p\n", this_ptr1);
	free537(this_ptr1);

	printf("Freeing memory from the middle of second pointer: %p ie %p\n", this_ptr2, this_ptr2+20);
	free537(this_ptr2+20);
	printf("Freeing memory : %p\n", this_ptr3);
	free537(this_ptr3);
    
	
	printf("Allocating 100 bytes of Memory - part 1\n");
	char *this_ptrA = malloc537(sizeof(char) * 100);
	if(this_ptrA == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 100 bytes of Memory at %p\n", this_ptrA);

	printf("Allocating 100 bytes of Memory - part 2\n");
	char *this_ptrB = malloc537(sizeof(char) * 100);
	if(this_ptrB == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 100 bytes of Memory at %p\n", this_ptrB);


	printf("Allocating 50 bytes of Memory - part 3\n");
	char *this_ptrC = malloc537(sizeof(char) * 50);
	if(this_ptrC == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 50 bytes of Memory at %p\n", this_ptrC);

	
	printf("Allocating 100 bytes of Memory - part 4\n");
	char *this_ptrD = malloc537(sizeof(char) * 100);
	if(this_ptrD == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated 100 bytes of Memory at %p\n", this_ptrD);
	
	printf("free part 1\n");
	free(this_ptrA);

	printf("Realloc Part A with 30 bytpes in the middle\n");
	realloc537(this_ptrA, 30);

	printf("Now we free part 2,3,4 and then realloc 400 bytes frlom start of part 2\n");
	free(this_ptrB);
	free(this_ptrC);
	free(this_ptrD);

	realloc(this_ptrB, 400);

	printf("Now we try freeing part 3 again\n");
	free(this_ptrC);

	return 0;
}
