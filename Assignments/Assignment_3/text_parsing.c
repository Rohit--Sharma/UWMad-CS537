///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "text_parsing.h"

extern const size_t MAX_LINE_LEN;
extern const size_t HASH_TABLE_SIZE;
extern const int debug;

make_tokens *create_token (char *token) {
    make_tokens *new_token = (make_tokens *) malloc(sizeof(make_tokens));
    new_token->token = token;
    new_token->next = NULL;
    return new_token;
}

make_tokens *tokenize_makestring (char *input) {
    if (debug) 
		fprintf(stdout, "Inside tokenize_makestring(%s)\n", input);
    
    int first_token = 1;

    char *temp = input;
    int i = 0;
    make_tokens *result = NULL, *curr = NULL;
    while (*(temp + i) != '\0') {
        
        if (first_token) {
            char *token = malloc(sizeof (char) * MAX_LINE_LEN);
            int j = 0;
            while (*(temp + i) != ' ' && *(temp + i) != ':' && *(temp + i) != '\0') {
                *(token + j) = *(temp + i);
                i++;
                j++;
            }
            // discard continuous spaces
            while (*(temp + i) == ' ' || *(temp + i) == ':') {
                i++;
            }

            *(token + j) = '\0';
            result = create_token (token); 
            curr = result;
            first_token = 0;
        }

        char *token = malloc(sizeof (char) * MAX_LINE_LEN);
        int j = 0;
        while (*(temp + i) != ' ' && *(temp + i) != ':' && *(temp + i) != '\0') {
            *(token + j) = *(temp + i);
            i++;
            j++;
        }
        // discard continuous spaces
        while (*(temp + i) == ' ') {
            i++;
        }

        *(token + j) = '\0';
        if (j > 0) {
            curr->next = create_token (token); 
        }
        else {
            free (token);
        }
        curr = curr->next;
    }
    return result;
}

char **tokenize_string (char *input) {
    if (debug) 
		fprintf(stdout, "Inside tokenize_string(%s)\n", input);
    char **result = NULL;
    char *temp = input;
    char *last_space = NULL;
    unsigned int i = 0;
    int count_tokens = 0;   // count_tokens will have the number of tokens in the input string + 1 for indicating the end
    while ((temp + i) != NULL && *(temp + i) != '\0') {
        if (*(temp + i) == ' ') {
            count_tokens++;
            // Ignore all the continuous white spaces
            while (*(temp + i) == ' ') {
                last_space = temp + i;
                i++;
            }
        }
        i++;
    }

    // Increment count_tokens for the last token
    if (input != NULL)
        count_tokens += last_space < (input + strlen(input) - 1);

    // Increment count_tokens for indicating the end of returned strings
    count_tokens++;

    if (debug) 
		fprintf(stdout, "Mallocing: %d\n", count_tokens);
    result = (char **) malloc(sizeof (char *) * count_tokens);
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\nExiting...\n");
        // free(munch1_param);
        // free(munch2_param);
        exit(1);
    }
    if (debug) 
		fprintf(stdout, "Malloced %d bytes in tokenize_string()\n", count_tokens);
    // TODO: Check return value of malloc
    size_t idx = 0;
    temp = input;
    i = 0;
    while (*(temp + i) != '\0') {
        char *token = malloc(sizeof (char) * MAX_LINE_LEN);
        int j = 0;
        while (*(temp + i) != ' ' && *(temp + i) != '\0') {
            *(token + j) = *(temp + i);
            i++;
            j++;
        }
        // discard continuous spaces
        while (*(temp + i) == ' ') {
            i++;
        }

        *(token + j) = '\0';
        *(result + idx++) = token;  // strdup(token);

        if (*(temp + i) == '\0')
            break;

        //i++;
    }
    *(result + idx) = 0;
    if (debug) 
		fprintf(stdout, "Done tokenizing\n");
    return result;
}

