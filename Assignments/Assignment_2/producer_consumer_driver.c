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


int main(int argc, char *argv[])
{
    pthread_t reader_t, munch1_t, munch2_t, writer_t;
    Queue *Q1, *Q2, *Q3;
    Q1 = createQueue(QUEUE_SIZE);
    Q2 = createQueue(QUEUE_SIZE);
    Q3 = createQueue(QUEUE_SIZE);

    pthread_create(&reader_t, NULL, reader, Q1);
    pthread_create(&munch1_t, NULL, munch1, Q1, Q2);
    pthread_create(&munch2_t, NULL, munch2, Q2, Q3);
    pthread_create(&writer_t, NULL, writer, Q3);
    pthread_join(reader_t, NULL);
    pthread_join(munch1_t, NULL);
    pthread_join(munch2_t, NULL);
    pthread_join(writer_t, NULL);
    return 0;
}
