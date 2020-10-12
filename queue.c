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
    assert(size > 0);
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

    q->dequeueStat = CreateQueueStatistic();
    q->enqueueStat = CreateQueueStatistic();

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
        perror("Can't enqueue an item to NULL.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    clock_t temp_start = clock();

    //printf("  {ENQUEUE adding %s}\n", string);

    // WAIT UNTIL SPACE IF NECESSARY
    while (q->tail == (q->head+1) % (q->size)) pthread_cond_wait(&q->full, &q->lock);
    
    // write at index head
    assert(q->item[q->head] == NULL);
    q->item[q->head] = string;

    // advance head ptr
    q->head = (q->head + 1) % q->size;
    // increment stats counter
    incrementCount(q->enqueueStat);
    pthread_cond_signal(&q->empty);
	
    addTime(q->enqueueStat, clock() - temp_start);

    pthread_mutex_unlock(&q->lock);
}

char* DequeueString(Queue *q) {
    if (q == NULL) {
        perror("Can't dequeue an item from NULL.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    clock_t temp_start = clock();

    // TODO: special case for cleanup with NULL sentinel value

    // WAIT UNTIL VALUE IF NECESSARY
    while (q->tail == q->head) pthread_cond_wait(&q->empty, &q->lock);

    char* string = q->item[q->tail]; // retrieve from queue
    q->item[q->tail] = NULL; // null out old ptr
    q->tail = (q->tail + 1) % q->size; // advance tail ptr
    incrementCount(q->dequeueStat);
    pthread_cond_signal(&q->full);

    addTime(q->dequeueStat, clock() - temp_start);

    pthread_mutex_unlock(&q->lock);

    //printf("\n  {DEQUEUE returning %s}\n", string);
    return string;
}

void PrintQueueStats(Queue *q) {
    if (q == NULL) {
        perror("Can't print statistics for NULL queue.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    double total_time_d = ((double)getTime(q->dequeueStat))/CLOCKS_PER_SEC;
    double total_time_e = ((double)getTime(q->enqueueStat))/CLOCKS_PER_SEC;

    fprintf(stderr, "Dequeue Count: %d\nEnqueue Count: %d\n", getCount(q->dequeueStat), getCount(q->enqueueStat));
    fprintf(stderr, "Dequeue Time: %f\nEnqueue Time: %f\n", total_time_d, total_time_e);

    pthread_mutex_unlock(&q->lock);
}