///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 14, 2018
//
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "producer_consumer_header.h"

extern const int MAX_LINE_LEN;
extern const int QUEUE_SIZE;

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
read_line_val *read_line(int buff_size)
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
        c = getchar();
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
    while ((c = getchar()) != '\n' && c != EOF);
    if (c == EOF)
        new_line_val->has_eof = 1;

    return new_line_val;
}

//Reader thread reads a line from stdio and enqueues it into Q1 for Munch1 to access
void *reader(void *out_queue)
{
    size_t max_line_len = MAX_LINE_LEN;

    char *line = NULL;
    do {
        read_line_val *line_struct = read_line(max_line_len);
        if (line_struct->buff_size_exceeding == 1) {
	    //If line size exceeds buffer size, then line is discarded and message is printed to stderr
            fprintf(stderr, "Line size exceeds the buffer size %zu\n", max_line_len);
            continue;
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
        // printf("Reading at %x: %s\n", line, line);
        EnqueueString((Queue *) out_queue, line);
    } while (line != NULL);

    // Enqueue NULL as a terminating condition for next queue
    EnqueueString((Queue *) out_queue, NULL);
    pthread_exit(NULL);
    return NULL;
}

//Munch1 dequeues string from Q1, processes it and enqueues the string to Q2
void *munch1(void *queues)
{
    char *string;
    do {
        string = DequeueString(((pthread_param *)queues)->input);
        
        if (string == NULL)
            break;

	//Replace every space with a *
        for (int i=0; string[i] != '\0'; i++) {
            if (string[i] == ' ')
                string[i] = '*';
        }

        EnqueueString(((pthread_param *)queues)->output, string);
    } while (string != NULL);

    // Enqueue NULL as a terminating condition for next queue
    EnqueueString(((pthread_param *)queues)->output, NULL);
    pthread_exit(NULL);

    return NULL;
}

//Munch2 dequeues string from Q2, processes it and enqueues the string to Q3
void *munch2(void *queues)
{
    char *string;
    do {
        string = DequeueString(((pthread_param *)queues)->input);
        
        if (string == NULL)
            break;

        // printf("Before Munch2: %s\n", string);

        for (int i=0; string[i] != '\0'; i++) {
            if (islower(string[i]))
                string[i] = toupper(string[i]);
        }

        // printf("After Munch2: %s\n", string);

        EnqueueString(((pthread_param *)queues)->output, string);
    } while (string != NULL);

    // Enqueue NULL as a terminating condition for next queue
    EnqueueString(((pthread_param *)queues)->output, NULL);
    pthread_exit(NULL);

    return NULL;
}

void *writer(void *in_queue)
{
    char *string = DequeueString((Queue *)in_queue);
    while (string != NULL) {
        fprintf(stdout, "%s\n", string);
        free(string);
        string = DequeueString((Queue *)in_queue);
    }
    fprintf(stdout, "\nTotal number of processed strings: %d\n", ((Queue *)in_queue)->enqueueCount - 1);
    pthread_exit(NULL);
    return NULL;
}
