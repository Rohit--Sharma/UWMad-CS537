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

    read_input_makefile(my_map, "test_inputs/Simple_makefile");
    // fprintf(stdout, "Exiting read_input_makefile()\n");

    directed_graph *dag = create_graph(50);
    // fprintf(stdout, "Exiting create_graph()\n");

    construct_graph_edges(dag, my_map);
    // fprintf(stdout, "Exiting construct_graph_edges()\n");

    print_graph(dag);
    depth_first_topological_traversal(dag, 1, dag->targets_and_dependencies);
    struct graph_adj_list_node **topologically_sorted_nodes = topo_list(dag);
    // printf("\n");
	// for (int i = 0; i < 11; i++)
	// 	printf("->%s", topologically_sorted_nodes[i]->target->name);
	// for each node in order, call execute_program(node)

    // int i = 0;
    // while (topologically_sorted_nodes[i] != NULL){
    //     execute_program(topologically_sorted_nodes[i]->target);
    //     // fprintf(stdout, "\nSorted node %d is %s\n", topologically_sorted_nodes[i]->target->name);
    //     i++;
    // }

    return 0;
}
