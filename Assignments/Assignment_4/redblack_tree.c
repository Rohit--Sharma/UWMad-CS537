///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      November 24, 2018
//
// Resources:		http://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html
//					https://www.geeksforgeeks.org/red-black-tree-set-1-introduction-2/
//					https://www.geeksforgeeks.org/red-black-tree-set-2-insert/
//					https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
//
///////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "redblack_tree.h"

/**
 * Constants used for accessing left and right child of red black tree
 */
#define LEFT_CHILD 0
#define RIGHT_CHILD 1

/**
 * This is the root of our tree!
 * Or maybe this should be in the malloc file???
 */
RBNode *root;

/**
 * Creates a new red black tree node.
 * Defaults: 
 * 	free: 0, color: red
 * :param ptr: The starting address of the memory location allocated
 * :param size: The size of memory allocated
 * :return: A new RBNode
 */
RBNode *create_RBNode(void *ptr, size_t size) {
	RBNode *new_node = (RBNode *) malloc(sizeof(RBNode));

	new_node->ptr = ptr;
	new_node->size = size;

	new_node->red = 1;
	new_node->free = 0;
	new_node->parent = NULL;
	new_node->children[LEFT_CHILD] = NULL;
	new_node->children[RIGHT_CHILD] = NULL;

	return new_node;
}

/**
 * Swaps the parent of old node with the parent of the new node
 * :param old: The red black tree node whose parent is to be swapped
 * :param new: The red black tree node whose parent is to be swapped with
 */
void replace_rbtree_node(RBNode *old, RBNode *new) {
	if (old->parent == NULL) {
		root = new;
	}
	else {
		if (old == old->parent->children[LEFT_CHILD]) {
			old->parent->children[LEFT_CHILD] = new;
		}
		else {
			old->parent->children[RIGHT_CHILD] = new;
		}
	}

	if (new != NULL) {
		new->parent = old->parent;
	}
}

/**
 * Red black tree rotation function.
 * :param node: The reference node from which the rotation is to be made
 * :param direction: LEFT_CHILD or RIGHT_CHILD. Rotates in the corresponding direction
 */
void rotate_rbnodes(RBNode *node, int direction) {
	RBNode *temp = node->children[(direction + 1) % 2];
	replace_rbtree_node(node, temp);
	node->children[(direction + 1) % 2] = temp->children[direction];
	if (temp->children[direction] != NULL) {
		temp->children[direction]->parent = node;
	}
	temp->children[direction] = node;
	node->parent = temp;
}

/**
 * The helper method used by the rbtree_point_search() method
 * :param ptr: The address that is to be searched
 * :param parent: The root of the red black tree in which ptr is to be searched
 * :return: The RBNode that starts with ptr in the tree
 */
RBNode *point_search_helper(void *ptr, RBNode *parent) {
	/*
	 * return the parent node if it's within the given ptr
	 * and size!
	 */

	/* Sanity check. We shouldn't get a null node, but if we do, we can't find the node! */
	if (parent == NULL) {
		return NULL;
	}
	else if (parent->ptr == NULL) {
		printf("bad node encountered.");
		return NULL;
	}
	else if ((parent->ptr == ptr)) {
		return parent;
	}
	/*
	 * Call on the left (smaller) child if this node's ptr is too big!
	 */
	else if (parent->ptr > ptr) {
		if (parent->children[LEFT_CHILD] != NULL) {
			return point_search_helper(ptr, parent->children[LEFT_CHILD]);
		}
	}
	/* 
	 * Call on right (larger) child if this node's ptr is too small!
	 */
	else {
		if (parent->children[RIGHT_CHILD] != NULL) {
			return point_search_helper(ptr, parent->children[RIGHT_CHILD]);
		}
	}

	/*
	 * If we get here, the node doesn't exist!
	 */
	return NULL;
}

/**
 * Searches a node in the red black tree whose ptr 
 * 	matches the argument ptr provided.
 * :param ptr: The address that is to be searched
 * :return: The red black tree node that starts with ptr in the tree
 * 				or NULL if not found
 */
RBNode *rbtree_point_search(void *ptr) {
	/*
	 * Just calls the recursive function
	 * on the root of the tree.
	 */
	if (root == NULL) {
		fprintf(stderr, "root is null\n");
		exit(EXIT_FAILURE);
	}

	return point_search_helper(ptr, root);
}

