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
	hash_table *my_map = create_hash_table(10000); // TODO: Make a const for the size of hash table

	make_stats *make_file_stats = read_input_makefile(my_map, "test_inputs/Makefile2");
	// fprintf(stdout, "Exiting read_input_makefile()\n");

	directed_graph *dag = create_graph(make_file_stats->nodes_count);
	// fprintf(stdout, "Exiting create_graph()\n");

	construct_graph_edges(dag, my_map, make_file_stats->root);
	fprintf(stdout, "Exiting construct_graph_edges()\n");
	print_graph(dag);

	int index_head = index_head_node(dag, make_file_stats->root);
	printf("Index of root: %d\n", index_head);

	depth_first_topological_traversal(dag, index_head, dag->targets_and_dependencies);
	struct graph_adj_list_node **topologically_sorted_nodes = topo_list(dag);
	printf("\n");
	for (int i = 0; i < 50; i++) {
		if (topologically_sorted_nodes[i] == NULL)
			break;
		printf("->%s", topologically_sorted_nodes[i]->target->name);
	}
	printf("\n");

	int i = 0;
	while (topologically_sorted_nodes[i] != NULL) {
		printf("Inside execute loop %d\n", i);
		FILE *fp;
		fp = fopen(topologically_sorted_nodes[i]->target->name, "r");
		printf("topologically_sorted_node->target and name are %d and %s and fp is %d\n", topologically_sorted_nodes[i]->target, topologically_sorted_nodes[i]->target->name, fp);
		if (fp == NULL) {
			if (topologically_sorted_nodes[i]->target->rules != NULL)
				execute_program(topologically_sorted_nodes[i]->target);
			printf("Exiting Execute_program for node at %d\n", topologically_sorted_nodes[i]->target);
		}
		else {
			fclose(fp);
		}
		i++;
	}

	return 0;
}
