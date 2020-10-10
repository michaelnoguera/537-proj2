/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera, Julien de Castelnau
 * @date 10/13/2020
 * @file queue.c
 */

#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

/**
 * Initializes a new empty Queue.
 * 
 * @param size queue capacity
 * 
 * @return pointer to new heap-allocated Queue
 */
Queue* CreateStringQueue(const int size) {
    // Malloc the queue structure
    Queue* q = (Queue*)malloc(sizeof(Queue) + size*sizeof(char*));
    if (q == NULL) {
        perror("Error allocating memory for new Queue structure.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i<size; i++) {
        q->item[i] = NULL;
    }

    q->head = 0;
    q->tail = 0;
    q->size = size;

    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueTime = 0;
    q->dequeueTime = 0;

    if (pthread_mutex_init(&q->lock, NULL) != 0) {
        perror("Error initializing Queue mutex lock.\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&q->empty, NULL) != 0) {
        perror("Error initializing 'queue empty' condition variable.\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&q->full, NULL) != 0) {
        perror("Error initializing 'queue full' condition variable.\n");
        exit(EXIT_FAILURE);
    }
    

    return q;
}

/**
 * Adds a new Node to the end of a Queue.
 * 
 * @param list The list to append to.
 * @param value The value of the new Node.
 */

void EnqueueString(Queue *q, char *string) {
    if (q == NULL) {
        perror("Can't add an element to NULL.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    // WAIT UNTIL SPACE IF NECESSARY
    while (q->tail == (q->head+1) % (q->size)) pthread_cond_wait(&q->full, &q->lock);
    
    // write at index head
    assert(q->item[q->head] == NULL/*make super duper extra sure we're not going to overwrite an occupied space in the queue*/);
    q->item[q->head] = string;

    // advance head ptr
    q->head = (q->head + 1) % q->size;
    pthread_cond_signal(&q->empty);
	
    pthread_mutex_unlock(&q->lock);
}

char* DequeueString(Queue *q) {
    if (q == NULL) {
        perror("Can't remove an element from NULL.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    // TODO: special case for cleanup with NULL sentinel value

    // WAIT UNTIL VALUE IF NECESSARY
    while (q->tail == q->head) pthread_cond_wait(&q->empty, &q->lock);

    char* string = q->item[q->tail]; // retrieve from queue
    q->item[q->tail] = NULL; // null out old ptr
    q->tail = (q->tail + 1) % q->size; // advance tail ptr
    
    pthread_cond_signal(&q->full);

    pthread_mutex_unlock(&q->lock);

    //printf("3: %x\n", string);
    return string;
}