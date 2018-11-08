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
#include <string.h>
#include "build_spec_graph.h"

extern const int debug;

// A structure to represent an adjacency list node 
graph_adj_list_node* new_adj_list_node(MakeNode* target_node) 
{ 
    graph_adj_list_node *new_node = (graph_adj_list_node *)malloc(sizeof(graph_adj_list_node)); 
    new_node->target = target_node;
    new_node->next = NULL;
    return new_node; 
}

directed_graph* create_graph(int no_of_target_dependencies) 
{ 
    // fprintf(stdout, "Inside create_graph()\n");
    directed_graph *dag = (directed_graph *) malloc(sizeof(directed_graph));
    dag->targets_and_dependencies = no_of_target_dependencies;
  
    // Create an array of adjacency lists 
    dag->dependencies = (graph_adj_list_node **) malloc(no_of_target_dependencies * sizeof(graph_adj_list_node *));
    dag->visited_node = (int *) malloc(no_of_target_dependencies * sizeof(int));
    dag->topological_num = (int *) malloc(no_of_target_dependencies * sizeof(int));
    // Initialize each adjacency list as empty
    int i; 
    for (i = 0; i < no_of_target_dependencies; ++i) {
    	dag->dependencies[i] = NULL; 
        dag->visited_node[i] = 0;
        dag->topological_num[i] = 0;
    }
    return dag;
} 

void add_dependency(directed_graph* dag, MakeNode* target, MakeNode* dependency)
{
    // Add an edge from target to dependency
    graph_adj_list_node* target_node = NULL;
    graph_adj_list_node* new_dependency_node = new_adj_list_node(dependency); 
    graph_adj_list_node *dependency_node = NULL;
    int i = 0;
    int src_node = -1;
    int dest_node = -1;

    while (dag->dependencies[i] != NULL) {
        if (dag->dependencies[i]->target == target) {
            src_node = i;
            break;
        }
        i++;
    }
    i = 0;
    while (dag->dependencies[i] != NULL) {
        if (dag->dependencies[i]->target == dependency) {
            dest_node = i;
            // dependency_node = dag->dependencies[i];
            break;
        }
        i++;
    }
    
    if (src_node == -1) {
    	target_node = new_adj_list_node(target);
        for (i = 0; i < dag->targets_and_dependencies; i++) {
            if (dag->dependencies[i] == NULL) {
        	    dag->dependencies[i] = target_node;
		        //printf("\n\ndag %d head and target are %d and %d", i,  dag->dependencies[i].head, target_node->target);
        	    src_node = i;
        	    break;
            }
        }
    }

    if (dest_node == -1 && dependency != NULL) {
    	dependency_node = new_adj_list_node(dependency);
        for (i = 0; i < dag->targets_and_dependencies; i++) {
            if (dag->dependencies[i] == NULL) {
        	    dag->dependencies[i] = dependency_node;
		        //printf("\n\ndag %d head and target are %d and %d", i,  dag->dependencies[i].head, dependency_node->target);
        	    break;
            }
        }
    }
	
    // printf ("\ntarget is : //%s// and destination is //%s// and src_node and dest_node are %d and %d", target->name, dependency->name, src_node, dest_node);
    
    //dependency_node->next = dag->dependencies[src_node].head; 
    
    if (dependency != NULL) {
        graph_adj_list_node* temp_node = dag->dependencies[src_node];
        while (temp_node->next != NULL) {
            temp_node = temp_node->next;
        }
        temp_node->next = new_dependency_node;
    }
}

int index_head_node (directed_graph *dag, char *root) {
    for (int i = 0; i < dag->targets_and_dependencies; i++) {
        if (dag->dependencies[i] != NULL && strcmp(dag->dependencies[i]->target->name, root) == 0) {
            return i;
        }
    }
    return -1;
}

void print_graph(directed_graph* dag)
{
    int i;
    graph_adj_list_node* ptr = NULL; //(struct graph_adj_list_node*) malloc(sizeof(struct graph_adj_list_node));
    for (i = 0; i < dag->targets_and_dependencies; i++) {
        if (dag->dependencies[i] != NULL) {	
            printf("\nInside for loop for printing p_g interation %d of %d", i, dag->targets_and_dependencies);
            ptr = dag->dependencies[i];
            printf("\n%d: ", i);
            while (ptr) {
                printf("-> %s", ptr->target->name);
                ptr = ptr->next;
            }
            printf("->NULL\n");
        }
    }
}

