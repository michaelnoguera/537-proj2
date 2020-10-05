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

    if (pthread_mutex_init(&(q->lock), 1) != 0) {
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

void EnqueueString(Queue *q, const char *string) {
    if (q == NULL) {
        perror("Can't add an element to NULL.");
        exit(EXIT_FAILURE);
    }

    // TODO: special case for cleanup with NULL sentinel value

    pthread_mutex_lock(&q->lock);

    // WAIT UNTIL SPACE IF NECESSARY
    while (q->head == (q->tail+1) % (q->size)) pthread_cond_wait(&q->full, &q->lock);
    
    // write at index head
    assert(q->item[q->head] == NULL);

    char* new = malloc(sizeof(char)*strlen(string));
    if (new == NULL) {
        perror("Error allocating memory to hold provided string.");
        exit(EXIT_FAILURE);
    }

    q->item[q->head] = strncpy(new, string, strlen(string));

    // advance head ptr
    q->head = (q->head + 1) % q->size;
    pthread_cond_signal(&q->empty);
	
    pthread_mutex_unlock(&q->lock);
}

char* DequeueString(Queue *q) {
    if (q == NULL) {
        perror("Can't add an element to NULL.");
        exit(EXIT_FAILURE);
    }

    // TODO: special case for cleanup with NULL sentinel value

    pthread_mutex_lock(&q->lock);

    // WAIT UNTIL VALUE IF NECESSARY
    while (q->head == q->tail) pthread_cond_wait(&q->empty, &q->lock);

    // advance tail ptr
    q->tail = (q->tail + 1) % q->size;
    pthread_cond_signal(&q->full);

    pthread_mutex_unlock(&q->lock);

    return 'a';
}