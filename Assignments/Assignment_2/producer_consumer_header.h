#include <semaphore.h>

extern const int QUEUE_SIZE;

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

#ifndef STRUCT_PTHREAD_PARAM
#define STRUCT_PTHREAD_PARAM 
typedef struct pthred_param
{
    Queue *input;
    Queue *output;
} pthread_param;
#endif

Queue* createQueue(int capacity);
void EnqueueString(Queue* queue, char *string);
char* DequeueString(Queue* queue);
void PrintQueueStats(Queue *queue);

void *reader(void *out_queue);
void *munch1(void *queues);
void *munch2(void *queues);
void *writer(void *in_queue);
