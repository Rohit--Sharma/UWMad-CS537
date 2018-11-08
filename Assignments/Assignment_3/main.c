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
const size_t HASH_TABLE_SIZE = 10000;
const int debug = 0;

int main()
{
	hash_table *my_map = create_hash_table(HASH_TABLE_SIZE); // TODO: Make a const for the size of hash table

	make_stats *make_file_stats = read_input_makefile(my_map, "test_inputs/Makefile2");
	if (debug)
		fprintf(stdout, "Exiting read_input_makefile()\n");

	directed_graph *dag = create_graph(make_file_stats->nodes_count);
	if (debug)
		fprintf(stdout, "Exiting create_graph(%d)\n", make_file_stats->nodes_count);

	construct_graph_edges(dag, my_map);
	if (debug)
	{
		fprintf(stdout, "Exiting construct_graph_edges()\n");
		print_graph(dag);
	}

	int cycle;
	cycle = is_dag_cyclic(dag);
	if (cycle)
	{
		fprintf(stderr, "Makefile has a cyclic dependency. Aborting!\n");
		exit(1);
	}
	else
	{
		if (debug)
			printf("\nGraph is %d and graph is alright", cycle);
	}
	int index_head = index_head_node(dag, make_file_stats->root);
	if (debug)
	{
		printf("Index of root: %d\n", index_head);
		print_modify_builds(dag);
		printf("Index of root: %d\n", index_head);
	}

	depth_first_topological_traversal(dag, index_head, dag->targets_and_dependencies);
	struct graph_adj_list_node **topologically_sorted_nodes = topo_list(dag);
	if (debug)
	{
		printf("\n");
		for (int i = 0; i < 50; i++)
		{
			if (topologically_sorted_nodes[i] == NULL)
				break;
			printf("->%s", topologically_sorted_nodes[i]->target->name);
		}
		printf("\n");
	}

	int i = 0;
	while (topologically_sorted_nodes[i] != NULL)
	{
		if (debug)
			printf("Inside execute loop %d\n", i);
		FILE *fp;
		fp = fopen(topologically_sorted_nodes[i]->target->name, "r");
		if (debug)
			printf("topologically_sorted_node->target and name are %p and %s and fp is %p\n", (void *)topologically_sorted_nodes[i]->target, topologically_sorted_nodes[i]->target->name, (void *)fp);
		if (fp == NULL)
		{
			if (topologically_sorted_nodes[i]->target->rules != NULL)
				execute_program(topologically_sorted_nodes[i]->target);
			if (debug)
				printf("Exiting Execute_program for node at %p\n", (void *)topologically_sorted_nodes[i]->target);
		}
		else
		{
			fclose(fp);
		}
		i++;
	}

	// Cleanup of memory
	// delete_hash_table (my_map, HASH_TABLE_SIZE);
	// delete_graph (dag, make_file_stats->nodes_count);

	return 0;
}
