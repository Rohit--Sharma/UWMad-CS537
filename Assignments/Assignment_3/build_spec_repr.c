///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "build_spec_graph.h"

MakeNode * create_node (char * target, char * command) {
    MakeNode * makenode = (MakeNode *) malloc (sizeof(MakeNode));

    makenode->name = target;
    makenode->rule = command;
    makenode->isLeafNode = 0;
    makenode->children = NULL;

    return makenode;
}

void display_node (MakeNode * makenode) {
    if (makenode != NULL)
        fprintf(stdout, "MTarget: %s\nMCommand: %s\n", makenode->name, makenode->rule);
    else
        fprintf(stderr, "Error: Cannot display makenode. makenode is NULL\n");
}