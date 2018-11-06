///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////
#include <time.h>

#ifndef STRUCT_LIST_COMMANDS
#define STRUCT_LIST_COMMANDS
typedef struct command {
    char *rule;
    struct command *next;
} command;
#endif

/**
 * Definition of the Node structure
 */
#ifndef STRUCT_MAKE_NODE
#define STRUCT_MAKE_NODE
typedef struct MakeNode
{
    char *name;                     // Tha name of the target
    time_t timestamp;
    command *rules;                     // The list of commands to be executed for the target
    int isLeafNode;                 // A flag which is 1 for a leaf node, else 0
    char **children;     // An array of pointers to dependency names
} MakeNode;
#endif

//void add_dependency(struct directed_acyclic_graph* dag, MakeNode* target, MakeNode* dependency);