/**
 * The helper method used by the rbtree_interval_search() method
 * 		The inteval is defined by (ptr, ptr + size)
 * :param ptr: The address that is to be searched for in an interval
 * :param parent: The root of the red black tree
 * :return: The RBNode in the red black tree whose interval contains ptr
 */
RBNode *interval_search_helper(void *ptr, RBNode *parent) {
	/*
	 * If our ptr equals the parent's ptr, return the parent.
	 */
	if (ptr == parent->ptr && !(parent->free)) {
		return parent;
	}
	/*
	 * If the parent's ptr is smaller, check if we're inside the parent's space!
	 * Return the parent if we are.
	 * Otherwise, continue our search.
	 */
	else if (ptr >= parent->ptr) {
		if ((size_t)ptr <= ((size_t)parent->ptr + parent->size) && !(parent->free)) {
			return parent;
		}
		else {
			/*
			 * If the ptr is smaller and it's not contained,
			 */

			RBNode *left_search = NULL;
			RBNode *right_search = NULL;
			if (parent->children[LEFT_CHILD] != NULL)
				left_search = interval_search_helper(ptr, parent->children[LEFT_CHILD]);

			if (parent->children[RIGHT_CHILD] != NULL)
				right_search = interval_search_helper(ptr, parent->children[RIGHT_CHILD]);

			if (right_search != NULL) {
				return right_search;
			}
			else {
				return left_search;
			}
		}
	}
	/*
	 * If the parent's ptr is bigger, just keep on looking.
	 */
	else {
		return interval_search_helper(ptr, parent->children[LEFT_CHILD]);
	}
}

/**
 * Searches an allocated node in the red black tree whose interval 
 * 	contains the given ptr. Ignores the freed nodes.
 * 		The inteval is defined by (ptr, ptr + size)	// ptr refers to node->ptr
 * :param ptr: The address that is to be searched for in an interval
 * :return: The RBNode in the red black tree whose interval contains ptr
 */
RBNode *rbtree_interval_search(void *ptr) {
	return interval_search_helper(ptr, root);
}

/**
 * The helper method used by the rbtree_range_search() method
 * 		The inteval is defined by (ptr, ptr + size)
 * :param ptr: The address that is to be searched for in an interval
 * :param size: The size of the interval pointed by ptr
 * :param parent: The root of the red black tree
 * :return: The RBNode in the red black tree whose interval contains the interval (ptr, ptr + size)
 */
RBNode *range_search_helper(void *ptr, size_t size, RBNode *parent) {
	/*
	 * If we somehow get a null node, return NULL.
	 */
	if (parent == NULL)
		return NULL;
	/*
	 * If the current node's ptr is too small, look at the right child.
	 */
	/*
	else if(((long)(parent->ptr) + parent->size) < (long)ptr)
	{
		
		if(parent->children[RIGHT_CHILD] != NULL)
		return range_search_helper(ptr, size, parent->children[RIGHT_CHILD]);
		else
		return NULL;
	}
	*/
	/*
	 * If the current node's ptr is too big, look at the left child.
	 */
	/*
	else if(((long)(ptr) + size) < (long)parent->ptr)
	{
		if(parent->children[LEFT_CHILD] != NULL)
		return range_search_helper(ptr, size, parent->children[LEFT_CHILD]);
		else
		return NULL;
	}
	*/
	/*
	 * If our node's ptr is in range, check the size and return if it's small enough and free.
	 * Otherwise, check both children if the exist.
	 */
	else if (parent->ptr > ptr && ((long)parent->ptr + parent->size) < ((long)ptr + size) && (parent->free == 1)) {
		return parent;
	}
	/*
	 * Here we search both children, and return one of the results.
	 * We prefer the left child arbitrarily.
	 */
	else {
		RBNode *left_return = NULL;
		RBNode *right_return = NULL;

		if (parent->children[LEFT_CHILD] != NULL)
			left_return = range_search_helper(ptr, size, parent->children[LEFT_CHILD]);

		if (parent->children[RIGHT_CHILD] != NULL)
			right_return = range_search_helper(ptr, size, parent->children[RIGHT_CHILD]);

		if (left_return != NULL)
			return left_return;
		else
			return right_return;
	}
}

