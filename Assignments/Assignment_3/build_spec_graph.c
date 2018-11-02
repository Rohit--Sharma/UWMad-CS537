///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "build_spec_graph.h"

// A structure to represent an adjacency list node 
struct graph_adj_list_node
{
    MakeNode *target;
    struct graph_adj_list_node* next;
};
  
// A structure to represent an adjacency list 
// struct graph_adj_list
// { 
//     struct graph_adj_list_node *head;
// };

struct directed_acyclic_graph 
{ 
    int targets_and_dependencies;
    //MakeNode *graph_head; 
    struct graph_adj_list_node **dependencies;
};

struct graph_adj_list_node* new_adj_list_node(MakeNode* target_node) 
{ 
    struct graph_adj_list_node *new_node = (struct graph_adj_list_node *)malloc(sizeof(struct graph_adj_list_node)); 
    new_node->target = target_node;
    new_node->next = NULL;
    return new_node; 
}

struct directed_acyclic_graph* create_graph(int no_of_target_dependencies) 
{ 
    struct directed_acyclic_graph *dag = (struct directed_acyclic_graph *) malloc(sizeof(struct directed_acyclic_graph));
    dag->targets_and_dependencies = no_of_target_dependencies;
  
    // Create an array of adjacency lists 
    dag->dependencies = (struct graph_adj_list_node **) malloc(no_of_target_dependencies * sizeof(struct graph_adj_list_node *));
  
    // Initialize each adjacency list as empty
    int i; 
    for (i = 0; i < no_of_target_dependencies; ++i) 
    	dag->dependencies[i] = NULL; 
  
    return dag;
} 

void add_dependency(struct directed_acyclic_graph* dag, MakeNode* target, MakeNode* dependency)
{
    // Add an edge from target to dependency
    struct graph_adj_list_node* target_node = NULL;
    struct graph_adj_list_node* new_dependency_node = new_adj_list_node(dependency); 
    struct graph_adj_list_node *dependency_node = NULL;
    int i = 0;
    int src_node = -1;
    int dest_node = -1;

    while (dag->dependencies[i] != NULL) {
        if (dag->dependencies[i]->target == target) {
            src_node = i;
            target_node = dag->dependencies[i];
            break;
        }
        i++;
    }
    i = 0;
    while (dag->dependencies[i] != NULL){
        if (dag->dependencies[i]->target == dependency) {
            dest_node = i;
            // dependency_node = dag->dependencies[i];
            break;
        }
        i++;
    }
    
    if (src_node == -1) {
    	target_node = new_adj_list_node(target);
        for (i=0; i < dag->targets_and_dependencies; i++) {
            if (dag->dependencies[i] == NULL) {
        	    dag->dependencies[i] = target_node;
		        //printf("\n\ndag %d head and target are %d and %d", i,  dag->dependencies[i].head, target_node->target);
        	    src_node = i;
        	    break;
            }
        }
    }

    if (dest_node == -1) {
    	dependency_node = new_adj_list_node(dependency);
        for (i=0; i<(dag->targets_and_dependencies); i++) {
            if (dag->dependencies[i] == NULL) {
        	    dag->dependencies[i] = dependency_node;
		        //printf("\n\ndag %d head and target are %d and %d", i,  dag->dependencies[i].head, dependency_node->target);
        	    dest_node = i;
        	    break;
            }
        }
    }
	
    // printf ("\ntarget is : //%s// and destination is //%s// and src_node and dest_node are %d and %d", target->name, dependency->name, src_node, dest_node);
    
    //dependency_node->next = dag->dependencies[src_node].head; 
    
    struct graph_adj_list_node* temp_node = dag->dependencies[src_node];
    while (temp_node->next != NULL) {
        temp_node = temp_node->next;
    }
    temp_node->next = new_dependency_node;
}

void print_graph(struct directed_acyclic_graph* dag)
{
    int i;
    struct graph_adj_list_node* ptr = NULL; //(struct graph_adj_list_node*) malloc(sizeof(struct graph_adj_list_node));
    for (i = 0; i < dag->targets_and_dependencies; i++)
    {	
        printf("\nInside for loop for printing interation %d of %d", i, dag->targets_and_dependencies);
	    ptr = dag->dependencies[i];
        printf("\n%d: ", i);
        while (ptr)
        {
            printf("-> %s", ptr->target->name);
            ptr = ptr->next;
        }
        printf("->NULL\n");
    }
}

int main() {
	//printf("\nEnter here");
	MakeNode* n1 = (MakeNode *) malloc(sizeof(MakeNode));
	MakeNode* n2 = (MakeNode *) malloc(sizeof(MakeNode));
	MakeNode* n3 = (MakeNode *) malloc(sizeof(MakeNode));
	MakeNode* n4 = (MakeNode *) malloc(sizeof(MakeNode));
	MakeNode* n5 = (MakeNode *) malloc(sizeof(MakeNode));
	//printf("\nNode 1 is %d\nNode2 is %d\nNode3 is %d\nNode4 is %d\n", n1, n2, n3, n4);
	char * str1 = (char *) malloc(sizeof(char)*50);
	char * str2 = (char *) malloc(sizeof(char)*50);
	char * str3 = (char *) malloc(sizeof(char)*50);
	char * str4 = (char *) malloc(sizeof(char)*50);
	char * str5 = (char *) malloc(sizeof(char)*50);
	str1 = "Red";
	str2 = "Green";
	str3 = "Blue";
	str4 = "Yellow";
	str5 = "White";
	n1->name = str1;
	n2->name = str2;
	n3->name = str3;
	n4->name = str4;
	n5->name = str5;
	
	//printf("\nAssigned Names here: Printing below:");
	//printf("\nn1 name is //%s//", n1->name);
	//printf("\nn2 name is //%s//", n2->name);
	//printf("\nn3 name is //%s//", n3->name);
	//printf("\nn4 name is //%s//", n4->name);
	
	struct directed_acyclic_graph *dag = (struct directed_acyclic_graph *) malloc(sizeof(struct directed_acyclic_graph));
	// printf("\nAllocated space for DAG:");
	
	dag = create_graph(5);
	// printf("\nCreatedDAG:");
	// printf("\n\nDAG is located at : %d", dag);
	add_dependency(dag, n1, n2);
	
	// printf("\n\nDAG is located at : %d", dag);
	add_dependency(dag, n1, n3);
	// printf("\n\nDAG is located at : %d", dag);
	add_dependency(dag, n2, n4);
	add_dependency(dag, n2, n5);
	add_dependency(dag, n3, n4);
	add_dependency(dag, n4, n5);

	// for (int i=0; i<3; i++){
	// 	printf("\n\ni is %i and list is %d, %d", i, dag->dependencies[i], dag->dependencies[i]->target);
	// }
	print_graph(dag);
	return 0;
}

