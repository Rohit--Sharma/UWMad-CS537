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
	printf("Allocated 50 bytes of Memory at %p\n", this_ptr1);

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
    //printf("Trying to free 10 bytes from byte 20 to 30";
	//// 1-B: Simple testcase - Memory check 1st 5 byts
	//printf("Checking first 5 bytes of %p\n", this_ptr);
	//memcheck537(this_ptr, 5);

	//strncpy(this_ptr, "hello world", 10);
	//printf("Content : %s\n", this_ptr);

	//// 1-C: Simple testcase - Memory check last 5 bytes
	//printf("Checking last 5 bytes of %p\n", this_ptr);
	//memcheck537(this_ptr + 5, 5);

	//printf("Freeing memory : %p\n", this_ptr);
	//// 1-D: Correct Free
	//free537(this_ptr);
	//printf("Successfully freed memory!\n");

	//// 1-E: Same cases as above
	//printf("Allocating memory for the struct\n");
	//struct testNode *new_ptr = (struct testNode *)malloc537(sizeof(struct testNode));
	//if(new_ptr == NULL) {
	//	printf("Memory allocation failed!\n");
	//	exit(1);
	//}
	//printf("Allocated Memory : %p\n", new_ptr);
	//printf("Allocated size : %ld\n", sizeof(struct testNode));

	//printf("Checking 1st 5 bytes of memory allocated\n");
	//memcheck537(new_ptr, 5);

	//printf("Freeing the structure pointer : %p\n", new_ptr);
	//free(new_ptr);

	//printf("If this prints, everything is success!\n");
	return 0;
}
