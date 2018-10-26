///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////

/**
 * Definition of the Node structure
 */
#ifndef STRUCT_MAKE_NODE
#define STRUCT_MAKE_NODE
typedef struct MakeNode
{
    char *name;                     // Tha name of the target
    char *rule;                     // The command for creation of the target
    int isLeafNode;                 // A flag which is 1 for a leaf node, else 0
    struct MakeNode **children;     // An array of pointers to child nodes
} MakeNode;
#endif
