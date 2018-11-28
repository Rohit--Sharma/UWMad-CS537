#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "gfg_rbtree.h"

#define LEFT_CHILD 0
#define RIGHT_CHILD 1

rbtree_node *root;

rbtree_node* uncle_node(rbtree_node *node) {
	if (node->parent == NULL || node->parent->parent == NULL)
		return NULL;
	
	if (node->parent == node->parent->parent->children[LEFT_CHILD])
		return node->parent->parent->children[RIGHT_CHILD];
	else
		return node->parent->parent->children[LEFT_CHILD];
}

rbtree_node* sibling_node(rbtree_node* node) {
	if (node->parent == NULL)
		return NULL;
	
	if (node == node->parent->children[LEFT_CHILD])
		return node->parent->children[RIGHT_CHILD];
	else
		return node->parent->children[LEFT_CHILD];
}

void move_node_down(rbtree_node* node, rbtree_node* new_parent) {
  	if (node->parent != NULL) {
		if (node == node->parent->children[LEFT_CHILD])
			node->parent->children[LEFT_CHILD] = new_parent;
		else
			node->parent->children[RIGHT_CHILD] = new_parent;
	}
	new_parent->parent = node->parent;
	node->parent = new_parent;
}

int node_has_red_child(rbtree_node* node) {
	return ((node->children[LEFT_CHILD]!=NULL && node->children[LEFT_CHILD]->red) || (node->children[RIGHT_CHILD]!=NULL && node->children[RIGHT_CHILD]->red));
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
			if (parent = parent->parent->children[LEFT_CHILD]) {
				if (node == node->parent->children[LEFT_CHILD]) {
					swap_colours_nodes(parent, grand_parent);
				}
				else {
					rotate_rbtree_nodes(parent, LEFT_CHILD);
					swap_colours_nodes(node,grand_parent);
				}
				rotate_rbtree_nodes(grand_parent, RIGHT_CHILD);
			}
			else {
				if (node == node->parent->children[LEFT_CHILD]) {
					rotate_rbtree_nodes(parent, RIGHT_CHILD);
					swap_colours_nodes(node, grand_parent);
				}
				else {
					swap_colours_nodes(parent, grand_parent);
				}
				rotate_rbtree_nodes(grand_parent, LEFT_CHILD);
			}
		}
	}
}

rbtree_node* successor_node(rbtree_node* node) {
	rbtree_node* temp_node = node;
	
	while(temp_node->children[LEFT_CHILD] != NULL)
		temp_node = temp_node->children[LEFT_CHILD];
 
	return temp_node;
}

