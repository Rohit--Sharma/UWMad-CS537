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
#include "build_spec_graph.h"

extern const int MAX_LINE_LEN;

char ** tokenize_string (char *input) {

    char **result = NULL;
    char *temp = input;
    char *last_space = NULL;
    int i = 0;
    int count_tokens = 0;   // count_tokens will have the number of tokens in the input string + 1 for indicating the end
    while (*(temp + i) != '\0') {
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

        *(token + j) == '\0';
        *(result + idx++) = token;  // strdup(token);

        if (*(temp + i) == '\0')
            break;

        //i++;
    }
    *(result + idx) = 0;

    return result;
}

char ** read_input_makefile (char *file_name) {
    FILE * makefile_ptr = fopen(file_name, "r");

    char ch = fgetc(makefile_ptr);
    while (ch != EOF) {     // ((ch = fgetc(makefile_ptr)) != EOF) {
        MakeNode * curr_node = NULL;
        char * target_line = NULL;
        // read a line: 
        //   4 possibilities: target line, command (begins with \t), empty line, comment (TODO: bonus)
        switch (ch) {
            case '\t': {
                // command

                // read the whole line into a buffer
                char * command_line = (char *) malloc(sizeof(char) * 256);   // TODO: Change it to MAX_LINE_LEN

                int i = 0;
                ch = fgetc(makefile_ptr);   // ignore the \t char
                while (ch != EOF && ch != '\n' && ch != '\0') {
                    *(command_line + i) = ch;
                    i++;
                    ch = fgetc(makefile_ptr);
                }
                *(command_line + i) = '\0';

                fprintf(stdout, "Command: %s\n", command_line);

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

                fprintf(stdout, "Target: %s\n", target_line);
                // return tokenize_string(target_line);

                break;
            }
        }
        // break;
    }

    fclose(makefile_ptr);
    return NULL;
}

int main() {
    // char ** tokens = tokenize_string("main.o: main.c main.h dep1.c");
    // char ** tokens = read_input_makefile("test_inputs/Makefile");
    // int i = 0;
    // while (*(tokens + i) != NULL) {
    //     printf("%s\n", *(tokens + i));
    //     free (*(tokens + i));
    //     i++;
    // }
    // free (tokens);
    read_input_makefile("test_inputs/Makefile");

    return 0;
}
