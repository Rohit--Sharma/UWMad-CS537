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

char **read_input_makefile (char *file_name) {
    char *input = "main.o: main.c main.h dep1.c";

    char **result = NULL;
    char *temp = input;
    char *last_space = NULL;
    int i = 0;
    int count_tokens = 0;   // count_tokens will have the number of tokens in the input string + 1 for indicating the end
    while (*(temp + i) != '\0') {
        if (*(temp + i) == ' ') {
            // TODO: Ignore all the continuous white spaces
            count_tokens++;
            last_space = temp + i;
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
        *(token + j) == '\0';
        *(result + idx++) = strdup(token);

        if (*(temp + i) == '\0')
            break;

        i++;
    }
    *(result + idx) = 0;

    return result;
}

int main() {
    char **tokens = read_input_makefile("temp_file");
    int i = 0;
    while (*(tokens + i) != NULL) {
        printf("%s\n", *(tokens + i));
        free (*(tokens + i));
        i++;
    }
    free (tokens);

    return 0;
}
