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
#include "build_spec_repr.h"
#include "proc_creation_prog_exe.h"
#include "text_parsing.h"

const size_t MAX_LINE_LEN = 1024;

int main() {
	
    hash_table *my_map = create_hash_table(10000);  // TODO: Make a const for the size of hash table

    read_input_makefile(my_map, "test_inputs/Makefile");

    directed_graph *dag = (directed_graph *) malloc(sizeof(directed_graph));
	
    dag = create_graph(50);
    
    construct_graph_edges(dag, my_map);

    print_graph(dag);
    depth_first_topological_traversal(dag, 11, dag->targets_and_dependencies);
    // struct graph_adj_list_node **topologically_sorted_nodes = topo_list(dag);
	// for each node in order, call execute_program(node)

    int i = 0;
    // while (topologically_sorted_nodes[i] != NULL){
    //     execute_program(topologically_sorted_nodes[i]->target);
    //     i++;
    // }

    return 0;
}