int depth_first_topological_traversal(directed_graph* dag, int node_num, int n)
{
    graph_adj_list_node* adj_list = dag->dependencies[node_num];
    graph_adj_list_node* temp = adj_list;
    int next_node_num;
    dag->visited_node[node_num] = 1;
    //printf("Visited %s \n", dag->dependencies[node_num]->target->name);
    // printf("\nnode_num is %d and dependency target is %s.", node_num, dag->dependencies[node_num]->target->name);
    while (temp != NULL) {
        graph_adj_list_node* next_node = temp;
        for (int i = 0; i < dag->targets_and_dependencies; i++) {
	    if (dag->dependencies[i] == NULL)
		break;
            if (dag->dependencies[i]->target == next_node->target) {
                next_node_num = i;
            }
        }

        if (dag->visited_node[next_node_num] == 0) {
            n = depth_first_topological_traversal(dag, next_node_num, n);
            // printf("\nnode num is %d, topological num is %d and target is %s",next_node_num, dag->topological_num[next_node_num], dag->dependencies[next_node_num]->target->name); 
        }
        temp = temp->next;
    }

 	dag->topological_num[node_num] = n;
	if (debug) 
		printf("\nnode num is %d, topological num is %d and target is %s", node_num, dag->topological_num[node_num], dag->dependencies[node_num]->target->name); 
	return n-1;	
}

graph_adj_list_node** topo_list(directed_graph* dag)
{
	graph_adj_list_node **topologically_sorted_nodes;
	topologically_sorted_nodes = (graph_adj_list_node **) malloc((dag->targets_and_dependencies) * sizeof(graph_adj_list_node *));
	for (int i = 0; i < dag->targets_and_dependencies; i++) {
		if (dag->dependencies[i] != NULL) {
			if (debug) 
		        printf("\nnode num is %d %d\n", dag->targets_and_dependencies, dag->topological_num[i]); 
			topologically_sorted_nodes[(dag->targets_and_dependencies)-(dag->topological_num[i])] = dag->dependencies[i];
		}
	}
	return topologically_sorted_nodes;
}

// int main_1() {
// 	//printf("\nEnter here");
// 	MakeNode* n1 = (MakeNode *) malloc(sizeof(MakeNode));
// 	MakeNode* n2 = (MakeNode *) malloc(sizeof(MakeNode));
// 	MakeNode* n3 = (MakeNode *) malloc(sizeof(MakeNode));
// 	MakeNode* n4 = (MakeNode *) malloc(sizeof(MakeNode));
// 	MakeNode* n5 = (MakeNode *) malloc(sizeof(MakeNode));
// 	MakeNode* n6 = (MakeNode *) malloc(sizeof(MakeNode));
// 	//printf("\nNode 1 is %d\nNode2 is %d\nNode3 is %d\nNode4 is %d\n", n1, n2, n3, n4);
// 	char * str1 = (char *) malloc(sizeof(char)*50);
// 	char * str2 = (char *) malloc(sizeof(char)*50);
// 	char * str3 = (char *) malloc(sizeof(char)*50);
// 	char * str4 = (char *) malloc(sizeof(char)*50);
// 	char * str5 = (char *) malloc(sizeof(char)*50);
// 	char * str6 = (char *) malloc(sizeof(char)*50);
// 	str1 = "Red";
// 	str2 = "Green";
// 	str3 = "Blue";
// 	str4 = "Yellow";
// 	str5 = "White";
// 	str6 = "Black";
// 	n1->name = str1;
// 	n2->name = str2;
// 	n3->name = str3;
// 	n4->name = str4;
// 	n5->name = str5;
// 	n6->name = str6;
	
// 	//printf("\nAssigned Names here: Printing below:");
// 	//printf("\nn1 name is //%s//", n1->name);
// 	//printf("\nn2 name is //%s//", n2->name);
// 	//printf("\nn3 name is //%s//", n3->name);
// 	//printf("\nn4 name is //%s//", n4->name);
	
// 	directed_graph *dag = (directed_graph *) malloc(sizeof(struct directed_graph));
// 	// printf("\nAllocated space for DAG:");
	
// 	dag = create_graph(50);
// 	// printf("\nCreatedDAG:");
// 	// printf("\n\nDAG is located at : %d", dag);
// 	add_dependency(dag, n1, n2);
	
// 	// printf("\n\nDAG is located at : %d", dag);
// 	add_dependency(dag, n1, n3);
// 	add_dependency(dag, n2, n1);
// 	// printf("\n\nDAG is located at : %d", dag);
// 	add_dependency(dag, n2, n4);
// 	add_dependency(dag, n2, n5);
// 	add_dependency(dag, n3, n4);
// 	add_dependency(dag, n4, n5);
// 	add_dependency(dag, n5, n6);

// 	// for (int i=0; i<3; i++){
// 	// 	printf("\n\ni is %i and list is %d, %d", i, dag->dependencies[i], dag->dependencies[i]->target);
// 	// }
// 	print_graph(dag);
// 	depth_first_topological_traversal(dag, 0, 50);
// 	graph_adj_list_node **topologically_sorted_nodes = topo_list(dag);
// 	printf("\n");
// 	for (int i =0; i<6;i++)
// 		printf("->%s", topologically_sorted_nodes[i]->target->name);
// 	return 0;
// }
