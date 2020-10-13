/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera (noguera) <mnoguera(at)wisc.edu>
 * @author Julien de Castelnau (de-castelnau) <decastelnau(at)wisc.edu>
 * @date 10/13/2020
 * @file queue.h
 */

#ifndef __QUEUE__
#include <pthread.h>
#include <time.h>

#include "stat.h"

/** Thread-safe implementation of a dynamically-allocated array-based queue
 */
typedef struct Queue {
    pthread_mutex_t lock;
    pthread_cond_t empty;
    pthread_cond_t full;

    Stat *enqueueStat;  /// total time spent doing enqueues
    Stat *dequeueStat;  /// total time spent doing dequeues

    int head;     /// next index to enqueue at
    int tail;     /// next index to dequeue from
    size_t size;  /// max items the queue can hold
    char *item[];

} Queue;

/**
 * Initializes a new empty Queue.
 * 
 * @param size queue capacity
 * 
 * @return pointer to new heap-allocated Queue
 */
Queue *CreateStringQueue(int size);

/**
 * Adds a new Node to the end of a Queue.
 * 
 * @param q The queue to enqueue to.
 * @param value The value of the new node.
 */
void EnqueueString(Queue *q, char *string);

/**
 * Removes a node from the end of a queue
 * 
 * @param q The queue from which to dequeue from
 * @return the char* pointer that was removed, which can point to NULL
 */
char *DequeueString(Queue *q);

/**
 * Prints queue statistics to stderr:
 *  enqueueCount - A count of the number of strings enqueued on this queue.
 *  dequeueCount - A count of the number of strings dequeued on this queue.
 *  enqueueTime - Total time spent performing enqueue operations. 
 *  dequeueTime - Total time spent performing dequeue operations.
 * 
 * @param q The Queue for which statistics are to be printed.
 */
void PrintQueueStats(Queue *q);

#define __QUEUE__
#endif