/**
 * Searches for a node in the red black tree whose interval 
 * 	contains the interval (ptr, ptr + size).
 * 		The inteval is defined by (ptr, ptr + size)	// ptr refers to node->ptr
 * :param ptr: The starting address of the interval
 * :param size: The size of the interval pointed by ptr
 * :return: The RBNode in the red black tree whose interval contains the interval (ptr, ptr + size)
 */
RBNode *rbtree_range_search(void *ptr, size_t size) {
	return range_search_helper(ptr, size, root);
}

/**
 * This method ensures that the red black tree properties are satisfied after an insert call.
 * :param child: The node from where the red black tree properties are to be restored
 * :return:
 */
int rbtreeify(RBNode *child) {
	RBNode *tempNode;
	RBNode *tempGparent;
	int whichChild = 0;
	/*
	 * And here's the fun part!
	 * Now we check for a violation of red-black tree properties!
	 * Because we always insert red nodes, we only need to check
	 * for violations of one of the properties!
	 *
	 * Modeled on CS 367 lecture notes
	 */

	/*
	 * If the child's parent is black, we're fine!
	 */

	if ((child->parent == NULL) || !(child->parent->red)) {
		return 1;
	}
	else {
		/*	printf("red = %d node at %p with ptr %p size %i, parent %p, and children %p %p\n",root->red, (void *)root,root->ptr, (int)root->size, (void *)root->parent, (void *)root->children[LEFT_CHILD], (void *)root->children[RIGHT_CHILD]);

		 * If the child's parent and the parent's sibling is red, just change their color to black and
		 * change the grandparent to red. If the grandparent is the root node, then just keep it
		 * as black to keep the red/black properties
	 	*/

		/*
		 * We need to handle insertion when the depth is low - i.e. we don't have enough nodes
		 * to actually have grandparents/great grandparents.
		 */

		/*NEED TO CHECK IF A CHILD/PARENT EXISTS BEFORE USING IT!*/

		if (child->parent->parent == NULL) {
			return 1;
		}

		/* if the grandparent has two children, and they're both red*/
		if ((child->parent->parent->children[LEFT_CHILD] != NULL) && (child->parent->parent->children[RIGHT_CHILD] != NULL) && (child->parent->parent->children[LEFT_CHILD]->red == 1) && (child->parent->parent->children[RIGHT_CHILD]->red == 1)) {
			child->parent->parent->children[LEFT_CHILD]->red = 0;
			child->parent->parent->children[RIGHT_CHILD]->red = 0;

			/* if the grandparent is the root node, we're done!*/
			if (child->parent->parent->parent == NULL) {

				return 1;
			}

			/*otherwise we change the grandparent's color to red and do a recursive call on that node*/
			else {

				child->parent->parent->red = 1;
				return rbtreeify(child->parent->parent);
			}
		}

		/*If this node is the left child of a left child, then we need to restructure*/
		if ((child->parent->parent->children[LEFT_CHILD] != NULL) && (child->parent->children[LEFT_CHILD] != NULL) && (child->parent->parent->children[LEFT_CHILD] == child->parent) && (child == child->parent->children[LEFT_CHILD])) {
			child->parent->red = 0;
			child->parent->parent->red = 1;

			tempGparent = child->parent->parent;

			/*If the grandparent's parent is null, it is the root*/
			if (child->parent->parent->parent == NULL) {
				root = child->parent;
			}
			/*Otherwise we set the great grandparents left child to the parent*/
			else {
				child->parent->parent->parent->children[LEFT_CHILD] = child->parent;
			}

			/*TODO Do not know if this next line is valid. maybe???*/
			child->parent->parent = child->parent->parent->parent;
			tempGparent->children[LEFT_CHILD] = child->parent->children[RIGHT_CHILD];
			if (child->parent->children[RIGHT_CHILD] != NULL) {
				child->parent->children[RIGHT_CHILD]->parent = tempGparent;
			}

			child->parent->children[RIGHT_CHILD] = tempGparent;
			tempGparent->parent = child->parent;

			return 1;
		}

		/*If this node is the right child of a right child, then we need to restructure*/
		if ((child->parent->parent->children[RIGHT_CHILD] != NULL) && (child->parent->children[RIGHT_CHILD] != NULL) && (child->parent->parent->children[RIGHT_CHILD] == child->parent) && (child == child->parent->children[RIGHT_CHILD])) {
			/*Set parent to black and grandparent to red*/
			child->parent->red = 0;
			child->parent->parent->red = 1;

			tempGparent = child->parent->parent;

			/*If the grandparent's parent is null, our parent becomes the root*/
			if (child->parent->parent->parent == NULL) {
				root = child->parent;
				/*something heree???*/
			}
			/*Otherwise we set the great grandparents right child to the parent*/
			else {
				child->parent->parent->parent->children[RIGHT_CHILD] = child->parent;
			}

			/*TODO Do not know if this next line is valid. What I want to do is to
			 	set the parent's parent field equal to the granparent's parent. this will
				break the connection but will we get the information before it is lost?*/
			child->parent->parent = child->parent->parent->parent;

			/*Set the granparents right child to the parents left. RBT rules make this okay*/
			tempGparent->children[RIGHT_CHILD] = child->parent->children[LEFT_CHILD];

			/*If the parent's left child is not null, we set it's parent to the gparent*/
			if (child->parent->children[LEFT_CHILD] != NULL) {
				child->parent->children[LEFT_CHILD]->parent = tempGparent;
			}
			/*set parents left child to the grandparent*/
			child->parent->children[LEFT_CHILD] = tempGparent;

			/*Set the temporary grandparent's parent to the parent of our child*/
			tempGparent->parent = child->parent;

			return 1;
		}
		/*If the parent is a left child, and the child is a right child*/
		if ((child->parent->children[RIGHT_CHILD] != NULL) && (child->parent->parent->children[LEFT_CHILD] != NULL) && (child == child->parent->children[RIGHT_CHILD]) && (child->parent->parent->children[LEFT_CHILD] == child->parent)) {
			/*Change this node's color to black and set the granparent to red*/
			child->red = 0;
			child->parent->parent->red = 1;

			/*Create temporary node to store grandparent's parent*/
			tempNode = child->parent->parent->parent;

			/*
			 * Also need to find which child the grandparent was! 0 is left, 1 is right
			 * This defaults to 0, so just set to 1 if it's a right child.
			 */
			if (child->parent->parent->parent->children[RIGHT_CHILD] == child->parent->parent)
				whichChild = 1;

			/*set the grandparent to the child's right child. Then we set the gparent's (now the child's child) left child to NULL and parent to child */
			child->parent->parent->children[LEFT_CHILD] = child->children[RIGHT_CHILD];
			child->children[RIGHT_CHILD] = child->parent->parent;
			child->children[RIGHT_CHILD]->parent = child;
			if (child->children[RIGHT_CHILD]->children[LEFT_CHILD] != NULL)
				child->children[RIGHT_CHILD]->children[LEFT_CHILD]->parent = child->children[RIGHT_CHILD];

			/*set the parent to the child's left child. Then we set the parents right child to NULL and it's parent to the child*/
			child->parent->children[RIGHT_CHILD] = child->children[LEFT_CHILD];
			child->children[LEFT_CHILD] = child->parent;
			child->children[LEFT_CHILD]->parent = child;
			if (child->children[LEFT_CHILD]->children[RIGHT_CHILD] != NULL)
				child->children[LEFT_CHILD]->children[RIGHT_CHILD]->parent = child->children[LEFT_CHILD];

			/*If the grandparent's parent was null, then we set the child to the root*/
			if (tempNode == NULL) {
				child->parent = NULL;
				root = child;
			}
			/*Otherwise we set the child's parent to the grandparents parent*/
			if (tempNode != NULL) {
				child->parent = tempNode;
				tempNode->children[whichChild] = child;
			}

			return 1;
		}
		/*If the parent is a right child and the child is a left child*/
		if ((child->parent->children[LEFT_CHILD] != NULL) && (child->parent->parent->children[RIGHT_CHILD] != NULL) && (child == child->parent->children[LEFT_CHILD]) && (child->parent->parent->children[RIGHT_CHILD] == child->parent)) {
			/*Change this node's color to black and set the granparent to red*/
			child->red = 0;
			child->parent->parent->red = 1;

			/*Create temporary node to store grandparent's parent*/
			tempNode = child->parent->parent->parent;

			/*
			 * Also need to find which child the grandparent was! 0 is left, 1 is right
			 * This defaults to 0, so just set to 1 if it's a right child.
			 */
			if (child->parent->parent->parent->children[RIGHT_CHILD] == child->parent->parent)
				whichChild = 1;

			/*set the grandparent to the child's left child. Then we set the gparent's (now the child's child) right child to NULL and parent to child */
			child->parent->parent->children[RIGHT_CHILD] = child->children[LEFT_CHILD];
			child->children[LEFT_CHILD] = child->parent->parent;
			child->children[LEFT_CHILD]->parent = child;
			if (child->children[LEFT_CHILD]->children[RIGHT_CHILD] != NULL)
				child->children[LEFT_CHILD]->children[RIGHT_CHILD]->parent = child->children[LEFT_CHILD];

			/*set the parent to the child's right child. Then we set the parents left child to NULL and it's parent to the child*/

			child->parent->children[LEFT_CHILD] = child->children[RIGHT_CHILD];
			child->children[RIGHT_CHILD] = child->parent;
			child->children[RIGHT_CHILD]->parent = child;
			if (child->children[RIGHT_CHILD]->children[LEFT_CHILD] != NULL)
				child->children[RIGHT_CHILD]->children[LEFT_CHILD]->parent = child->children[RIGHT_CHILD];

			/*If the grandparent's parent was null, then we set the child to the root*/
			if (tempNode == NULL) {
				child->parent = NULL;
				root = child;
			}
			/*Otherwise we set the child's parent to the grandparents parent*/
			if (tempNode != NULL) {
				child->parent = tempNode;
				tempNode->children[whichChild] = child;
			}
			return 1;
		}
	}

	return 0;
}

