/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera (noguera) <mnoguera(at)wisc.edu>
 * @author Julien de Castelnau (de-castelnau) <decastelnau(at)wisc.edu>
 * @date 10/13/2020
 * @file queue.c
 */

#include "queue.h"

#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
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
    Queue* q = (Queue*)malloc(sizeof(Queue) + size * sizeof(char*));
    if (q == NULL) {
        perror("Error allocating memory for new Queue structure.\n");
        exit(EXIT_FAILURE);
    }

    // Setup queue spots and internal fields
    for (int i = 0; i < size; i++) {
        q->item[i] = NULL;
    }

    q->head = 0;
    q->tail = 0;
    q->size = size;

    // Setup statistics: our stat type tracks both time and count for a given
    // operation
    q->dequeueStat = CreateQueueStatistic();
    q->enqueueStat = CreateQueueStatistic();

    // Setup synchronization
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
 * @param q The queue to enqueue to.
 * @param value The value of the new node.
 */
void EnqueueString(Queue* q, char* string) {
    if (q == NULL) {
        perror("Can't enqueue an item to NULL.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    clock_t temp_start = clock();

    // WAIT UNTIL SPACE IF NECESSARY
    while (q->tail == (q->head + 1) % (q->size))
        pthread_cond_wait(&q->full, &q->lock);

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

/**
 * Removes a node from the end of a queue
 * 
 * @param q The queue from which to dequeue from
 * @return the char* pointer that was removed, which can point to NULL
 */
char* DequeueString(Queue* q) {
    if (q == NULL) {
        perror("Can't dequeue an item from NULL.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    clock_t temp_start = clock();

    // WAIT UNTIL VALUE IF NECESSARY
    while (q->tail == q->head) pthread_cond_wait(&q->empty, &q->lock);

    char* string = q->item[q->tail];    // retrieve from queue
    q->item[q->tail] = NULL;            // null out old ptr
    q->tail = (q->tail + 1) % q->size;  // advance tail ptr
    incrementCount(q->dequeueStat);
    pthread_cond_signal(&q->full);

    addTime(q->dequeueStat, clock() - temp_start);

    pthread_mutex_unlock(&q->lock);

    return string;
}

/**
 * Prints queue statistics to stderr:
 *  enqueueCount - A count of the number of strings enqueued on this queue.
 *  dequeueCount - A count of the number of strings dequeued on this queue.
 *  enqueueTime - Total time spent performing enqueue operations. 
 *  dequeueTime - Total time spent performing dequeue operations.
 * 
 * @param q The Queue for which statistics are to be printed.
 */
void PrintQueueStats(Queue* q) {
    if (q == NULL) {
        perror("Can't print statistics for NULL queue.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&q->lock);

    double total_time_d = ((double)getTime(q->dequeueStat)) / CLOCKS_PER_SEC;
    double total_time_e = ((double)getTime(q->enqueueStat)) / CLOCKS_PER_SEC;

    fprintf(stderr, "Dequeue Count: %d\nEnqueue Count: %d\n",
            getCount(q->dequeueStat), getCount(q->enqueueStat));
    fprintf(stderr, "Dequeue Time: %f\nEnqueue Time: %f\n",
            total_time_d, total_time_e);

    pthread_mutex_unlock(&q->lock);
}