//This structure is used for returning a line from stdio
//It also checks whether eof has been encountered or
//if the line has exceeded the buffer size.
typedef struct {
    char *read_str;
    int has_eof;
    int buff_size_exceeding;
} read_line_val;

//This function returns a line if it is lesser than buffer size
//It takes care of what is returned when the EOF is encountered
read_line_val *read_line(int buff_size, FILE *fileptr) {
    read_line_val *new_line_val = (read_line_val *) malloc(sizeof(read_line_val));
    char *new_line = (char *) malloc(sizeof(char) * buff_size);
    if (errno == ENOMEM) {
        fprintf(stderr, "No enough memory for malloc\n");
        new_line[0] = '\0';
        new_line_val->read_str = new_line;
        new_line_val->has_eof = 1;
        new_line_val->buff_size_exceeding = 0;
        return new_line_val;
    }
    char c = '\0';
    for (int i = 0; i < buff_size; i++) {
        c = fgetc(fileptr);
        if (c != EOF && c != '\n') {
            new_line[i] = c;
        } else if (c == '\n') {
            new_line[i] = '\0';
            new_line_val->read_str = new_line;
            new_line_val->has_eof = 0;
            new_line_val->buff_size_exceeding = 0;
            return new_line_val;
        } else {
            new_line[i] = '\0';
            new_line_val->read_str = new_line;
            new_line_val->has_eof = 1;
            new_line_val->buff_size_exceeding = 0;
            return new_line_val;
        }
    }
    new_line_val->buff_size_exceeding = 1;
    new_line[buff_size - 1] = '\0';
    new_line_val->read_str = new_line;
    new_line_val->has_eof = 0;

    // Flush the rest of the line
    while ((c = fgetc(fileptr)) != '\n' && c != EOF);
    if (c == EOF)
        new_line_val->has_eof = 1;

    return new_line_val;
}

make_stats *read_input_makefile (hash_table *map, char *file_name) {
    make_stats *make_file_stats = (make_stats *) malloc(sizeof(make_stats));
    if (debug)
        fprintf(stdout, "Inside read_input_makefile()\n");
    FILE *makefile_ptr = fopen(file_name, "r");

    char *line = NULL;
    char *target_line = NULL;
    char *root = NULL;

    int num_nodes = 0;

    command *cmds_head = NULL, *curr_cmd = NULL;
    MakeNode *curr_node = NULL;
    int first_cmd = 1;

    do {
        read_line_val *line_struct = read_line(MAX_LINE_LEN, makefile_ptr);
        if (line_struct->buff_size_exceeding == 1) {
	        //If line size exceeds buffer size, then line is discarded and message is printed to stderr
            fprintf(stderr, "Error in parsing the Makefile. Line size exceeds the buffer size %zu\n", MAX_LINE_LEN);
            break;  // exit(1) ??
        }
        if (line_struct->has_eof == 1) {
            if (line_struct->read_str[0] != '\0') {
                line = line_struct->read_str;
            }
            else {
                break;
            }
        }
        else {
            line = line_struct->read_str;
        }

        // Process the line
        switch (*line) {
            case '\t': {
                // command

                if (target_line == NULL) {
                    fprintf(stderr, "Error: No target for the command found.\n");
                }

                if (first_cmd && cmds_head == NULL) {
                    cmds_head = create_command(line + 1);
                    curr_cmd = cmds_head;
                    first_cmd = 0;
                }
                else {
                    curr_cmd->next = create_command(line + 1);
                    curr_cmd = curr_cmd->next;
                }

                break;
            }
            case '\0': {
                // empty line

                break;
            }
            case '#': {
                // comment

                break;
            }
            default: {
                // target line

                // Execute previous commands if any exist
                if (target_line != NULL) {
                    curr_node = create_node(target_line, cmds_head);
                    int num_children = num_dependencies(curr_node);
                    num_nodes++;
                    num_nodes += num_children;
		            // printf("\n****current node name is %s****\n", curr_node->name);
                    // insert node into the hashmap
                    hash_insert(map, curr_node->name, curr_node);

                    if (root == NULL) {
                        root = curr_node->name;
                        make_file_stats->root = root;
                    }

                    if (debug) 
		                display_node(curr_node);

                    cmds_head = NULL;
                    first_cmd = 1;
                }
		
                // store the current line
                target_line = line;
                break;
            }
        }
        free(line_struct);
    } while (line != NULL);

    // If cmds_head is not null, create the last makenode
    if (cmds_head != NULL) {
        curr_node = create_node(target_line, cmds_head);
        int num_children = num_dependencies(curr_node);
        num_nodes++;
        num_nodes += num_children;
	    // printf("\n****current node name is %s****\n", curr_node->name);
        // insert node into the hashmap
        hash_insert(map, curr_node->name, curr_node);

        if (root == NULL) {
            root = curr_node->name;
            make_file_stats->root = root;
        }

        if (debug) 
		    display_node(curr_node);
    }

    fclose(makefile_ptr);

    make_file_stats->nodes_count = num_nodes;
    return make_file_stats;
}

