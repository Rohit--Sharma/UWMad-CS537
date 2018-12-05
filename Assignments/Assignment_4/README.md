# Programming Assignment - 3
## *Safe malloc/free library*

The features of the *safe malloc* library:
- malloc537 takes a size parameter and returns a pointer to the allocated memory
- free537 checks if the specified ptr is valid, was allocated with malloc537, is the first byte of range of allocated memory, and if the memory was already freed.
- realloc537 takes care of reallocating memory similar to realloc() and does the needful in different corner cases based on POSIX description.
- memcheck537 checks if address range specified by ptr and size falls within range allocated by malloc537() and memory not freed already by free537.
- Takes care of all cases and prints appropriate error messages.
- It is implemented as a balanced red black tree and has several range, interval, node queries that aid in implementing the above functions.

### Usage
The following indicates how to use the *537malloc* utility. To run the program with our functions, copy your test case (using safe malloc function calls) as main.c and compile the program by utilizing the make utility as follows:
```
$ make
```
It executes all the source codes and creates the object files and the final executable called *output*

To compile the program with CSA, run the following command:
```
$ scan-build make
```
Now, running the binary will produce the following output for certain testcases provided as part of test_inputs:
```
$ cat simple_testcase1.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "537malloc.h"

struct testNode {
	int val;
	char name;
};

int main()
{
	// 1-A: Simple testcase - Memory Allocation
	printf("Allocating 10 bytes of Memory\n");
	char *this_ptr = malloc537(sizeof(char) * 10);
	if(this_ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}

	// 1-B: Simple testcase - Memory check 1st 5 byts
	printf("Checking first 5 bytes of %p\n", this_ptr);
	memcheck537(this_ptr, 5);

	strncpy(this_ptr, "hello world", 10);
	printf("Content : %s\n", this_ptr);

	// 1-C: Simple testcase - Memory check last 5 bytes
	printf("Checking last 5 bytes of %p\n", this_ptr);
	memcheck537(this_ptr + 5, 5);

	printf("Freeing memory : %p\n", this_ptr);
	// 1-D: Correct Free
	free537(this_ptr);
	printf("Successfully freed memory!\n");

	// 1-E: Same cases as above
	printf("Allocating memory for the struct\n");
	struct testNode *new_ptr = (struct testNode *)malloc537(sizeof(struct testNode));
	if(new_ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	printf("Allocated Memory : %p\n", new_ptr);
	printf("Allocated size : %ld\n", sizeof(struct testNode));

	printf("Checking 1st 5 bytes of memory allocated\n");
	memcheck537(new_ptr, 5);

	printf("Freeing the structure pointer : %p\n", new_ptr);
	free(new_ptr);

	printf("If this prints, everything is success!\n");
	return 0;
}


$ make; ./output

Allocating 10 bytes of Memory
Checking first 5 bytes of 0x1748420
Content : hello worl
Checking last 5 bytes of 0x1748420
Freeing memory : 0x1748420
Successfully freed memory!
Allocating memory for the struct
Allocated Memory : 0x1748420
Allocated size : 8
Checking 1st 5 bytes of memory allocated
Freeing the structure pointer : 0x1748420
If this prints, everything is success!

$ cat error_testcase3.c

#include <stdio.h>
#include "537malloc.h"

#define SIZE 1000

int main() {
	int *ptr = malloc537(SIZE);
	printf("Allocated %d bytes @ %p\n", SIZE, ptr);

	printf("Memcheck at %p - 10 : Should fail - Invalid address!\n", ptr);
	memcheck537(ptr - 10, SIZE);

	printf("If this prints, no points\n");

	return 0;
}

$ make; ./output

Allocated 1000 bytes @ 0x9a4010
Memcheck at 0x9a4010 - 10 : Should fail - Invalid address!
Error: Memory check failed. The node within the range is not allocated or already freed. Exiting...
```
### Program Organization
The program files are organized in the following manner:
- proj4/
	- README.md
	- partner.txt
	- 537malloc.c
	- 537malloc.h
	- redblack_tree.c
	- redblack_tree.h
	- Makefile
	- main.c (dummy test sample_testcase1.c)

The *redblack_tree.c* file contains the different functions that are used for constructing and using the red black tree such as create_node, insert_node, delete_node and it also takes care of balancing the tree after every insert and delete. File *537malloc.c* contains the checks and our versions of the required malloc, free, memcheck, realloc functions. 

The *Makefile* contains the rules to compile and link all the source code files and create the executable *output*.


### Description
Everytime malloc537 is called with a size, it internally calls malloc which returns a pointer after allocating memory starting at pointer of required size. This is inserted as a node into the redblack tree. In case of free537, it internally calls free and deletes the corresponding node from redblack tree. realloc537 takes care of the different corner cases as per POSIX standard and reallocates like realloc. memcheck verifies whether the memory range is a valid one that had been allocated with malloc537 and not freed by free537. All possible error test cases are taken care of. 
 
