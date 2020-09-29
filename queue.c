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

// head                tail
// |                   |
//[O -> (O] -> [O) -> (O] ...
// > value char** -> char* in the heap

// O is node type: value, next_ptr, sem_t

// enqueue: lock current and next (head) for the duration of the operation
// dequeue: lock prev (second-to-last) and current (last) for the duration of the operation
// iterator: locks current, gets next, locks next, releases current, releases next, returns &next

/**
 * Initializes a new empty Queue.
 * Remember to call ll_free when done!
 * 
 * @return pointer to new heap-allocated Queue
 */
Queue* CreateStringQueue(int size) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        perror("Error allocating memory for new queue.\n");
        exit(EXIT_FAILURE);
    }

    assert(sem_init(q.lock, 0, 1) == 0);
    q->size = 0;
    q->capacity = size;
    q->head = NULL;
    q->tail = NULL;

    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    sem_post()

    return q;
}

Node* getNext(Node* curr) {

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

    sem_post(q->lock);
    if (q->size == q->capacity) {
        
    }

    // construct Node to add
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) {
        perror("Error allocating memory for new Node.\n");
        exit(EXIT_FAILURE);
    }
    sem_init(new->lock, 1);
    sem_post(new->lock);
    new->value = value;

    // attach Node to tail of list
    if (q->size == 0) {
        q->head = new;
        q->tail = new;
    } else {
        q->tail->next = new;
        q->tail = new;
    }
    q->size++;

    new->next = q->head;  // complete circular linking
}

/**
 * Gets the value of the Node at an index.
 * 
 * @param list The list to search.
 * @param index The index of the Node to be retrieved. Lists are zero-indexed.
 * @return The value of the Node, or NULL if no such Node exists.
 */
int ll_get(const Queue* queue, int index) {
    if (list == NULL) {
        perror("Can't get an element from NULL.");
        exit(EXIT_FAILURE);
    }
    
    if (list->size <= index) return -1;

    Node* curr = list->head;
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }

    return curr->value;
}

/**
 * Frees all memory from the specified Queue.
 * If  `ptr` is NULL, no operation is performed.
 * 
 * @param ptr Pointer to Queue to be freed.
 */
void ll_free(Queue* ptr) {
    if (ptr == NULL) return;
    
    // free all Nodes
    Node* tmp = NULL;
    while (ptr->head != ptr->tail) {
        tmp = ptr->head;
        ptr->head = ptr->head->next;
        free(tmp);
    }
    free(ptr->tail);

    // free list wrapper
    free(ptr);
}

/**
 * Prints all values from the specified Queue.
 * @author Julien de Castelnau
 * 
 * @param list The list to print.
 */
void ll_print(const Queue* list) {
    Node* curr = list->head;

    for (int i = 0; i < list->size; i++) {
        printf("%d, ", curr->value);
        curr = curr->next;
    }
    printf("\n");