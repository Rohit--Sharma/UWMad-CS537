///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      November 24, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "redblack_tree.h"


/*This is a safe malloc that records a ptr and size in the rbtree for memory that has been allocated. 
ptr is obtained from return of malloc and size is checked if its 0 and reported with a warning
*/
void *malloc537(size_t size) {
	// If size is 0, report the error and continue
	if (size == 0) {
		fprintf(stderr, "Warning: Calling malloc with size 0.\n");
	}

	// Allocate the memory by calling malloc
	void *ptr = malloc(size);
	
	// Delete all the freed nodes whose address is contained in the newly allocated memory
	// rbtree_node *prev_freed_in_range = rbtree_range_search(ptr, size);
	// while (prev_freed_in_range != NULL) {
	// 	rbtree_delete_node(prev_freed_in_range->ptr);
	// 	prev_freed_in_range = rbtree_range_search(ptr, size);
	// }
	rbtree_delete_in_range(ptr, size);

	// Split the free node which contains the newly allocated memory's start pointer into two nodes
	rbtree_node *node_to_split = rbtree_interval_search(ptr, 1);
	if (node_to_split != NULL) {
		// Check if the newly allocated address is in middle and not the same as node_to_split's ptr
		if (ptr > node_to_split->ptr) {
			// Split the node here (Which is identical to just reducing the node's size and later inserting a new node)
			node_to_split->size = ((size_t)ptr - (size_t)node_to_split->ptr);
		}
	}

	rbtree_node *old_node = rbtree_node_search(ptr);
	// If the node isn't in rbtree, insert it. Else, check if the memory was previously freed.
	if (old_node == NULL) {
		rbtree_insert(ptr, size);
	}
	else {
		if (old_node->free == 1) {
			// printf("The memory was previously freed.\n");
			// TODO: What now? For now, just setting free to 0 and changing the size
			old_node->free = 0;
			old_node->size = size;
		}
		else {
			fprintf(stderr, "Error: malloc() library call returned an address that was already allocated. Exiting...\n");
			exit(-1);
		}
	}

	return ptr;
}

/*This function checks if freeing the memory associated with ptr is correct and then invokes free() to do the actual free. 
It verifies that the memory being freed was allocated with malloc537, that it is not the first byte of range of memory that was
allocated and is not being double freed. Errors are reported if the conditions are violated.*/
void free537(void *ptr) {
	//If pointer is null, it is an error and the program exits.
	if (ptr == NULL) {
		fprintf(stderr, "Error: Trying to free a NULL pointer! Exiting...\n");
		exit(-1);
	}

	// TODO: Is this how to search the node? What if the ptr is in the middle of the node?
	rbtree_node *node_to_free = rbtree_node_search(ptr);
	if (node_to_free == NULL) {
		// Check if the address that is being freed is somewhere in the middle to throw an error accordingly.
		node_to_free = rbtree_interval_search(ptr, 0);
		if (node_to_free == NULL) {
			fprintf(stderr, "Error: Cannot free a memory location that hasn't been allocated. Exiting...\n");
			exit(-1);
		}
		else {
			fprintf(stderr, "Error: Attempting to free memory at %p which is not a starting address. It is part of memory already allocated at %p with size %ld! Exiting...\n", ptr, node_to_free->ptr, node_to_free->size);
			exit(-1);
		}
	}
	else {
		if (node_to_free->free) {
			fprintf(stderr, "Error: Trying to free a memory that has already been freed. Exiting...\n");
			exit(-1);
		}
		else {
			// Call free and mark the node as freed in the data structure
			free(ptr);
			node_to_free->free = 1;
		}
	}
}

/*This function reallocates memory. It checks if ptr is null and invokes malloc to handle it.
If size is 0 and ptr is not NULL, then free is invoked. Realloc changes the memory location 
and the function checks if a node exists with ptr and of length size. */
void *realloc537(void *ptr, size_t size) {
	if (ptr == NULL) {
		// Let malloc537 handle it
		return malloc537(size);
	}
	else if (size == 0) {
		// Let free537 handle it
		fprintf(stderr, "Warning: Trying to realloc with size 0. Freeing the memory instead.\n");
		free537(ptr);
		return malloc537(size);	// TODO: Should you return back the ptr or NULL?
	}
	else {
		// Check if ptr is a valid starting address that has been previously allocated.
		rbtree_node *node_to_realloc = rbtree_node_search(ptr);
		if (node_to_realloc == NULL) {
			fprintf(stderr, "Error: Cannot realloc a memory location that hasn't been allocated or isn't a starting address. Exiting...\n");
			exit(-1);
		}

		void *new_ptr = realloc(ptr, size);
		if (new_ptr != ptr) {
			// Free the previous memory at ptr
			node_to_realloc->free = 1;
		}

		// Delete all the freed nodes whose address is contained in the newly allocated memory
		// rbtree_node *prev_freed_in_range = rbtree_range_search(new_ptr, size);
		// while (prev_freed_in_range != NULL) {
		// 	rbtree_delete_node(prev_freed_in_range->ptr);
		// 	prev_freed_in_range = rbtree_range_search(new_ptr, size);
		// }
		rbtree_delete_in_range(new_ptr, size);

		// Split the free node which contains the newly allocated memory's start pointer into two nodes
		rbtree_node *node_to_split = rbtree_interval_search(new_ptr, 1);
		if (node_to_split != NULL) {
			// Check if the newly allocated address is in middle and not the same as node_to_split's ptr
			if (new_ptr > node_to_split->ptr) {
				// Split the node here (Which is identical to just reducing the node's size and later inserting a new node)
				node_to_split->size = ((size_t)new_ptr - (size_t)node_to_split->ptr);
			}
		}

		rbtree_node *old_node = rbtree_node_search(new_ptr);
		// If the node isn't in rbtree, insert it. Else, check if the memory was previously freed.
		if (old_node == NULL) {
			rbtree_insert(new_ptr, size);
		}
		else {
			old_node->free = 0;
			old_node->size = size;
		}

		return new_ptr;
	}
}

/*This funtion checks if address range specified by ptr and size length are within range allocated by malloc537 and not
 yet freed using free537. When error is deteched, it is reported and program is exited.*/
void memcheck537(void *ptr, size_t size) {
	// TODO: Verify for all the nodes in the range? No, the answer in piazza #301 says only check the first one
	rbtree_node *node_to_check = rbtree_interval_search(ptr, 0);
	if (node_to_check == NULL || node_to_check->free) {
		fprintf(stderr, "Error: Memory check failed. The memory within the range is either not allocated or already freed. Exiting...\n");
		exit(-1);
	}
	else {
		// Check if the bounds of ptr are within the allocated memory
		if ((size_t)ptr + size > (size_t)node_to_check->ptr + node_to_check->size) {
			fprintf(stderr, "Error: Memory check failed. Not all the memory starting from %p with size %ld is allocated! Exiting...\n", ptr, size);
			exit(-1);
		}
	}
	return;
}