rbtree_node* replace_BST_node(rbtree_node* node) {
	if (node->children[LEFT_CHILD] !=NULL && node->children[RIGHT_CHILD] != NULL)
		return successor_node(node->children[RIGHT_CHILD]);

	if (node->children[LEFT_CHILD] == NULL && node->children[RIGHT_CHILD] == NULL)
		return NULL;

	if (node->children[LEFT_CHILD]!=NULL)
		return node->children[LEFT_CHILD];
	else
		return node->children[RIGHT_CHILD];
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
			if (sibling == sibling->parent->children[LEFT_CHILD])
				rotate_rbtree_nodes(parent, RIGHT_CHILD);
			else
				rotate_rbtree_nodes(parent, LEFT_CHILD);
			fix_rbtree_double_black(node);
		}
		else {
			if ((sibling->children[LEFT_CHILD] != NULL && sibling->children[LEFT_CHILD]->red == 1) || (sibling->children[RIGHT_CHILD] != NULL && sibling->children[RIGHT_CHILD]->red == 1)) {
				if (sibling->children[LEFT_CHILD] != NULL && sibling->children[LEFT_CHILD]->red == 1) {
					if (sibling == sibling->parent->children[LEFT_CHILD]) {
			    		sibling->children[LEFT_CHILD]->red = sibling->red;
						sibling->red = parent->red;
						rotate_rbtree_nodes(parent, RIGHT_CHILD);
					}
					else {
						sibling->children[LEFT_CHILD]->red = parent->red;
						rotate_rbtree_nodes(sibling, RIGHT_CHILD);
						rotate_rbtree_nodes(parent, LEFT_CHILD);
					}
				} else {
					if (sibling == sibling->parent->children[RIGHT_CHILD]) {
						sibling->children[RIGHT_CHILD]->red = parent->red;
						rotate_rbtree_nodes(sibling, LEFT_CHILD);
						rotate_rbtree_nodes(parent, RIGHT_CHILD);
					} else {
						sibling->children[RIGHT_CHILD]->red = sibling->red;
						sibling->red = parent->red;
						rotate_rbtree_nodes(parent, LEFT_CHILD);
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
			
			if (node == node->parent->children[LEFT_CHILD])
				parent->children[LEFT_CHILD] = NULL;
			else
				parent->children[RIGHT_CHILD] = NULL;
		}
		free(node);
		return;
	}
	
	if (node->children[LEFT_CHILD] == NULL || node->children[RIGHT_CHILD] == NULL) {
		if (node == root) {
			node->ptr = temp_node->ptr;
			node->size = temp_node->size;
			node->free = temp_node->free;
			node->children[LEFT_CHILD] = node->children[RIGHT_CHILD] = NULL;
			free(temp_node);
		}
		else {
			if (node == node->parent->children[LEFT_CHILD])
				parent->children[LEFT_CHILD] = temp_node;
			else
				parent->children[RIGHT_CHILD] = temp_node;
		
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

  
rbtree_node* get_rbtree_root() {
	return root;
}

rbtree_node *search_for_node(void *ptr, rbtree_node *node) {
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
		if (node->children[LEFT_CHILD] != NULL) {
			return search_for_node(ptr, node->children[LEFT_CHILD]);
		}
	}
	else {
		if (node->children[RIGHT_CHILD] != NULL) {
			return search_for_node(ptr, node->children[RIGHT_CHILD]);
		}
	}
	return NULL;
}

rbtree_node *node_search_rbtree(void *ptr, rbtree_node *root) {
	if (root == NULL) {
		fprintf(stderr, "Root is NULL! No tree to search\n");
		exit(EXIT_FAILURE);
	}

	return search_for_node(ptr, root);
}

rbtree_node* create_rbtree_node(void *ptr, size_t size) {
	rbtree_node *new_node = (rbtree_node *) malloc(sizeof(rbtree_node));
	
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->free = 0;
	new_node->red = 1;
	new_node->parent = NULL;
	new_node->children[LEFT_CHILD] = NULL;
	new_node->children[RIGHT_CHILD] = NULL;

	return new_node;
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
		if (root->children[RIGHT_CHILD] != NULL) {
			return insert_node(ptr, size, root->children[RIGHT_CHILD], new_node);
		}
		else {
			root->children[RIGHT_CHILD] = new_node;
			new_node->parent = root;
			return 1;
		}
	}
	else if (root->ptr > ptr) {
		if (root->children[LEFT_CHILD] != NULL) {
			return insert_node(ptr, size, root->children[LEFT_CHILD], new_node);
		}
		else {
			root->children[LEFT_CHILD] = new_node;
			new_node->parent = root;
			return 1;
		}
	}

	return 0;
}

int is_still_rbtree(rbtree_node *new_node) {
	rbtree_node *temp_node;
	rbtree_node *grandparent;
	int child = 0;

	if ((new_node->parent == NULL) || (new_node->parent->red == 0))
		return 1;
	else {
		if (new_node->parent->parent == NULL) {
			return 1;
		}

		if ((new_node->parent->parent->children[LEFT_CHILD] != NULL) && (new_node->parent->parent->children[LEFT_CHILD]->red == 1) && (new_node->parent->parent->children[RIGHT_CHILD] != NULL) && (new_node->parent->parent->children[RIGHT_CHILD]->red == 1)) {
			new_node->parent->parent->children[LEFT_CHILD]->red = 0;
			new_node->parent->parent->children[RIGHT_CHILD]->red = 0;

			if (new_node->parent->parent->parent == NULL) 
				return 1;
			else {
				new_node->parent->parent->red = 1;
				return is_still_rbtree(new_node->parent->parent);
			}
		}

		if ((new_node->parent->parent->children[LEFT_CHILD] != NULL) && (new_node->parent->children[LEFT_CHILD] != NULL) && (new_node->parent->parent->children[LEFT_CHILD] == new_node->parent) && (new_node == new_node->parent->children[LEFT_CHILD])) {
			new_node->parent->red = 0;
			new_node->parent->parent->red = 1;
			grandparent = new_node->parent->parent;
			
			if (new_node->parent->parent->parent == NULL)
				root = new_node->parent;
			else
				new_node->parent->parent->parent->children[LEFT_CHILD] = new_node->parent;

			/*TODO Do not know if this next line is valid. maybe???*/
			new_node->parent->parent = new_node->parent->parent->parent;
			
			grandparent->children[LEFT_CHILD] = new_node->parent->children[RIGHT_CHILD];
			
			if (new_node->parent->children[RIGHT_CHILD] != NULL)
				new_node->parent->children[RIGHT_CHILD]->parent = grandparent;

			new_node->parent->children[RIGHT_CHILD] = grandparent;
			grandparent->parent = new_node->parent;
			
			return 1;
		}

		if ((new_node->parent->parent->children[RIGHT_CHILD] != NULL) && (new_node->parent->children[RIGHT_CHILD] != NULL) && (new_node->parent->parent->children[RIGHT_CHILD] == new_node->parent) && (new_node == new_node->parent->children[RIGHT_CHILD])) {
			new_node->parent->red = 0;
			new_node->parent->parent->red = 1;

			grandparent = new_node->parent->parent;

			if (new_node->parent->parent->parent == NULL)
				root = new_node->parent;
			else
				new_node->parent->parent->parent->children[RIGHT_CHILD] = new_node->parent;

			/*TODO Do not know if this next line is valid. maybe???*/
			new_node->parent->parent = new_node->parent->parent->parent;

			grandparent->children[RIGHT_CHILD] = new_node->parent->children[LEFT_CHILD];

			if (new_node->parent->children[LEFT_CHILD] != NULL)
				new_node->parent->children[LEFT_CHILD]->parent = grandparent;
			
			new_node->parent->children[LEFT_CHILD] = grandparent;
			grandparent->parent = new_node->parent;

			return 1;
		}

		if ((new_node->parent->children[RIGHT_CHILD] != NULL) && (new_node->parent->parent->children[LEFT_CHILD] != NULL) && (new_node == new_node->parent->children[RIGHT_CHILD]) && (new_node->parent->parent->children[LEFT_CHILD] == new_node->parent)) {
			new_node->red = 0;
			new_node->parent->parent->red = 1;
	
			temp_node = new_node->parent->parent->parent;

			if (new_node->parent->parent->parent->children[RIGHT_CHILD] == new_node->parent->parent)
				child = 1;

			new_node->parent->parent->children[LEFT_CHILD] = new_node->children[RIGHT_CHILD];
			new_node->children[RIGHT_CHILD] = new_node->parent->parent;
			new_node->children[RIGHT_CHILD]->parent = new_node;
			if (new_node->children[RIGHT_CHILD]->children[LEFT_CHILD] != NULL)
				new_node->children[RIGHT_CHILD]->children[LEFT_CHILD]->parent = new_node->children[RIGHT_CHILD];

			new_node->parent->children[RIGHT_CHILD] = new_node->children[LEFT_CHILD];
			new_node->children[LEFT_CHILD] = new_node->parent;
			new_node->children[LEFT_CHILD]->parent = new_node;
			if (new_node->children[LEFT_CHILD]->children[RIGHT_CHILD] != NULL)
				new_node->children[LEFT_CHILD]->children[RIGHT_CHILD]->parent = new_node->children[LEFT_CHILD];

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


		if ((new_node->parent->children[LEFT_CHILD] != NULL) && (new_node->parent->parent->children[RIGHT_CHILD] != NULL) && (new_node == new_node->parent->children[LEFT_CHILD]) && (new_node->parent->parent->children[RIGHT_CHILD] == new_node->parent)) {
			new_node->red = 0;
			new_node->parent->parent->red = 1;
	
			temp_node = new_node->parent->parent->parent;

			if (new_node->parent->parent->parent->children[RIGHT_CHILD] == new_node->parent->parent)
				child = 1;

			new_node->parent->parent->children[RIGHT_CHILD] = new_node->children[LEFT_CHILD];
			new_node->children[LEFT_CHILD] = new_node->parent->parent;
			new_node->children[LEFT_CHILD]->parent = new_node;
			if (new_node->children[LEFT_CHILD]->children[RIGHT_CHILD] != NULL)
				new_node->children[LEFT_CHILD]->children[RIGHT_CHILD]->parent = new_node->children[LEFT_CHILD];

			new_node->parent->children[LEFT_CHILD] = new_node->children[RIGHT_CHILD];
			new_node->children[RIGHT_CHILD] = new_node->parent;
			new_node->children[RIGHT_CHILD]->parent = new_node;
			if (new_node->children[RIGHT_CHILD]->children[LEFT_CHILD] != NULL)
				new_node->children[RIGHT_CHILD]->children[LEFT_CHILD]->parent = new_node->children[RIGHT_CHILD];

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

int rbtree_insert_node(void *ptr, size_t size) {
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

rbtree_node* interval_search_rbtree(void *ptr, rbtree_node *root) {
	if (root == NULL) {
		return NULL;
	}

	if ((ptr == root->ptr) && (root->free == 0))
		return root;
	else if (ptr >= root->ptr) {
		if ((size_t)ptr <= ((size_t)root->ptr + root->size) && (root->free == 0))
			return root;
		else {
			if (root->children[RIGHT_CHILD] != NULL) {
				rbtree_node *search_right_side = interval_search_rbtree(ptr, root->children[RIGHT_CHILD]);
				if (search_right_side != NULL)
					return search_right_side;
			}
			if (root->children[LEFT_CHILD] != NULL) {
				rbtree_node *search_left_side = interval_search_rbtree(ptr, root->children[LEFT_CHILD]);
				if (search_left_side != NULL)
					return search_left_side;
			}
		}
	}
	else {
		return interval_search_rbtree(ptr, root->children[LEFT_CHILD]);
	}
	
	return NULL;
}

rbtree_node* range_search_rbtree(void *ptr, size_t size, rbtree_node *root) {
	
	if (root == NULL)
		return NULL;
	else if (root->ptr > ptr && ((size_t)root->ptr + root->size) < ((long)ptr + size) && (root->free == 1)) {
		return root;
	} else {
		rbtree_node* left_side_search = NULL; 
		rbtree_node* right_side_search = NULL;

		if (root->children[LEFT_CHILD] != NULL) {
			left_side_search = range_search_rbtree(ptr, size, root->children[LEFT_CHILD]);
			if (left_side_search != NULL)
				return left_side_search;
		}
 
		if (root->children[RIGHT_CHILD] != NULL) {
			right_side_search = range_search_rbtree(ptr, size, root->children[RIGHT_CHILD]);
			if (right_side_search != NULL)
				return right_side_search;
		}
	}
	
	return NULL;
}

/**
 * The helper method used by rbtree_print
 * :param root: The root of the red black tree to be printed
 * :param depth: The recursive depth parameter to print the .'s
 */
void print_helper(rbtree_node *root, int depth, rbtree_node *gl_root) {
	if (root != NULL) {
		print_helper(root->children[LEFT_CHILD], depth + 1, gl_root);

		int curr_depth = depth;
		printf("Current Depth is %d\n", curr_depth);
		while (curr_depth--) {
			printf(".");
		}
		// printf("red = %d, free = %d node at %p with ptr %p size %i, parent %p, and children %p %p\n", root->red, root->free, (void *)root, root->ptr, (int)root->size, (void *)root->parent, (void *)root->children[LEFT_CHILD], (void *)root->children[RIGHT_CHILD]);
		printf("%p - ptr: %d R: %d, F: %d Children: %p %p\n", (void *)root, (root->ptr - gl_root->ptr) / 4, root->red, root->free, (void *)root->children[LEFT_CHILD], (void *)root->children[RIGHT_CHILD]);

		print_helper(root->children[RIGHT_CHILD], depth + 1, gl_root);
	}
}

/**
 * This method prints the red black tree rooted at global "root" param
 */
void rbtree_print() {
	// Get the left most node so that the addresses printed are relative and easier to make sense
	rbtree_node *temp = root;
	while (temp->children[LEFT_CHILD] != NULL) {
		temp = temp->children[LEFT_CHILD];
	}

	print_helper(root, 0, temp);
}

// Returns the number of black nodes in a subtree of the given node
// or -1 if it is not a red black tree.
int computeBlackHeight(rbtree_node* curr_node) {
    // For an empty subtree the answer is obvious
    if (curr_node == NULL)
        return 0; 
    // Computes the height for the left and right child recursively
    int leftHeight = computeBlackHeight(curr_node->children[LEFT_CHILD]);
    int rightHeight = computeBlackHeight(curr_node->children[RIGHT_CHILD]);
    int add = curr_node->red == 0 ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
        return -1; 
    else
        return leftHeight + add;
}

int checkNoTwoAdjacentRedNodes(rbtree_node *curr_node) {
	if (curr_node == NULL) {
		return 1;
	}

	if (curr_node->red == 1) {
		if (curr_node->children[LEFT_CHILD] != NULL && curr_node->children[LEFT_CHILD]->red == 1) {
			return 0;
		}
		if (curr_node->children[RIGHT_CHILD] != NULL && curr_node->children[RIGHT_CHILD]->red == 1) {
			return 0;
		}
	}

	return checkNoTwoAdjacentRedNodes(curr_node->children[LEFT_CHILD]) && checkNoTwoAdjacentRedNodes(curr_node->children[RIGHT_CHILD]);
}

int isBST(rbtree_node *curr_node, rbtree_node *left_node, rbtree_node *right_node) {
	// Base condition 
    if (curr_node == NULL) 
        return 1; 
  
    // if left node exist then check it has 
    // correct data or not i.e. left node's data 
    // should be less than root's data 
    if (left_node != NULL && curr_node->ptr < left_node->ptr) 
        return 0; 
  
    // if right node exist then check it has 
    // correct data or not i.e. right node's data 
    // should be greater than root's data 
    if (right_node != NULL && curr_node->ptr > right_node->ptr) 
        return 0; 
  
    // check recursively for every node. 
    return isBST(curr_node->children[LEFT_CHILD], left_node, curr_node) && 
           isBST(curr_node->children[RIGHT_CHILD], curr_node, right_node);
}

/**
 * Returns 1 if red black tree properties are followed by the tree rooted at root, else 0
 * Rules:
 * 1) Every node has a color either red or black.
 * 2) Root of tree is always black.
 * 3) There are no two adjacent red nodes (A red node cannot have a red parent or red child).
 * 4) Every path from a node (including root) to any of its descendant NULL node has the same number of black nodes.
 */
int isRedBlackTree() {
	// Check BST
	if (isBST(root, NULL, NULL) == 0) {
		fprintf(stderr, "The red-black tree is not a binary SEARCH tree.\n");
		return 0;
	}

	// Rule 2
	if (root->red == 1) {
		fprintf(stderr, "Root of a red-black tree must be Black.\n");
		return 0;
	}

	// Rule 3
	if (checkNoTwoAdjacentRedNodes(root) == 0) {
		fprintf(stderr, "A red node cannot have a red child in a red-black tree.\n");
		return 0;
	}

	// Rule 4
	if (computeBlackHeight(root) == -1) {
		fprintf(stderr, "The black height of left subtree and right subtree must be the same in a red-black tree.\n");
		return 0;
	}

	return 1;
}
