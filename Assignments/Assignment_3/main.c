///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "text_parsing.h"

const int MAX_LINE_LEN = 1024;

int main() {
	
    hash_table *my_map = create_hash_table(10000);  // TODO: Make a const for the size of hash table

    read_input_makefile(my_map, "test_inputs/Makefile");

    // construct_graph_edges(my_map);

    return 0;
}