/**
 * Helper method used by rbtree_insert()
 * Inserts a node into the red black tree like a standard BST and then rbtreeifies for 
 * 		red-black tree properties.
 * :param ptr: The address that has been allocated and to be inserted
 * :param size: The size of the memory allocated to be inserted
 * :param rbtree_root: The pointer to the root of the red black tree
 * :param new_node: The new node to be inserted
 * :return:
 */
int insert_helper(void *ptr, size_t size, RBNode *rbtree_root, RBNode *new_node) {
	/*
	 * We always want to insert at the
 	 * bottom of the tree,
	 * so we insert the node at the bottom first,
	 * then fix problems!
	 */

	/*
	 * Replace the already existing node
	 * as long as it's been freed, otherwise
	 * return -1 (error).
	 */
	if (rbtree_root->ptr == ptr) {
		if (rbtree_root->free) {
			rbtree_root->size = size;
			rbtree_root->free = 0;
			return 1;
		}
		else {
			printf("Error on malloc537\nOccupied node with address %p already exists! How did this happen?\n", ptr);
			return -1;
		}
	}
	/*
	 * If our ptr is bigger than the rbtree_root's ptr,
	 * look at the right node. If it's empty,
	 * make a new node and put it there.
	 * Otherwise, call insert_r on that right node!
	 * Recursion!
	 */
	else if (rbtree_root->ptr < ptr) {
		if (rbtree_root->children[RIGHT_CHILD] != NULL) {
			return insert_helper(ptr, size, rbtree_root->children[RIGHT_CHILD], new_node);
		}
		else {
			rbtree_root->children[RIGHT_CHILD] = new_node;
			new_node->parent = rbtree_root;
			return 1;
		}
	}
	/*
	 * The only thing left is to check the left node!
	 * Insert a new node if it's empty, or call insert_r
	 * with the left node as rbtree_root.
	 */
	else if (rbtree_root->ptr > ptr) {
		if (rbtree_root->children[LEFT_CHILD] != NULL) {
			return insert_helper(ptr, size, rbtree_root->children[LEFT_CHILD], new_node);
		}
		else {
			rbtree_root->children[LEFT_CHILD] = new_node;
			new_node->parent = rbtree_root;
			return 1;
		}
	}

	return 0;
}

