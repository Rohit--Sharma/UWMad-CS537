#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *munch1(Queue *queue1, Queue *queue2)
{
    char *string;
    string = DequeueString(queue1);
    
    for (int i=0; string[i]!=NULL; i++){
	if (string[i]==" ")
	    string[i] = '*';
    }

    EnqueueString(queue2, string);
}

void *munch2(Queue *queue2, Queue *queue3)
{
    char *string;
    string = DequeueString(queue2);
    
    for (int i=0; string[i]!=NULL; i++){
	string[i] = toupper(string[i])
    }

    EnqueueString(queue3, string);
}


