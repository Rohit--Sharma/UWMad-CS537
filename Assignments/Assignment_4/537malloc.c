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

void *malloc537(size_t size) {
	// If size is 0, report the error and continue
	if (size == 0) {
		fprintf(stderr, "Error: Cannot malloc memory of size 0.\n");
		return NULL;
	}

	// Allocate the memory by calling malloc
	void *ptr = malloc(size);
	
	rbtree_node *old_node = rbtree_node_search(ptr);
	// If the node isn't in rbtree, insert it. Else, check if the memory was previously freed.
	if (old_node == NULL) {
		rbtree_insert(ptr, size);
	}
	else {
		if (old_node->free == 1) {
			printf("The memory was previously freed.\n");
			// TODO: What now? For now, just setting free to 0 and changing the size
			old_node->free = 0;
			old_node->size = size;
		}
	}

	return ptr;
}

void free537(void *ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "Error: Trying to free a NULL pointer! Exiting...\n");
		exit(-1);
	}

	// TODO: Is this how to search the node? What if the ptr is in the middle of the node?
	rbtree_node *node_to_free = rbtree_node_search(ptr);
	if (node_to_free == NULL) {
		node_to_free = rbtree_interval_search(ptr);
		if (node_to_free == NULL) {
			fprintf(stderr, "Cannot free a memory location that hasn't been allocated. Exiting...\n");
			exit(-1);
		}
		else {
			fprintf(stderr, "Attempting to free memory at %p which is not a starting address. It is part of memory already allocated at %p with size %ld! Exiting...\n", ptr, node_to_free->ptr, node_to_free->size);
			exit(-1);
		}
	}
	else {
		if (node_to_free->free) {
			fprintf(stderr, "Trying to free a memory that has already been freed. Exiting...\n");
			exit(-1);
		}
		else {
			// Call free and mark the node as freed in the data structure
			free(ptr);
			node_to_free->free = 1;
		}
	}
}

void *realloc537(void *ptr, size_t size) {
	if (ptr == NULL && size == 0) {
		fprintf(stderr, "Invalid parameters passed to realloc537. Exiting...\n");
		exit(-1);
	}
	else if (ptr == NULL) {
		// Let malloc537 handle it
		return malloc537(size);
	}
	else if (size == 0) {
		// Let free537 handle it
		free537(ptr);
		return NULL;	// TODO: Should you return back the ptr or NULL?
	}
	else {
		// TODO: verify if the below step is correct
		rbtree_node *node_to_realloc = rbtree_node_search(ptr);
		if (node_to_realloc != NULL) {
			// TODO: Delete it or just mark it as free?
			rbtree_delete_node(ptr);
		}
		void *new_ptr = realloc(ptr, size);
		rbtree_insert(new_ptr, size);
		return new_ptr;
	}
}

void memcheck537(void *ptr, size_t size) {
	// TODO: Verify for all the nodes in the range? No, the answer in piazza #301 says only check the first one
	rbtree_node *node_to_check = rbtree_interval_search(ptr);
	if (node_to_check == NULL || node_to_check->free) {
		fprintf(stderr, "Memory check failed. The node within the range is not allocated. Exiting...\n");
		exit(-1);
	}
	else {
		// Check if the bounds of ptr are within the allocated memory
		if (ptr + size > node_to_check->ptr + node_to_check->size) {
			fprintf(stderr, "Memory check failed. Not all the memory starting from %p with size %ld is allocated! Exiting...\n", ptr, size);
			exit(-1);
		}
	}
	return;
}
