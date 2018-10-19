///////////////////////////////////////////////////////////////////////////////
//
// Description:     Driver method where execution begins
// Course:          CS537 - Introduction to Operating Systems (Fall-2018)
// Organization:    University of Wisconsin-Madison
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      September 26, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "producer_consumer_header.h"

const int MAX_LINE_LEN = 7;
const int QUEUE_SIZE = 10;

int main()
{
    pthread_t reader_t, munch1_t, munch2_t, writer_t;
    Queue *Q1, *Q2, *Q3;
    Q1 = createQueue(QUEUE_SIZE);
    Q2 = createQueue(QUEUE_SIZE);
    Q3 = createQueue(QUEUE_SIZE);
    int error_num = 0;

    pthread_param *munch1_param = (pthread_param *) malloc(sizeof(pthread_param)), 
        *munch2_param = (pthread_param *) malloc(sizeof(pthread_param));
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\nExiting...\n");
        return -1;
    }

    munch1_param->input = Q1;
    munch1_param->output = Q2;
    munch2_param->input = Q2;
    munch2_param->output = Q3;

    if ((error_num = pthread_create(&reader_t, NULL, reader, (void *) Q1)) != 0 || 
      (error_num = pthread_create(&munch1_t, NULL, munch1, (void *) munch1_param)) != 0 ||
      (error_num = pthread_create(&munch2_t, NULL, munch2, (void *) munch2_param)) != 0 ||
      (error_num = pthread_create(&writer_t, NULL, writer, (void *) Q3)) != 0) {
        fprintf(stderr, "One or more thread(s) couldn't be created successfully.\n");
        if (error_num == EAGAIN)
            fprintf(stderr, "Insufficient resources to create a thread.\nExiting...\n");
        else if (error_num == EINVAL)
            fprintf(stderr, "Invalid settings in attr.\nExiting...\n");
        else if (error_num == EPERM)
            fprintf(stderr, "No permission to set the scheduling policy and parameters specified in attr.\nExiting...\n");
        return -1;
    }

    if ((error_num = pthread_join(reader_t, NULL)) != 0 || 
      (error_num = pthread_join(munch1_t, NULL)) != 0 || 
      (error_num = pthread_join(munch2_t, NULL)) != 0 || 
      (error_num = pthread_join(writer_t, NULL)) != 0) {
        fprintf(stderr, "One or more thread(s) couldn't be joined successfully.\n");
        if (error_num == EDEADLK)
            fprintf(stderr, "A deadlock was detected.\nExiting...\n");
        else if (error_num == EINVAL)
            fprintf(stderr, "The thread is not a joinable thread.\nExiting...\n");
        else if (error_num == ESRCH)
            fprintf(stderr, "No thread with thread id could be found.\nExiting...\n");
        return -1;
    }

    fprintf(stderr, "\n\nQueue Statistics:\n");
    fprintf(stderr, "\nQueue 1 (Reader to Munch1):\n");
    PrintQueueStats(Q1);
    fprintf(stderr, "\nQueue 2 (Munch1 to Munch2):\n");
    PrintQueueStats(Q2);
    fprintf(stderr, "\nQueue 3 (Munch2 to Writer):\n");
    PrintQueueStats(Q3);

    return 0;
}
