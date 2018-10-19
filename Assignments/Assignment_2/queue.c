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

Queue* createQueue(int capacity) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\n");
	free(queue);
        return NULL;
    }

    queue->size = capacity; 
    queue->no_of_elements = 0;
    queue->head = 0;  
    queue->tail = 0;
    queue->enqueueCount = 0;
    queue->dequeueCount = 0;
    queue->enqueueBlockCount = 0;
    queue->dequeueBlockCount = 0;
    queue->string = (char **) malloc(sizeof(char *) * capacity);
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\n");
	free(queue);
        return NULL;
    }

    for (int i = 0; i < capacity; i++)    
	    queue->string[i] = NULL;
    
    // if sem_init() returns -1, then there was an error in initializing sem
    if (sem_init(&(queue->stat_block_mutex), 0, 1) == -1 || sem_init(&(queue->mutex), 0, 1) == -1 || 
        sem_init(&(queue->full), 0, 0) == -1 || sem_init(&(queue->empty), 0, capacity) == -1) {
        fprintf(stderr, "Error in initializing the semaphore\n");
        if (errno == EINVAL) {
            fprintf(stderr, "Semaphore value exceeds SEM_VALUE_MAX\n");
        }
        else if (errno == ENOSYS) {
            fprintf(stderr, "pshared is nonzero, but the system does not support process-shared semaphores\n");
        }
	free(queue);
        return NULL;
    }

    return queue;
}

void EnqueueString(Queue* queue, char *string) 
{
    sem_wait(&(queue->stat_block_mutex));
    if (queue->size == queue->no_of_elements) {
	    queue->enqueueBlockCount++;
    }
    sem_post(&(queue->stat_block_mutex));

    sem_wait(&(queue->empty));
    sem_wait(&(queue->mutex));

    queue->string[queue->tail] = string; 
    queue->tail = (queue->tail + 1) % queue->size;
    queue->no_of_elements++;
    queue->enqueueCount++;

    sem_post(&queue->mutex);
    sem_post(&queue->full);
}

char* DequeueString(Queue* queue) 
{
    sem_wait(&(queue->stat_block_mutex));
    if (queue->no_of_elements==0){
	    queue->dequeueBlockCount++;
    }
    sem_post(&(queue->stat_block_mutex));

    sem_wait(&queue->full);
    sem_wait(&queue->mutex);

    char* queue_string = queue->string[queue->head];
    queue->head = (queue->head + 1) % queue->size; 
    queue->no_of_elements--;
    queue->dequeueCount++;
     
    sem_post(&queue->mutex);
    sem_post(&queue->empty);
    return queue_string;
}

void PrintQueueStats(Queue *q)
{
	fprintf(stderr, "1. Enqueue Count: %d\n2. Dequeue Count: %d\n3. Enqueue Block Count: %d\n4. Dequeue Block Count: %d\n", q->enqueueCount - 1, q->dequeueCount - 1, q->enqueueBlockCount, q->dequeueBlockCount);
}
