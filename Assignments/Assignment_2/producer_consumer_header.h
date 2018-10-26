///////////////////////////////////////////////////////////////////////////////
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 14, 2018
//
///////////////////////////////////////////////////////////////////////////////
#include <semaphore.h>

//Queue definition
#ifndef STRUCT_STAT_STATM_CMDLINE
#define STRUCT_STAT_STATM_CMDLINE
typedef struct Queue
{
    int head, tail, size, no_of_elements;
    int enqueueCount, dequeueCount, enqueueBlockCount, dequeueBlockCount;
    char **string;
    sem_t mutex, full, empty, stat_block_mutex;
} Queue;
#endif

//This structure is used to pass the Queues as arguments to the threads
#ifndef STRUCT_PTHREAD_PARAM
#define STRUCT_PTHREAD_PARAM 
typedef struct pthred_param
{
    Queue *input;
    Queue *output;
} pthread_param;
#endif

//Function definitions for the Queue methods
Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);

//Function definitions for the Threads
void *reader(void *out_queue);
void *munch1(void *queues);
void *munch2(void *queues);
void *writer(void *in_queue);
