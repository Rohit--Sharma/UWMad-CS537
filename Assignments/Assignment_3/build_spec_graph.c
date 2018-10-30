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
struct adjListGraphNode
{
    MakeNode *target;
    struct adjListGraphNode* next;
};
  
// A structure to represent an adjacency list 
struct graphAdjList
{ 
    struct adjListGraphNode *head;
};

struct directedAcyclicGraph 
{ 
    int targets_and_dependencies;
    MakeNode *graphHead; 
    struct graphAdjList* dependencies;
};

struct adjListGraphNode* newAdjListNode(MakeNode* target_node) 
{ 
    struct adjListGraphNode* newNode = (struct adjListGraphNode*)malloc(sizeof(struct adjListGraphNode)); 
    newNode->target = target_node;
    newNode->next = NULL;
    return newNode; 
}

struct directedAcyclicGraph* createGraph(int no_of_target_dependencies) 
{ 
    struct directedAcyclicGraph* DAG = (struct directedAcyclicGraph*) malloc(sizeof(struct directedAcyclicGraph));
    DAG->targets_and_dependencies = no_of_target_dependencies;
  
    // Create an array of adjacency lists 
    DAG->dependencies = (struct graphAdjList*) malloc(no_of_target_dependencies * sizeof(struct graphAdjList));
  
    // Initialize each adjacency list as empty
    int i; 
    for (i = 0; i < no_of_target_dependencies; ++i) 
    	DAG->dependencies[i].head = NULL; 
  
    return DAG;
} 

void addDependency(struct directedAcyclicGraph* DAG, MakeNode *target, MakeNode *dependency)
{
    // Add an edge from target to dependency 
    struct adjListGraphNode* newNode = newAdjListNode(dependency);
    //How to update pointers here?
}

int main() {
	return 0;
}

