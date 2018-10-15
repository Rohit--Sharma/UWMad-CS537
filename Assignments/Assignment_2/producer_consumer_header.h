#include <semaphore.h>

#define MAX_LINE_LEN 20
#define QUEUE_SIZE 10

#ifndef STRUCT_STAT_STATM_CMDLINE
#define STRUCT_STAT_STATM_CMDLINE
typedef struct Queue
{
    int head, tail, size, no_of_elements;
    int enqueueCount, dequeueCount, enqueueBlockCount, dequeueBlockCount;
    char *string[QUEUE_SIZE];
    sem_t mutex, full, empty;
} Queue;
#endif

#ifndef STRUCT_PTHREAD_PARAM
    #define STRUCT_PTHREAD_PARAM typedef struct pthred_param
    {
        Queue *input;
        Queue *output;
    } pthread_param;
#endif

Queue* createQueue(int capacity);
void EnqueueString(Queue* queue, char *string);
char* DequeueString(Queue* queue);
