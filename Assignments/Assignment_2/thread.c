#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "producer_consumer_header.h"

void *reader(Queue *out_queue)
{
    char *line = NULL;
    size_t max_line_len = MAX_LINE_LEN + 1;
    while (getline(&line, &max_line_len, stdin) != -1) {
        // printf("%s\n", line);
        EnqueueString(out_queue, line);
    }
}

void *munch1(Queue *queue1, Queue *queue2)
{
    char *string;
    string = DequeueString(queue1);
    
    for (int i=0; string[i] != '\0'; i++) {
        if (string[i] == ' ')
            string[i] = '*';
    }

    EnqueueString(queue2, string);
}

void *munch2(Queue *queue2, Queue *queue3)
{
    char *string;
    string = DequeueString(queue2);
    
    for (int i=0; string[i] != '\0'; i++) {
	    string[i] = toupper(string[i]);
    }

    EnqueueString(queue3, string);
}

void *write_queue(Queue *in_queue)
{
    char *string;
    string = DequeueString(in_queue);

    printf("%s\n", string);
}

/*
int main()
{
    reader(NULL);
    return 0;
}
*/
