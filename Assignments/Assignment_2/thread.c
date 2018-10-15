#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "producer_consumer_header.h"

char *read_line(int buff_size)
{
    char *new_line = NULL;
    if (getline(&new_line, &buff_size, stdin) != -1)
        return new_line;
    return NULL;
}

void *reader(void *out_queue)
{
    size_t max_line_len = MAX_LINE_LEN + 1;

    char *line;
    do {
        line = read_line(max_line_len);
        if (line == NULL)
            break;
        printf("Reading at %x: %s", line, line);
        EnqueueString((Queue *) out_queue, line);
    } while (line != NULL);

    // Enqueue NULL as a terminating condition for next queue
    EnqueueString((Queue *) out_queue, NULL);
    pthread_exit(NULL);
    return NULL;
}

void *munch1(void *queues)
{
    char *string;
    do {
        string = DequeueString(((pthread_param *)queues)->input);
        
        if (string == NULL)
            break;

        printf("Before Munch1: %s", string);  

        for (int i=0; string[i] != '\0'; i++) {
            if (string[i] == ' ')
                string[i] = '*';
        }

        printf("After Munch1: %s", string);

        EnqueueString(((pthread_param *)queues)->output, string);
    } while (string != NULL);

    // Enqueue NULL as a terminating condition for next queue
    EnqueueString(((pthread_param *)queues)->output, NULL);
    pthread_exit(NULL);

    return NULL;
}

void *munch2(void *queues)
{
    char *string;
    do {
        string = DequeueString(((pthread_param *)queues)->input);
        
        if (string == NULL)
            break;

        printf("Before Munch2: %s", string);

        for (int i=0; string[i] != '\0'; i++) {
            if (islower(string[i]))
                string[i] = toupper(string[i]);
        }

        printf("After Munch2: %s", string);

        EnqueueString(((pthread_param *)queues)->output, string);
    } while (string != NULL);

    // Enqueue NULL as a terminating condition for next queue
    EnqueueString(((pthread_param *)queues)->output, NULL);
    pthread_exit(NULL);

    return NULL;
}

void *writer(void *in_queue)
{
    char *string;
    do {
        string = DequeueString((Queue *)in_queue);
        if (string == NULL)
            break;

        printf("Writing: %s", string);
    } while (string != NULL);
    
    pthread_exit(NULL);
    return NULL;
}

/*
int main()
{
    reader(NULL);
    return 0;
}
*/
