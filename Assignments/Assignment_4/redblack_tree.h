///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      November 24, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>

typedef struct RBNode {
	struct RBNode *parent;
	struct RBNode *children[2];
	void *ptr;
	size_t size;
	int free;
	int red;
} RBNode;

int rbtree_insert(void *ptr, size_t size);
int rbtree_delete_node(void *ptr);

RBNode *rbtree_point_search(void *ptr);
RBNode *rbtree_interval_search(void *ptr);
RBNode *rbtree_range_search(void *ptr, size_t size);

void rbtree_print();
