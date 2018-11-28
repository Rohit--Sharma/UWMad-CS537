#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "gfg_rbtree.h"  
#define LEFT 0
#define RIGHT 1

rbtree_node *root;

rbtree_node* uncle_node(rbtree_node *node) {
	if (node->parent == NULL || node->parent->parent == NULL)
		return NULL;
	
	if (node->parent == node->parent->parent->children[LEFT])
		return node->parent->parent->children[RIGHT];
	else
		return node->parent->parent->children[LEFT];
}

rbtree_node* sibling_node(rbtree_node* node) {
	if (node->parent == NULL)
		return NULL;
	
	if (node == node->parent->children[LEFT])
		return node->parent->children[RIGHT];
	else
		return node->parent->children[LEFT];
}

void move_node_down(rbtree_node* node, rbtree_node* new_parent) {
  	if (node->parent != NULL) {
		if (node == node->parent->children[LEFT])
			node->parent->children[LEFT] = new_parent;
		else
			node->parent->children[RIGHT] = new_parent;
	}
	new_parent->parent = node->parent;
	node->parent = new_parent;
}

int node_has_red_child(rbtree_node* node) {
	return ((node->children[LEFT]!=NULL && node->children[LEFT]->red) || (node->children[RIGHT]!=NULL && node->children[RIGHT]->red));
}

void rotate_rbtree_nodes(rbtree_node* node, int direction) {
	rbtree_node *temp_node = node->children[(direction+1)%2];
	
	if (node == root)
		root = temp_node;

	move_node_down(node, temp_node);
	node->children[(direction+1)%2] = temp_node->children[direction];
	
	if (temp_node->children[direction] != NULL)
		temp_node->children[direction]->parent = node;

	temp_node->children[direction] = node;
}

void swap_colours_nodes(rbtree_node* node1, rbtree_node* node2) {
	int temp_red;
	temp_red = node1->red;
	node1->red = node2->red;
	node2->red = temp_red;
}

void swap_values_nodes(rbtree_node* node1, rbtree_node* node2) {
	void *temp_ptr;
	temp_ptr = node1->ptr;
	node1->ptr = node2->ptr;
	node2->ptr = temp_ptr;

	size_t temp_size;
	temp_size = node1->size;
	node1->size = node2->size;
	node2->size = temp_size;

	int temp_free;
	temp_free = node1->free;
	node1->free = node2->free;
	node2->free = temp_free;
}

void red_red_node(rbtree_node* node) {
	if (node == root) {
		node->red = 0;
		return;
	}
	
	rbtree_node *parent = node->parent;
	rbtree_node *grand_parent = parent->parent;
	rbtree_node *uncle = uncle_node(node);

	if (parent->red == 1) {
		if (uncle != NULL && uncle->red == 1) {
			parent->red = 0;
			uncle->red = 0;
			grand_parent->red = 1;
			red_red_node(grand_parent);
		} 
		else {
			if (parent = parent->parent->children[LEFT]) {
				if (node == node->parent->children[LEFT]) {
					swap_colours_nodes(parent, grand_parent);
				}
				else {
					rotate_rbtree_nodes(parent, LEFT);
					swap_colours_nodes(node,grand_parent);
				}
				rotate_rbtree_nodes(grand_parent, RIGHT);
			}
			else {
				if (node == node->parent->children[LEFT]) {
					rotate_rbtree_nodes(parent, RIGHT);
					swap_colours_nodes(node, grand_parent);
				}
				else {
					swap_colours_nodes(parent, grand_parent);
				}
				rotate_rbtree_nodes(grand_parent, LEFT);
			}
		}
	}
}

rbtree_node* successor_node(rbtree_node* node) {
	rbtree_node* temp_node = node;
	
	while(temp_node->children[LEFT] != NULL)
		temp_node = temp_node->children[LEFT];
 
	return temp_node;
}

rbtree_node* replace_BST_node(rbtree_node* node) {
	if (node->children[LEFT] !=NULL && node->children[RIGHT] != NULL)
		return successor_node(node->children[RIGHT]);

	if (node->children[LEFT] == NULL && node->children[RIGHT == NULL])
		return NULL;

	if (node->children[LEFT]!=NULL)
		return node->children[LEFT];
	else
		return node->children[RIGHT];
}

