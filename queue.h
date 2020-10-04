#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/** 
 * enqueueCount      A count of the number of strings enqueued on this queue.
 * dequeueCount      A count of the number of strings dequeued on this queue. We would expect that when the program exits, the two count values are equal.
 * enqueueBlockCount A count of the number of times that an enqueue was attempted but blocked.
 * dequeueBlockCount A count of the number of times that a dequeue was attempted but blocked.
 */

/**
 * Node type for Queue
 * 
 * Contains its own lock.
 * Holds a single string value and `next` pointer to maintain list structure.
 */
typedef struct q_node_t {
    sem_t lock;            /**< mutex lock on this node */
    volatile struct q_node_t* next; /**< pointer to the next node in the list */
    const char value[];    /**< the value stored in the node. */ 
} Node;

struct q_stat_t {
    sem_t lock;
    volatile int count;
    volatile time_t time;
};

/** Thread-safe semi-parallel implementation of a singly-linked queue
 * 
 */
typedef struct Queue {
    sem_t space;

    sem_t head_ptr_lock;
    struct q_node_t* head;
    sem_t tail_ptr_lock;
    struct q_node_t* tail;

    struct q_stat_t enqueue;
    //|- contains lock, count, time

    struct q_stat_t dequeue;
    // -> contains lock, count, time
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
