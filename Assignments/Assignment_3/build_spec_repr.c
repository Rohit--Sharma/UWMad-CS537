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
#include <stdlib.h>
#include "build_spec_graph.h"

command * create_command (char * rule) {
    command * cmd = (command *) malloc(sizeof(command));
    cmd->rule = rule;
    cmd->next = NULL;
    return cmd;
}

MakeNode * create_node (char * target, command * cmds_head) {
    MakeNode * makenode = (MakeNode *) malloc (sizeof(MakeNode));

    makenode->name = target;
    makenode->rules = cmds_head;
    makenode->isLeafNode = 0;
    makenode->children = NULL;

    return makenode;
}

void display_node (MakeNode * makenode) {
    if (makenode != NULL) {
        fprintf(stdout, "Target: %s\n", makenode->name);
        command * cmds_head = makenode->rules;
        while (cmds_head != NULL) {
            fprintf(stdout, "  Command: %s\n", cmds_head->rule);
            cmds_head = cmds_head->next;
        }
    }
    else
        fprintf(stderr, "Error: Cannot display makenode. makenode is NULL\n");
}
