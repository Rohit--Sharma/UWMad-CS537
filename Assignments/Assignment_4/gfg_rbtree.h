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

typedef struct rbtree_node {
	struct rbtree_node *parent;
	struct rbtree_node *children[2];
	void *ptr;
	size_t size;
	int free;
	int red;
} rbtree_node;

int rbtree_insert_node(void* ptr, size_t size);
//int insert_node(void *ptr, size_t size);
//int delete_node(void *ptr);

//rbtree_node *rbtree_point_search(void *ptr);
//rbtree_node *rbtree_interval_search(void *ptr);
//rbtree_node *rbtree_range_search(void *ptr, size_t size);

void rbtree_print();
//void rbtree_print();
int isRedBlackTree();
