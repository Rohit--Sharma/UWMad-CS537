///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////
#include "build_spec_repr.h"

#ifndef STRUCT_MAKE_STATS
#define STRUCT_MAKE_STATS
typedef struct make_stats 
{ 
    char *root;
    int nodes_count;
} make_stats;
#endif

char **tokenize_string (char *input);
void construct_graph_edges (directed_graph *dag, hash_table *hash_map, char *root);
make_stats *read_input_makefile (hash_table *map, char *file_name);
