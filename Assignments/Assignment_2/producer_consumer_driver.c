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
#include <pthread.h>
#include <semaphore.h>
#include "producer_consumer_header.h"

const int MAX_LINE_LEN = 20;
const int QUEUE_SIZE = 10;

int main()
{
    pthread_t reader_t, munch1_t, munch2_t, writer_t;
    Queue *Q1, *Q2, *Q3;
    Q1 = createQueue(QUEUE_SIZE);
    Q2 = createQueue(QUEUE_SIZE);
    Q3 = createQueue(QUEUE_SIZE);

    pthread_create(&reader_t, NULL, reader, (void *) Q1);

    pthread_param *munch1_param = (pthread_param *) malloc(sizeof(pthread_param)), 
        *munch2_param = (pthread_param *) malloc(sizeof(pthread_param));
    munch1_param->input = Q1;
    munch1_param->output = Q2;
    munch2_param->input = Q2;
    munch2_param->output = Q3;

    pthread_create(&munch1_t, NULL, munch1, (void *) munch1_param);
    pthread_create(&munch2_t, NULL, munch2, (void *) munch2_param);
    pthread_create(&writer_t, NULL, writer, (void *) Q3);

    pthread_join(reader_t, NULL);
    pthread_join(munch1_t, NULL);
    pthread_join(munch2_t, NULL);
    pthread_join(writer_t, NULL);

    PrintQueueStats(Q1);
    PrintQueueStats(Q2);
    PrintQueueStats(Q3);

    return 0;
}