/**
 * Creates edges for all the dependencies of the makefile
 */
void construct_graph_edges (directed_graph* dag, hash_table *hash_map) {
    if (debug)
        fprintf(stdout, "Inside construct_graph_edges()\n");
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hash_map->list[i] != NULL) {
            hash_node *temp = hash_map->list[i];
            while (temp != NULL) {
                if (debug)
                    fprintf(stdout, "%s: %p\n", temp->key, (void *) temp->val);
                make_tokens *children = temp->val->children;
                
				// if no dependencies
				if (children == NULL) {
                    if (debug)
                        fprintf(stdout, "Adding edge from %s to NULL\n", temp->val->name);
					add_dependency(dag, temp->val, NULL);
				}
				else {
                    int j = 0;
                    while (children != NULL && children->token != NULL && strlen(children->token) > 0) {
                        // fprintf(stdout, "%s | ", *(children + j));
                        // If the node for child exists, create an edge in the graph. Else, create the leaf node and edge for it in the graph
                        MakeNode *dependency_node = hash_lookup(hash_map, children->token);
                        if (dependency_node == NULL) {
                            // create leaf node
                            if (debug) 
		                        fprintf(stdout, "Creating new node for %s | \n", children->token);
                            dependency_node = create_node(children->token, NULL);
                            if (debug) 
		                        display_node(dependency_node);
                            if (debug) 
		                        printf("***children %d, name is %s***\n", j, children->token);
                            // printf("\n****current dependency name is %s****\n", dependency_node->name);

                            // insert the created node into the hash_map
                            hash_insert(hash_map, children->token, dependency_node);
                        }
                        // printf("\n****current dependency name is %s****\n", dependency_node->name);
                        if (debug)
                            fprintf(stdout, "Adding edge from %s to %s\n", temp->val->name, dependency_node->name);
                        add_dependency(dag, temp->val, dependency_node);

                        j++;
                        children = children->next;
                    }
                }
                temp = temp->next;
            }
        }
    }
}

/**
 * Returns the number of dependencies for a node
 * Used for computing the number of nodes to be created
 */
int num_dependencies (MakeNode *node) {
    make_tokens *dependencies = node->children;
    int i = 0;
    while (dependencies != NULL) {
        i++;
        dependencies = dependencies->next;
    }
    return i;
}

// int main() {
//     // char ** tokens = tokenize_string("main.o: main.c main.h dep1.c");
//     // char ** tokens = read_input_makefile("test_inputs/Makefile");
//     // int i = 0;
//     // while (*(tokens + i) != NULL) {
//     //     printf("%s\n", *(tokens + i));
//     //     free (*(tokens + i));
//     //     i++;
//     // }
//     // free (tokens);

//     hash_table *my_map = create_hash_table(10000);  // TODO: Make a const for the size of hash table

//     read_input_makefile(my_map, "test_inputs/Makefile");

//     construct_graph_edges(my_map);

//     return 0;
// }
