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
#include <ctype.h>
#include <unistd.h>
#include <linux/limits.h>
#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "proc_creation_prog_exe.h"
#include "text_parsing.h"

const size_t MAX_LINE_LEN = 1024;
const size_t HASH_TABLE_SIZE = 10000;
const int debug = 0;

typedef struct {
	char *makefile_name;
	char *target_token;
} cmdline_options;

cmdline_options *create_cmdline_args_struct (char *make, char *token) {
	cmdline_options *result = (cmdline_options *) malloc(sizeof(cmdline_options));
	result->makefile_name = make;
	result->target_token = token;
	return result;
}

//This takes care of the bonus part - to pass any name for makefile through input
cmdline_options *options_processor (int argc, char *argv[]) 
{
	char *makefile_path = NULL;
	// if no argument passed, return makefile or Makefile in the same order whichever exists. else, throw an error
	if (argc <= 1) {
		makefile_path = "makefile";
		if (access(makefile_path, F_OK) != -1) {
			return create_cmdline_args_struct (makefile_path, NULL);
		}
		else {
			makefile_path = "Makefile";
			if (access(makefile_path, F_OK) != -1) {
				return create_cmdline_args_struct (makefile_path, NULL);
			}
			else {
				fprintf(stderr, "Error 'makefile' or 'Makefile' doesn't exist.\n");
				return NULL;
			}
		}
	}

	extern char *optarg;
	extern int optopt;
	extern int optind;

	int c;
	while ((c = getopt(argc, argv, "f:")) != -1) {
		switch (c)
		{
			case 'f':
				if (optarg != NULL) {
					makefile_path = optarg;
					if (access(makefile_path, F_OK) == -1) {
						fprintf(stderr, "Error no such file %s exists.\n", makefile_path);
						return NULL;
					}
				}
				break;
			case '?':
				if (optopt == 'f')
					fprintf(stderr, "Option -f requires an argument.\n");
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				return NULL;
			default:
				abort();
				if (debug)
					printf ("Default\n");
				return NULL;
		}
	}

	// Check if any targets are passed that are to be executed using non-opt args
	if (optind + 1 < argc) {
		if (debug)
			fprintf(stdout, "Extra command line arguments passed. \nExiting...\n");
		exit(1);
		return NULL;
	}
	if (optind < argc) {
		if (debug)
			fprintf(stdout, "Target to be executed: %s", argv[optind]);
		if (makefile_path != NULL) {
			return create_cmdline_args_struct (makefile_path, argv[optind]);
		}
		else {
			makefile_path = "makefile";
			if (access(makefile_path, F_OK) != -1) {
				return create_cmdline_args_struct (makefile_path, argv[optind]);
			}
			else {
				makefile_path = "Makefile";
				if (access(makefile_path, F_OK) != -1) {
					return create_cmdline_args_struct (makefile_path, argv[optind]);
				}
				else {
					fprintf(stderr, "Error 'makefile' or 'Makefile' doesn't exist.\n");
					return NULL;
				}
			}
		}
	}
	return create_cmdline_args_struct (makefile_path, NULL);
}

int main(int argc, char *argv[])
{
	cmdline_options *makefile_struct = options_processor(argc, argv);
	if (makefile_struct == NULL) {
		fprintf(stderr, "Error: Specified Makefile not found.\n Usage: ./537make (or) ./537make -f <Makefile name>\nExiting...\n");
		exit(1);
	}

	if (debug)
		fprintf (stderr, "Makefile from cmdline: %s %s\n", makefile_struct->makefile_name, makefile_struct->target_token);

	//Creating a hash table
	hash_table *my_map = create_hash_table(HASH_TABLE_SIZE);

	//Reads input make file and makes sense of the targets and dependencies
	make_stats *make_file_stats = read_input_makefile(my_map, makefile_struct->makefile_name);
	if (debug)
		fprintf(stdout, "Exiting read_input_makefile()\n");

	//Creates a directed graph with targets and dependencies
	directed_graph *dag = create_graph(make_file_stats->nodes_count);
	if (debug)
		fprintf(stdout, "Exiting create_graph(%d)\n", make_file_stats->nodes_count);

	//Construct the graph with all the edges taken care of
	construct_graph_edges(dag, my_map);
	if (debug)
	{
		fprintf(stdout, "Exiting construct_graph_edges()\n");
		print_graph(dag);
	}

	char *root = NULL;
	if (makefile_struct->target_token != NULL) {
		root = makefile_struct->target_token;
		free (makefile_struct);
	}
	else {
		free (makefile_struct);
		root = make_file_stats->root;
	}

	int index_head = root != NULL ? index_head_node(dag, root) : -1;
	if (index_head == -1) {
		fprintf (stderr, "Target %s not found in the makefile.\n Exiting...\n", root);
		free(make_file_stats->root);
		free(make_file_stats);
		exit(1);
	}

	if (debug)
	{
		printf("Index of root: %d\n", index_head);
		print_modify_builds(dag);
		printf("Index of root: %d\n", index_head);
	}	
	
	//Detects any cycles in the graph and exits if one exists
	int cycle;
	cycle = is_dag_cyclic(dag, index_head);
	if (cycle)
	{
		fprintf(stderr, "Makefile has a cyclic dependency. Aborting!\n");
		free(make_file_stats->root);
		free(make_file_stats);
		exit(1);
	}
	else
	{
		if (debug)
			printf("\nGraph is %d and graph is alright", cycle);
	}

	//Since there are no cycles in the directed graph, do a topological sort to 
	//determine order of executing the dependencies
	depth_first_topological_traversal(dag, index_head, dag->targets_and_dependencies);
	struct graph_adj_list_node **topologically_sorted_nodes = topo_list(dag);
	if (debug)
	{
		printf("\n");
		for (int i = 0; i < dag->targets_and_dependencies; i++)
		{
			if (topologically_sorted_nodes[i] == NULL)
				break;
			printf("->%s", topologically_sorted_nodes[i]->target->name);
		}
		printf("\n");
	}

	//If the file does not exist, build it.
	//If the file exists, but needs to be rebuilt due to some dependency file being change, build it.
	int i = 0;
	while (i < dag->targets_and_dependencies && topologically_sorted_nodes[i] != NULL)
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
			if (topologically_sorted_nodes[i]->target->modify_build == 1)
				execute_program(topologically_sorted_nodes[i]->target);
			fclose(fp);
		}
		i++;
	}

	// Cleanup of memory
        //delete_hash_table (my_map, HASH_TABLE_SIZE);
	//delete_graph (dag, make_file_stats->nodes_count);
	free(make_file_stats->root);
	free(make_file_stats);

	// delete topo_sorted_nodes
	/*
	struct graph_adj_list_node **temp = topologically_sorted_nodes;
	for (int i = 0; i < dag->targets_and_dependencies; i++) {
		struct graph_adj_list_node *curr = temp[i];
		
		struct graph_adj_list_node *curr_temp = NULL;
		while (curr != NULL) {
			curr_temp = curr->next;
			delete_makenode(curr->target);
			free(curr);
			curr = curr_temp;
		}
	}
	*/

	return 0;
}