void delete_rbtree_node(rbtree_node *node) {
	rbtree_node *temp_node;
	temp_node = replace_BST_node(node);
	int both_black_nodes;
	
	if ((temp_node == NULL || temp_node->red == 0) && (node->red == 0))
		both_black_nodes = 1;
	
	rbtree_node *parent = node->parent;

	if (temp_node == NULL) {
		if (node == root)
			root = NULL;
		else {
			if (both_black_nodes) {
				fix_rbtree_double_black(node);
			}
			else {
				rbtree_node* sibling;
				sibling = sibling_node(node);
				if (sibling != NULL)
					sibling->red = 1;
			}
			
			if (node == node->parent->children[LEFT])
				parent->children[LEFT] = NULL;
			else
				parent->children[RIGHT] = NULL;
		}
		free(node);
		return;
	}
	
	if (node->children[LEFT] == NULL || node->children[RIGHT] == NULL) {
		if (node == root) {
			node->ptr = temp_node->ptr;
			node->size = temp_node->size;
			node->free = temp_node->free;
			node->children[LEFT] = node->children[RIGHT] = NULL;
			free(temp_node);
		}
		else {
			if (node == node->parent->children[LEFT])
				parent->children[LEFT] = temp_node;
			else
				parent->children[RIGHT] = temp_node;
		
			free(node);
			temp_node->parent = parent;
			if (both_black_nodes)
				fix_rbtree_double_black(temp_node);
			else
				temp_node->red = 0;
		}
		return;
	}

  	swap_values_nodes(temp_node, node);
	free(temp_node);
}

void fix_rbtree_double_black(rbtree_node* node) {
	if (node == root)
		return;

	rbtree_node* sibling = sibling_node(node);
	rbtree_node* parent = node->parent;
	if (sibling == NULL)
		fix_rbtree_double_black(parent);
	else {
		if (sibling->red == 1) {
			parent->red = 1;
			sibling->red = 0;
			if (sibling == sibling->parent->children[LEFT])
				rotate_rbtree_nodes(parent, RIGHT);
			else
				rotate_rbtree_nodes(parent, LEFT);
			fix_rbtree_double_black(node);
		}
		else {
			if ((sibling->children[LEFT] != NULL && sibling->children[LEFT]->red == 1) || (sibling->children[RIGHT] != NULL && sibling->children[RIGHT]->red == 1)) {
				if (sibling->children[LEFT] != NULL && sibling->children[LEFT]->red == 1) {
					if (sibling == sibling->parent->children[LEFT]) {
			    		sibling->children[LEFT]->red = sibling->red;
						sibling->red = parent->red;
						rotate_rbtree_nodes(parent, RIGHT);
					}
					else {
						sibling->children[LEFT]->red = parent->red;
						rotate_rbtree_nodes(sibling, RIGHT);
						rotate_rbtree_nodes(parent, LEFT);
					}
				} else {
					if (sibling == sibling->parent->children[RIGHT]) {
						sibling->children[RIGHT]->red = parent->red;
						rotate_rbtree_nodes(sibling, LEFT);
						rotate_rbtree_nodes(parent, RIGHT);
					} else {
						sibling->children[RIGHT]->red = sibling->red;
						sibling->red = parent->red;
						rotate_rbtree_nodes(parent, LEFT);
					}
				}
				parent->red = 0;
			} else {
  				sibling->red = 1;
				if (parent->red == 0)
					fix_rbtree_double_black(parent);
				else
					parent->red = 0;
			}
		}
	}
}

  
rbtree_node* get_rbtree_root() {
	return root;
}

rbtree_node* search_for_point_node(void *ptr, rbtree_node *node) {
	if (node == NULL) {
		return NULL;
	}
	else if (node->ptr == NULL) {
		printf("Encountered a bad node\n");
		return NULL;
	}
	else if ((node->ptr == ptr)) {
		return node;
	}
	else if (node->ptr > ptr) {
		if (node->children[LEFT] != NULL) {
			return search_for_point_node(ptr, node->children[LEFT]);
		}
	}
	else {
		if (node->children[RIGHT] != NULL) {
			return search_for_point_node(ptr, node->children[RIGHT]);
		}
	}
	return NULL;
}

rbtree_node* create_rbtree_node(void *ptr, size_t size) {
	rbtree_node* new_node = (rbtree_node *) malloc(sizeof(rbtree_node));
	
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->free = 0;
	new_node->red = 1;
	new_node->parent = NULL;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;

	return new_node;
}

