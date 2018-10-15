#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "producer_consumer_header.h"

void *reader(void *out_queue)
{
    char *line = NULL;
    size_t max_line_len = MAX_LINE_LEN + 1;
    while (getline(&line, &max_line_len, stdin) != -1) {
        printf("Reading: %s", line);
        printf("Head before: %d\n", ((Queue *)out_queue)->head);
        EnqueueString((Queue *)out_queue, line);
        printf("Head after: %d\n", ((Queue *)out_queue)->head);
    }
    return NULL;
}

void *munch1(void *queues)
{
    char *string;
    string = DequeueString(((pthread_param *)queues)->input);
    
    printf("Before Munch1: %s", string);  

    for (int i=0; string[i] != '\0'; i++) {
        if (string[i] == ' ')
            string[i] = '*';
    }

    printf("After Munch1: %s", string);

    EnqueueString(((pthread_param *)queues)->output, string);
    return NULL;
}

void *munch2(void *queues)
{
    char *string;
    string = DequeueString(((pthread_param *)queues)->input);
    
    printf("Before Munch2: %s", string);

    for (int i=0; string[i] != '\0'; i++) {
        if (islower(string[i]))
    	    string[i] = toupper(string[i]);
    }

    printf("After Munch2: %s", string);

    EnqueueString(((pthread_param *)queues)->output, string);
    return NULL;
}

void *writer(void *in_queue)
{
    char *string;
    string = DequeueString((Queue *)in_queue);

    printf("Writing: %s", string);
    return NULL;
}

/*
int main()
{
    reader(NULL);
    return 0;
}
*/
