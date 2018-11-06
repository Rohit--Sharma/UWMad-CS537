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

extern const int MAX_LINE_LEN;

char **tokenize_string (char *input) {
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
    count_tokens += last_space < (input + strlen(input) - 1);

    // Increment count_tokens for indicating the end of returned strings
    count_tokens++;

    result = (char **) malloc(sizeof (char *) * count_tokens);
    // TODO: Check return value of malloc
    size_t idx = 0;
    temp = input;
    i = 0;
    while (*(temp + i) != '\0') {
        char *token = malloc(sizeof (char) * 64);
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
read_line_val *read_line(int buff_size, FILE *fileptr)
{
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

char **read_input_makefile (hash_table *map, char *file_name) {
    FILE *makefile_ptr = fopen(file_name, "r");

    char ch = fgetc(makefile_ptr);
    char *target_line = NULL;
    while (ch != EOF) {     // ((ch = fgetc(makefile_ptr)) != EOF) {
        MakeNode *curr_node = NULL;
        // read a line: 
        //   4 possibilities: target line, command (begins with \t), empty line, comment (TODO: bonus)
        switch (ch) {
            case '\t': {
                // command

                if (target_line == NULL) {
                    fprintf(stderr, "Error: No target for the command found.\n");
                }

                command *cmds_head = NULL;

                // read the whole line into a buffer
                char *command_line = (char *) malloc(sizeof(char) * 256);   // TODO: Change it to MAX_LINE_LEN

                int i = 0;
                ch = fgetc(makefile_ptr);   // ignore the \t char
                while (ch != EOF && ch != '\n' && ch != '\0') {
                    *(command_line + i) = ch;
                    i++;
                    ch = fgetc(makefile_ptr);
                }
                *(command_line + i) = '\0';

                cmds_head = create_command(command_line);

                // read the rest of the commands in the linked list
                command * curr_cmd = cmds_head;
                if (ch == '\n') {
                    ch = fgetc(makefile_ptr);
                    while (ch == '\t') {
                        command_line = (char *) malloc(sizeof(char) * 256);   // TODO: Change it to MAX_LINE_LEN

                        int i = 0;
                        ch = fgetc(makefile_ptr);   // ignore the \t char
                        while (ch != EOF && ch != '\n' && ch != '\0') {
                            *(command_line + i) = ch;
                            i++;
                            ch = fgetc(makefile_ptr);
                        }
                        *(command_line + i) = '\0';

                        curr_cmd->next = create_command(command_line);
                        curr_cmd = curr_cmd->next;

                        if (ch == '\n')
                            ch = fgetc(makefile_ptr);
                    }
                    fseek(makefile_ptr, -1, SEEK_CUR);
                }
                // fprintf(stdout, "Command: %s\n", command_line);

                char **tokens = tokenize_string(target_line);
                char *target = tokens[0];
                char **dependencies = tokens + 1;
                target[strlen(target) - 1] = '\0';    // remove the : char
                curr_node = create_node(target, cmds_head, dependencies);

                // insert node into the hashmap
                hash_insert(map, target, curr_node);

                display_node(curr_node);
                printf("   Dependencies: ");
                i = 0;
                while (*(dependencies + i) != NULL) {
                    printf("%s | ", *(dependencies + i));
                    i++;
                }
                printf("\n");

                target_line = NULL;

                break;
            }
            case '\n': {
                // empty line

                // igmore all the consecutive empty lines
                while (ch == '\n') {
                    // fprintf(stdout, "Ignoring new line\n");
                    ch = fgetc(makefile_ptr);
                }

                break;
            }
            case '#': {
                // comment

                break;
            }
            default: {
                // target line

                // read the whole line into a buffer
                target_line = (char *) malloc(sizeof(char) * 256);   // TODO: Change it to MAX_LINE_LEN

                int i = 0;
                while (ch != EOF && ch != '\n' && ch != '\0') {
                    *(target_line + i) = ch;
                    i++;
                    ch = fgetc(makefile_ptr);
                }
                *(target_line + i) = '\0';

                // fprintf(stdout, "Target: %s\n", target_line);
                // return tokenize_string(target_line);

                break;
            }
        }
        // break;
    }

    fclose(makefile_ptr);
    return NULL;
}

void construct_graph_edges (hash_table *hash_map) {
    for (int i = 0; i < 10000; i++) {  // TODO: Make a const for the size of hash table
        if (hash_map->list[i] != NULL) {
            hash_node *temp = hash_map->list[i];
            while (temp != NULL) {
                // fprintf(stdout, "%s: %p\n", temp->key, (void *) temp->val);
                char **children = temp->val->children;
                int j = 0;
                while (*(children + j) != NULL) {
                    // fprintf(stdout, "%s | ", *(children + j));
                    // If the node for child exists, create an edge in the graph. Else, create the leaf node and edge for it in the graph
                    MakeNode *dependency_node = hash_lookup(hash_map, *(children + j));
                    if (dependency_node == NULL) {
                        // create leaf node
                        dependency_node = create_node(*(children + j), NULL, NULL);

                        // insert the created node into the hash_map
                        hash_insert(hash_map, *(children + j), dependency_node);
                    }
                    // add_dependency(dag, temp->val, dependency_node);

                    j++;
                }
                temp = temp->next;
            }
        }
    }
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