int rbtree_insert_node(void* ptr, size_t size) {
	int insert_successful;
	int rbtree_intact;

	rbtree_node *new_node = create_rbtree_node(ptr, size);

	if (root == NULL) {
		root = new_node;
		root->red = 0;
		return 1;
	}

	insert_successful = insert_node(ptr, size, root, new_node);
	if (insert_successful < 0) {
		fprintf(stderr, "Could not insert node to red black tree! Exiting program...\n");
		return insert_successful;
	}

	rbtree_intact = is_still_rbtree(new_node);
	if (rbtree_intact < 0) {
		fprintf(stderr, "Error as the tree is no longer red black after insertion!\n");
		return rbtree_intact;
	}
	
	return 1;
}

int insert_node(void *ptr, size_t size, rbtree_node *root, rbtree_node *new_node) {
	
	if (root->ptr == ptr) {
		if (root->free) {
			root->size = size;
			root->free = 0;
			return 1;
		}
		else {
			fprintf(stderr, "Error on malloc537\nNode with address %p already exists and cannot be allocated there.\n", ptr);
			return -1;
		}
	}
	else if (root->ptr < ptr) {
		if (root->children[RIGHT] != NULL) {
			return insert_node(ptr, size, root->children[RIGHT], new_node);
		}
		else {
			root->children[RIGHT] = new_node;
			new_node->parent = root;
			return 1;
		}
	}
	else if (root->ptr > ptr) {
		if (root->children[LEFT] != NULL) {
			return insert_node(ptr, size, root->children[LEFT], new_node);
		}
		else {
			root->children[LEFT] = new_node;
			new_node->parent = root;
			return 1;
		}
	}

	return 0;
}

int is_still_rbtree(rbtree_node *new_node) {
	rbtree_node *temp_node;
	rbtree_node *grandparent;
	int child = LEFT;

	if ((new_node->parent == NULL) || (new_node->parent->red == 0))
		return 1;
	else {
		if (new_node->parent->parent == NULL) {
			return 1;
		}

		if ((new_node->parent->parent->children[LEFT] != NULL) && (new_node->parent->parent->children[LEFT]->red) && (new_node->parent->parent->children[RIGHT] != NULL) && (new_node->parent->parent->children[RIGHT]->red)) {
			new_node->parent->parent->children[LEFT]->red = 0;
			new_node->parent->parent->children[RIGHT]->red = 0;

			if (new_node->parent->parent->parent == NULL) 
				return 1;
			else {
				new_node->parent->parent->red = 1;
				return is_still_rbtree(new_node->parent->parent);
			}
		}

		if ((new_node->parent->parent->children[LEFT] != NULL) && (new_node->parent->children[LEFT] != NULL) && (new_node->parent->parent->children[LEFT] == new_node->parent) && (new_node == new_node->parent->children[LEFT])) {
			new_node->parent->red = 0;
			new_node->parent->parent->red = 1;
			grandparent = new_node->parent->parent;
			
			if (new_node->parent->parent->parent == NULL)
				root = new_node->parent;
			else
				new_node->parent->parent->parent->children[LEFT] = new_node->parent;

			/*TODO Do not know if this next line is valid. maybe???*/
			new_node->parent->parent = new_node->parent->parent->parent;
			
			grandparent->children[LEFT] = new_node->parent->children[RIGHT];
			
			if (new_node->parent->children[RIGHT] != NULL)
				new_node->parent->children[RIGHT]->parent = grandparent;

			new_node->parent->children[RIGHT] = grandparent;
			grandparent->parent = new_node->parent;
			
			return 1;
		}

		if ((new_node->parent->parent->children[RIGHT] != NULL) && (new_node->parent->children[RIGHT] != NULL) && (new_node->parent->parent->children[RIGHT] == new_node->parent) && (new_node == new_node->parent->children[RIGHT])) {
			new_node->parent->red = 0;
			new_node->parent->parent->red = 1;

			grandparent = new_node->parent->parent;

			if (new_node->parent->parent->parent == NULL)
				root = new_node->parent;
			else
				new_node->parent->parent->children[RIGHT] = new_node->parent;

			/*TODO Do not know if this next line is valid. maybe???*/
			new_node->parent->parent = new_node->parent->parent->parent;

			grandparent->children[RIGHT] = new_node->parent->children[LEFT];

			if (new_node->parent->children[LEFT] != NULL)
				new_node->parent->children[LEFT]->parent = grandparent;
			
			new_node->parent->children[LEFT] = grandparent;
			grandparent->parent = new_node->parent;

			return 1;
		}

		if ((new_node->parent->children[RIGHT] != NULL) && (new_node->parent->parent->children[LEFT] != NULL) && (new_node == new_node->parent->children[RIGHT]) && (new_node->parent->parent->children[LEFT] == new_node->parent)) {
			new_node->red = 0;
			new_node->parent->parent->red = 1;
	
			temp_node = new_node->parent->parent->parent;

			if (new_node->parent->parent->parent->children[RIGHT] == new_node->parent->parent)
				child = RIGHT;

			new_node->parent->parent->children[LEFT] = new_node->children[RIGHT];
			new_node->children[RIGHT] = new_node->parent;
			new_node->children[RIGHT]->parent = new_node;
			if (new_node->children[RIGHT]->children[LEFT] != NULL)
				new_node->children[RIGHT]->children[LEFT]->parent = new_node->children[RIGHT];

			new_node->parent->children[RIGHT] = new_node->children[LEFT];
			new_node->children[LEFT] = new_node->parent;
			new_node->children[LEFT]->parent = new_node;
			if (new_node->children[LEFT]->children[RIGHT] != NULL)
				new_node->children[LEFT]->children[RIGHT]->parent = new_node->children[LEFT];

			if (temp_node == NULL) {
				new_node->parent = NULL;
				root = new_node;
			}

			if (new_node != NULL) {
				new_node->parent = temp_node;
				temp_node->children[child] = new_node;
			}
			
			return 1;
		}


		if ((new_node->parent->children[LEFT] != NULL) && (new_node->parent->parent->children[RIGHT] != NULL) && (new_node == new_node->parent->children[LEFT]) && (new_node->parent->parent->children[RIGHT] == new_node->parent)) {
			new_node->red = 0;
			new_node->parent->parent->red = 1;
	
			temp_node = new_node->parent->parent->parent;

			if (new_node->parent->parent->parent->children[RIGHT] == new_node->parent->parent)
				child = RIGHT;

			new_node->parent->parent->children[RIGHT] = new_node->children[LEFT];
			new_node->children[LEFT] = new_node->parent;
			new_node->children[LEFT]->parent = new_node;
			if (new_node->children[LEFT]->children[RIGHT] != NULL)
				new_node->children[LEFT]->children[RIGHT]->parent = new_node->children[LEFT];

			new_node->parent->children[LEFT] = new_node->children[RIGHT];
			new_node->children[RIGHT] = new_node->parent;
			new_node->children[RIGHT]->parent = new_node;
			if (new_node->children[RIGHT]->children[LEFT] != NULL)
				new_node->children[RIGHT]->children[LEFT]->parent = new_node->children[RIGHT];

			if (temp_node == NULL) {
				new_node->parent = NULL;
				root = new_node;
			}

			if (new_node != NULL) {
				new_node->parent = temp_node;
				temp_node->children[child] = new_node;
			}
			
			return 1;
		}
	}

	return 0;
}

