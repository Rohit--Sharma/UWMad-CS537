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
#include "redblack_tree.h"

void test_insert_ascending(int *base_ptr) {
	rbtree_insert(base_ptr + 0, sizeof(int) * 5);
	rbtree_insert(base_ptr + 10, sizeof(int) * 5);
	rbtree_insert(base_ptr + 16, sizeof(int) * 20);
	rbtree_insert(base_ptr + 40, sizeof(int) * 10);
	rbtree_insert(base_ptr + 52, sizeof(int) * 5);
	rbtree_insert(base_ptr + 60, sizeof(int) * 5);
	rbtree_insert(base_ptr + 67, sizeof(int) * 5);
	rbtree_insert(base_ptr + 80, sizeof(int) * 6);

	printf("Tree after insertions:\n");
	rbtree_print();
	printf("\n");
}

void test_insert_descending(int *base_ptr) {
	rbtree_insert(base_ptr + 80, sizeof(int) * 5);
	rbtree_insert(base_ptr + 67, sizeof(int) * 5);
	rbtree_insert(base_ptr + 60, sizeof(int) * 20);
	rbtree_insert(base_ptr + 52, sizeof(int) * 10);
	rbtree_insert(base_ptr + 40, sizeof(int) * 5);
	rbtree_insert(base_ptr + 16, sizeof(int) * 5);
	rbtree_insert(base_ptr + 10, sizeof(int) * 5);
	rbtree_insert(base_ptr + 0, sizeof(int) * 6);

	printf("Tree after insertions:\n");
	rbtree_print();
	printf("\n");
}

void test_insert_1(int *base_ptr) {
	rbtree_insert(base_ptr + 47, sizeof(int) * 5);
	rbtree_insert(base_ptr + 5, sizeof(int) * 5);
	rbtree_insert(base_ptr + 60, sizeof(int) * 20);
	rbtree_insert(base_ptr + 35, sizeof(int) * 10);
	rbtree_insert(base_ptr + 85, sizeof(int) * 5);
	rbtree_insert(base_ptr + 95, sizeof(int) * 5);
	rbtree_insert(base_ptr + 20, sizeof(int) * 5);
	rbtree_insert(base_ptr + 54, sizeof(int) * 6);

	printf("Tree after insertions:\n");
	rbtree_print();
	printf("\n");
}

void test_insert_2(int *base_ptr) {
	rbtree_insert(base_ptr + 30, sizeof(int) * 5);
	rbtree_insert(base_ptr + 70, sizeof(int) * 5);
	rbtree_insert(base_ptr + 180, sizeof(int) * 20);
	rbtree_insert(base_ptr + 100, sizeof(int) * 10);
	rbtree_insert(base_ptr + 220, sizeof(int) * 5);
	rbtree_insert(base_ptr + 80, sizeof(int) * 5);
	rbtree_insert(base_ptr + 110, sizeof(int) * 5);
	rbtree_insert(base_ptr + 260, sizeof(int) * 6);
	rbtree_insert(base_ptr + 20, sizeof(int) * 5);
	rbtree_insert(base_ptr + 60, sizeof(int) * 6);
	rbtree_insert(base_ptr + 130, sizeof(int) * 6);

	printf("Tree after insertions:\n");
	rbtree_print();
	printf("\n");
}

void test_point_search(int *base_ptr) {
	rbtree_node *searched_node = rbtree_node_search(base_ptr + 54);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}

	searched_node = rbtree_node_search(base_ptr + 20);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}

	searched_node = rbtree_node_search(base_ptr + 60);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}

	searched_node = rbtree_node_search(base_ptr + 47);
	if (searched_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", searched_node);
	}

	searched_node = rbtree_node_search(base_ptr + 40);
	if (searched_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", searched_node);
	}

	searched_node = rbtree_node_search(base_ptr + 92);
	if (searched_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", searched_node);
	}

	printf("Tree after regular search:\n");
	rbtree_print();
	printf("\n");
}

void test_interval_search(int *base_ptr) {
	printf("Testing interval_search()\n");
	rbtree_node *itvl_node = rbtree_interval_search(base_ptr + 54);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = rbtree_interval_search(base_ptr + 22);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = rbtree_interval_search(base_ptr + 60);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = rbtree_interval_search(base_ptr + 40);
	if (itvl_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", itvl_node);
	}
	itvl_node = rbtree_interval_search(base_ptr + 4);
	if (itvl_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", itvl_node);
	}
	itvl_node = rbtree_interval_search(base_ptr + 92);
	if (itvl_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", itvl_node);
	}

	printf("Tree after interval search:\n");
	rbtree_print();
	printf("\n");
}

void test_range_search(int *base_ptr) {
	printf("Testing range_search()\n");
	rbtree_node_search(base_ptr + 54)->free = 1;
	rbtree_node *range_node = rbtree_range_search(base_ptr + 52, sizeof(int) * 10);
	if (range_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", range_node);
	}
	rbtree_node_search(base_ptr + 35)->free = 1;
	range_node = rbtree_range_search(base_ptr + 30, sizeof(int) * 16);
	if (range_node == NULL) {
		printf("Failure! Not found\n");
	} else {
		printf("Success! Address: %p\n", range_node);
	}
	range_node = rbtree_range_search(base_ptr + 47, sizeof(int) * 5);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
	range_node = rbtree_range_search(base_ptr + 12, sizeof(int) * 5);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
	range_node = rbtree_range_search(base_ptr + 92, sizeof(int) * 10);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}
	range_node = rbtree_range_search(base_ptr + 18, sizeof(int) * 5);
	if (range_node == NULL) {
		printf("Success! Not found\n");
	} else {
		printf("Failure! Address: %p\n", range_node);
	}

	printf("Tree after range search:\n");
	rbtree_print();
	printf("\n");
}

void test_delete_node_1(int *base_ptr) {
	printf("Deleting 47\n");
	rbtree_delete_node(base_ptr + 47);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());
	
	printf("Deleting 60\n");
	rbtree_delete_node(base_ptr + 60);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());
	
	printf("Deleting 5\n");
	rbtree_delete_node(base_ptr + 5);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());
	
	printf("Deleting 5 AGAIN\n");
	rbtree_delete_node(base_ptr + 5);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());
}

void test_delete_node_2(int *base_ptr) {
	printf("Deleting 180\n");
	rbtree_delete_node(base_ptr + 180);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());

	printf("Deleting 110\n");
	rbtree_delete_node(base_ptr + 110);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());

	
	printf("Deleting 30\n");
	rbtree_delete_node(base_ptr + 30);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());

	
	printf("Deleting 100\n");
	rbtree_delete_node(base_ptr + 100);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());
	
	
	printf("Deleting 220\n");
	rbtree_delete_node(base_ptr + 220);
	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());

	rbtree_print();
	printf("Is red-black after deletion: %d\n", is_red_black_tree());
}


int main() {
	int temp[1000] = {0};
	int *base_ptr = temp;

	test_insert_1(base_ptr);
	printf("Is red-black after insertion: %d\n", is_red_black_tree());

	// Test searching
	test_point_search(base_ptr);
	test_interval_search(base_ptr);
	test_range_search(base_ptr);
	printf("Is red-black after search: %d\n", is_red_black_tree());
	
	// test deleting
	test_delete_node_1(base_ptr);

	return 0;
}
