#include <stdio.h>
#include "gfg_rbtree.h"

void test_insert(int *base_ptr) {
	// test insert
	//int a;
	//a = rbtree_insert(base_ptr + 47, sizeof(int) * 5);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 5, sizeof(int) * 5);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 60, sizeof(int) * 20);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 35, sizeof(int) * 10);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 85, sizeof(int) * 5);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 95, sizeof(int) * 5);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 20, sizeof(int) * 5);
	//printf("Node inserted successfully ? %d\n", a);
	//a = rbtree_insert(base_ptr + 54, sizeof(int) * 6);
	//printf("Node inserted successfully ? %d\n", a);
	//rbtree_print();
	int a;
	a = rbtree_insert_node(base_ptr + 47, sizeof(int) * 5);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 5, sizeof(int) * 5);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 60, sizeof(int) * 20);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 35, sizeof(int) * 10);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 85, sizeof(int) * 5);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 95, sizeof(int) * 5);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 20, sizeof(int) * 5);
	printf("Node inserted successfully ? %d\n", a);
	a = rbtree_insert_node(base_ptr + 54, sizeof(int) * 6);
	printf("Node inserted successfully ? %d\n", a);
	rbtree_print();
}


void test_point_search(int *base_ptr, rbtree_node* root) {
	rbtree_node *searched_node = node_search_rbtree(base_ptr + 54, root);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}
	searched_node = node_search_rbtree(base_ptr + 20, root);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}
	searched_node = node_search_rbtree(base_ptr + 60, root);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}
	searched_node = node_search_rbtree(base_ptr + 40, root);
	if (searched_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", searched_node);
	}
	searched_node = node_search_rbtree(base_ptr + 92, root);
	if (searched_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", searched_node);
	}
}

void test_interval_search(int *base_ptr, rbtree_node *root) {
	printf("Testing interval_search()\n");
	rbtree_node *itvl_node = interval_search_rbtree(base_ptr + 54, root);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = interval_search_rbtree(base_ptr + 22, root);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = interval_search_rbtree(base_ptr + 60, root);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = interval_search_rbtree(base_ptr + 40, root);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = interval_search_rbtree(base_ptr + 4, root);
	if (itvl_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", itvl_node);
	}
	itvl_node = interval_search_rbtree(base_ptr + 92, root);
	if (itvl_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", itvl_node);
	}
}

void test_range_search(int *base_ptr, rbtree_node *root) {
	printf("Testing range_search()\n");
	node_search_rbtree(base_ptr + 54, root)->free = 1;
	rbtree_node *range_node = range_search_rbtree(base_ptr + 52, sizeof(int) * 10, root);
	if (range_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", range_node);
	}
	node_search_rbtree(base_ptr + 35, root)->free = 1;
	range_node = range_search_rbtree(base_ptr + 30, sizeof(int) * 16, root);
	if (range_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", range_node);
	}
	range_node = range_search_rbtree(base_ptr + 47, sizeof(int) * 5, root);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
	range_node = range_search_rbtree(base_ptr + 12, sizeof(int) * 5, root);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
	range_node = range_search_rbtree(base_ptr + 92, sizeof(int) * 10, root);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
	range_node = range_search_rbtree(base_ptr + 18, sizeof(int) * 5, root);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
}

void test_delete_node(int *base_ptr, rbtree_node *root) {
	printf("%p - ROOT\n", (void *)root);
	rbtree_node *node = node_search_rbtree(base_ptr + 60, root);
	delete_rbtree_node(node);
	rbtree_print(); // doesn't behave properly. Tree is no longer BST in case of root (47) and no longer red black in case of deleting 60
	printf("Is red-black after deletion: %d\n", isRedBlackTree());
	root = get_rbtree_root();
	printf("%p - ROOT\n", (void *)root);
	node = node_search_rbtree(base_ptr + 95, root);
	delete_rbtree_node(node);
	rbtree_print();
}


int main() {
	int temp[100] = {0};
	int *base_ptr = temp;

	test_insert(base_ptr);
	printf("Is red-black after insertion: %d\n", isRedBlackTree());

	rbtree_node *root = get_rbtree_root();
	// Test searching
	
	test_point_search(base_ptr, root);
	test_interval_search(base_ptr, root);
	test_range_search(base_ptr, root);
	printf("Is red-black after search: %d\n", isRedBlackTree());
	
	// test deleting
	test_delete_node(base_ptr, root);
	printf("Is red-black after deletion: %d\n", isRedBlackTree());

	return 0;
}