rbtree_node* interval_search_rbtree(void* ptr, rbtree_node* root) {
	if (root == NULL) {
		return NULL;
	}

	if ((ptr == root->ptr) && (root->free != 0))
		return root;
	else if (ptr >= root->ptr) {
		if ((size_t)ptr <= ((size_t)root->ptr + root->size) && (root->free != 0))
			return root;
		else {
			if (root->children[RIGHT] != NULL) {
				rbtree_node *search_right_side = interval_search_rbtree(ptr, root->children[RIGHT]);
				if (search_right_side != NULL)
					return search_right_side;
			}
			if (root->children[LEFT] != NULL) {
				rbtree_node *search_left_side = interval_search_rbtree(ptr, root->children[LEFT]);
				if (search_left_side != NULL)
					return search_left_side;
			}
		}
	}
	else {
		return interval_search_rbtree(ptr, root->children[LEFT]);
	}
	
	return NULL;
}

rbtree_node* range_search_rbtree(void* ptr, size_t size, rbtree_node* root) {
	
	if (root == NULL)
		return NULL;
	else if (root->ptr > ptr && ((size_t)root->ptr + root->size) < ((long)ptr + size) && (root->free == 1)) {
		return root;
	} else {
		rbtree_node* left_side_search = NULL; 
		rbtree_node* right_side_search = NULL;

		if (root->children[LEFT] != NULL) {
			left_side_search = range_search_rbtree(ptr, size, root->children[LEFT]);
			if (left_side_search != NULL)
				return left_side_search;
		}
 
		if (root->children[RIGHT] != NULL) {
			right_side_search = range_search_rbtree(ptr, size, root->children[RIGHT]);
			if (right_side_search != NULL)
				return right_side_search;
		}
	}
	
	return NULL;
}

int main() {
	return 1;
}
