#include <pthread.h>
#include <time.h>

/** Thread-safe implementation of a dynamically-allocated array-based queue
 */
typedef struct Queue {
    pthread_mutex_t lock;
    pthread_cond_t empty;
    pthread_cond_t full;

    int enqueueCount;
    int dequeueCount;
    time_t enqueueTime; /// total time spent doing enqueues
    time_t dequeueTime; /// total time spent doing dequeues
    
    int head; /// next index to enqueue at
    int tail; /// next index to dequeue from
    size_t size; /// max items the queue can hold
    char* item[];

} Queue;

/**
 * Dynamically allocate a new Queue structure and initialize it with an array of
 * character points of length size. That means you'll malloc the queue structure
 * and then malloc the char ** array pointed to from that structure. Also remember
 * to any state and synchronization variables used in this structure.
 * 
 * The function returns a pointer to the new queue structure.
 * 
 * For testing purposes, create your Queue's with a size of 10. 
 */
Queue *CreateStringQueue(int size);

/**
 * This function places the pointer to the string at the end of queue q. If the
 * queue is full, then this function blocks until there is space available.
 */
void EnqueueString(Queue *q, char *string);

/**
 * This function removes a pointer to a string from the beginning of queue q. If
 * the queue is empty, then this function blocks until there is a string placed
 * into the queue. This function returns the pointer that was removed from the queue. 
 * 
 * Dequeue from an empty list returns NULL.
 */
char * DequeueString(Queue *q);

/**
 * This function prints the statistics for this queue (see the next section for details).
 *  enqueueCount      A count of the number of strings enqueued on this queue.
 *  dequeueCount      A count of the number of strings dequeued on this queue. We would expect that when the program exits, the two count values are equal.
 *  enqueueBlockCount A count of the number of times that an enqueue was attempted but blocked.
 *  dequeueBlockCount A count of the number of times that a dequeue was attempted but blocked.
 */
void PrintQueueStats(Queue *q);
