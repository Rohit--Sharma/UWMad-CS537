///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////
#include "build_spec_graph.h"

#ifndef STRUCT_HASH_NODE
#define STRUCT_HASH_NODE
typedef struct hash_node {
    char *key;
    MakeNode *val;
    struct hash_node *next;
} hash_node;
#endif

#ifndef STRUCT_HASH_TABLE
#define STRUCT_HASH_TABLE
typedef struct hash_table {
    int size;
    struct hash_node **list;
} hash_table;
#endif

command *create_command (char *rule);
MakeNode *create_node (char *target_line, command *cmds_head);
void display_node (MakeNode *makenode);

hash_table *create_hash_table (int size);
void hash_insert (hash_table *t, char *key, MakeNode *val);
MakeNode *hash_lookup (hash_table *t, char *key);
