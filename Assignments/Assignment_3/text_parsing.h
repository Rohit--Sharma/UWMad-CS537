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

char **tokenize_string (char *input);
void construct_graph_edges (directed_graph *dag, hash_table *hash_map);
void read_input_makefile (hash_table *map, char *file_name);