/**
 * Creates a new node for ptr and size and inserts it into the red black tree.
 * The tree follows all the red black properties and hence remains balanced.
 * The node is created only if no node pointing ptr in red black tree exists.
 * If such a node exists, but has been freed, it is just updated.
 * If a node which hasn't been freed exists, an error is returned.
 * :param ptr: The starting address of the memory allocated to be inserted in the tree
 * :param size: The size of the memory allocated
 * :return:
 */
int rbtree_insert(void *ptr, size_t size) {
	int insert_return;
	int clean_tree_return;

	/*
	 * First we make our node.
	 */
	RBNode *temp = create_RBNode(ptr, size);

	/*
	 * Special ptr case:
	 * empty tree!
	 * For an empty tree, we create a black node,
	 * and insert it as the tree's root!
	 */
	if (root == NULL) {
		root = temp;
		root->red = 0;
		return 1;
	}

	/*
	 * Otherwise, pass along our created node
	 * to our recursive insert function.
	 */
	insert_return = insert_helper(ptr, size, root, temp);
	if (insert_return < 0) {
		fprintf(stderr, "Error while inserting a red black node! Exiting...\n");
		return insert_return;
	}

	/*DEBUG - print the tree here. we're lazy.
	print(root, 0); */

	/*
	 * And now, we clean up our messy tree!
	 */
	clean_tree_return = rbtreeify(temp);
	if (clean_tree_return < 0) {
		fprintf(stderr, "Error on clean_tree return!\n");
		return clean_tree_return;
	}

	/*print the node we've added.
	printf("red = %d node at %p with ptr %p size %i, parent %p, and children %p %p\n",temp->red, (void *)temp ,temp->ptr, (int)temp->size, (void *)temp->parent, (void *)temp->children[LEFT_CHILD], (void *)temp->children[RIGHT_CHILD]);
	*/

	/*
	 * Return 1 on a success.
	 */
	return 1;
}

/**
 * rbtreeifies the tree after deleting a node by performing required rotations
 * :param node: The node that was deleted
 */
void delete_rearrangement(RBNode *node) {
	/*if the node has become the root, we are fine.*/

	if (node->parent == NULL) {
		return;
	}

	/*In these next two cases (mirror images of each other), node has a red sibling. We will change the
	 *color of n's sibling and rotate around the parent of node. This will not fix the problem, but instead
	 * it makes it so we can fix it later*/
	if (node->parent->children[LEFT_CHILD] == node) {
		if (node->parent->children[RIGHT_CHILD]->red == 1) {
			node->parent->children[RIGHT_CHILD]->red = 0;
			node->parent->red = 1;
			rotate_rbnodes(node->parent, LEFT_CHILD);
		}
	}

	/*As mentioned above, this is a mirror image of the previous case*/
	if (node->parent->children[RIGHT_CHILD] == node) {
		if (node->parent->children[LEFT_CHILD]->red == 1) {
			node->parent->children[LEFT_CHILD]->red = 0;
			node->parent->red = 1;
			rotate_rbnodes(node->parent, RIGHT_CHILD);
		}
	}

	/*The next two cases are mirror imgages of each other. If node's parent, sibling and siblings kids
	 *are all black, we color it's sibling red and run a recursive call on N's parent*/
	if (node->parent->children[LEFT_CHILD] == node) {
		if ((node->parent->red == 0) && (node->parent->children[RIGHT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[RIGHT_CHILD]->red == 0)) {
			node->parent->children[RIGHT_CHILD]->red = 1;
			delete_rearrangement(node->parent);
		}
	}
	/*Mirror image of the previous case*/
	if (node->parent->children[RIGHT_CHILD] == node) {
		if ((node->parent->red == 0) && (node->parent->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red == 0)) {
			node->parent->children[LEFT_CHILD]->red = 1;
			delete_rearrangement(node->parent);
		}
	}
	/*The node's parent is red, and it's sibling, and sibling's children are black. We simply exchange the
	 * color of the node's*/
	if (node->parent->children[LEFT_CHILD] == node) {
		if ((node->parent->red == 1) && (node->parent->children[RIGHT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[RIGHT_CHILD]->red == 0)) {
			node->parent->red = 0;
			node->parent->children[RIGHT_CHILD]->red = 1;
		}
	}
	/*Mirror image*/
	if (node->parent->children[RIGHT_CHILD] == node) {
		if ((node->parent->red == 1) && (node->parent->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red == 0)) {
			node->parent->red = 0;
			node->parent->children[LEFT_CHILD]->red = 1;
		}
	}
	/*The node's sibling is black, and it's(the child's sibling's) left child is red and right is black
	 *We switch the color of the sibling and and its left sibling and rotate_r*/
	if (node->parent->children[LEFT_CHILD] == node) {
		if ((node->parent->children[RIGHT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red == 1) && (node->parent->children[RIGHT_CHILD]->children[RIGHT_CHILD]->red == 0)) {
			node->parent->children[RIGHT_CHILD]->red = 1;
			node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red = 0;
			rotate_rbnodes(node->parent->children[RIGHT_CHILD], RIGHT_CHILD);
			delete_rearrangement(node);
		}
	}
	/*Mirror Image*/
	if (node->parent->children[RIGHT_CHILD] == node) {
		if ((node->parent->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red == 1)) {
			node->parent->children[LEFT_CHILD]->red = 1;
			node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red = 0;
			rotate_rbnodes(node->parent->children[LEFT_CHILD], LEFT_CHILD);
			delete_rearrangement(node);
		}
	}
	/*The node's sibling is balck and that's sibling's right child is red. In this case the node
	 *to be deleted is the left child of it's parent. We switch the color of our nodes parent
	 * and our nodes sibling and rotate_l at our node's parent.*/
	if ((node->parent->children[LEFT_CHILD] == node) && (node->parent->children[RIGHT_CHILD]->red == node->red)) {
		node->parent->red = 0;
		node->parent->children[RIGHT_CHILD]->red = 0;
		rotate_rbnodes(node->parent, LEFT_CHILD);
	}
	/*mirror image of above*/
	if ((node->parent->children[RIGHT_CHILD] == node) && (node->parent->children[LEFT_CHILD]->red == node->red)) {
		node->parent->red = 0;
		node->parent->children[LEFT_CHILD]->red = 0;
		rotate_rbnodes(node->parent, RIGHT_CHILD);
	}
}

/**
 * Deletes a node which starts at ptr and fixes the tree for red black properties
 * :param ptr: The starting address of the node that is to be deleted
 * :return:
 */
int rbtree_delete_node(void *ptr) {
	RBNode *temp = rbtree_point_search(ptr);
	printf("Deleting node at %p\n", (void *)temp);
	if (temp == NULL) {
		printf("You cannot delete a node for a ptr that is not in the tree.");
	}
	/*If node to be deleted has 2 children, give that node the value of its in order
 	*predecessor. We will then delete that node.
 	*/
	if (temp->children[LEFT_CHILD] != NULL && temp->children[RIGHT_CHILD] != NULL) {
		RBNode *predecessor = temp;
		while (predecessor->children[RIGHT_CHILD] != NULL) {
			predecessor = predecessor->children[RIGHT_CHILD];
		}
		temp->ptr = predecessor->ptr;
		temp->size = predecessor->size;
		temp = predecessor;
	}
	/*creating child node. We will repalce our temporary node with it later*/
	RBNode *child = NULL;
	if (temp->children[LEFT_CHILD] == NULL) {
		child = temp->children[RIGHT_CHILD];
	}

	if (temp->children[RIGHT_CHILD] == NULL) {
		child = temp->children[LEFT_CHILD];
	}

	/*If the node we are now deleting (temp) is red, we are finished. If it is black, we have to 
 	**rearrange the tree*/
	if (temp->red == 0) {
		temp->red = child->red;
		delete_rearrangement(temp);
	}

	replace_rbtree_node(temp, child);
	free(temp);
	return 1;
}

/**
 * The helper method used by rbtree_print
 * :param root: The root of the red black tree to be printed
 * :param depth: The recursive depth parameter to print the .'s
 */
void print_helper(RBNode *root, int depth) {
	int i;
	if (root->children[LEFT_CHILD] != NULL)
		print_helper(root->children[LEFT_CHILD], depth + 1);
	for (i = 0; i < depth; i++) {
		printf(".");
	}

	printf("red = %d, free = %d node at %p with ptr %p size %i, parent %p, and children %p %p\n", root->red, root->free, (void *)root, root->ptr, (int)root->size, (void *)root->parent, (void *)root->children[LEFT_CHILD], (void *)root->children[RIGHT_CHILD]);

	if (root->children[RIGHT_CHILD] != NULL)
		print_helper(root->children[RIGHT_CHILD], depth + 1);
}

/**
 * This method prints the red black tree rooted at global "root" param
 */
void rbtree_print() {
	print_helper(root, 0);
}
