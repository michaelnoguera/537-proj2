/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera, Julien de Castelnau
 * @date 10/13/2020
 * @file workers.c
 */

#define _GNU_SOURCE

#include "workers.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * Custom getline() implementation that meets the assignment requirements.
 * Reads from stdin.
 * 
 * @param overflow flag to set if input size greater than buffer
 * @param endOfFile flag to set if EOF reached
 * 
 * @return pointer to heap-allocated string on success, NULL and the appropriate
 * flag on failure
 */
char* readerReadLine(bool* overflow, bool* endOfFile) {
    char c;
    unsigned int i = 0;

    // allocate a buffer to hold the line
    const size_t BUFSIZE = 4096;
    char* buf = (char*)calloc(BUFSIZE, sizeof(char));
    if (buf == NULL) {
        perror("Error allocating memory to hold line.");
        exit(EXIT_FAILURE);
    }

    // read input one character at at time
    while (i < BUFSIZE) {
        c = getchar();

        // CASE 1: EOF on newline or EOF left on buffer by CASE 2
        // -> return sentinel with EOF flag set
        if (i == 0 && c == EOF) {
            *endOfFile = true;
            free(buf);
            return NULL;
        }

        // CASE 2: current string ended, either by newline or EOF
        // -> return string
        // -> if EOF was reached, CASE 1 will be triggered upon next read
        if (c == EOF || c == '\n') {
            buf[i] = '\0';
            return buf;
        }

        // CASE 3: nothing special, store the character
        buf[i] = c;
        i++;
    }

    // OVERFLOW: line length exceeds BUFSIZE
    // -> consume the remainder of the oversized line
    // -> return sentinel with overflow flag set
    *overflow = true;
    fprintf(stderr, "line too long, skipping\n");
    free(buf);

    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    return NULL;  // return sentinel with overflow flag set
}

/**
 * Reader thread. Consumes input from stdin and parses it into the first
 * Muncher's queue.
 * 
 * @param outputQueue queue where Reader should put strings it reads
 */
void* Reader(Queue* outputQueue) {
    char* line;
    bool overflow = false;
    bool endOfFile = false;
    while (1) {
        // READ A LINE
        overflow = false;
        line = readerReadLine(&overflow, &endOfFile);

        // PARSE THE LINE
        if (overflow) continue;  // don't enqueue anything for oversized lines
        if (endOfFile) break;

        // STORE THE LINE
        EnqueueString(outputQueue, line);
    }
    // OUT OF LINES -> enqueue null sentinel value
    EnqueueString(outputQueue, NULL);

    pthread_exit(NULL);
}

/**
 * First Muncher. Replaces spaces with asterisks as it reads from Munch1Queue,
 * and enqueues results to Munch2Queue.
 * 
 * @param queue array of two pointers to queues. `queue[0]` is used for input to
 *  this function and `queue[1]` is used for output.
 */
void* Munch1(Queue** queue) {
    assert(queue[0] != NULL && queue[1] != NULL);

    char* line;
    char* occurrence;

    while ((line = DequeueString(queue[0])) != NULL) {
        // replace spaces with asterisks
        while ((occurrence = (char*)index(line, ' '))) {
            *occurrence = '*';
        }
        EnqueueString(queue[1], line);
    }

    // enqueue null sentinel value
    EnqueueString(queue[1], NULL);
    pthread_exit(NULL);
}

/**
 * Second Muncher. Capitalizes all lowercase letters as it reads from
 * Munch2Queue, and enqueues the results in WriteQueue.
 * 
 * @param queue array of two pointers to queues. `queue[0]` is used for input to
 *  this function and `queue[1]` is used for output.
 */
void* Munch2(Queue** queue) {
    assert(queue[0] != NULL && queue[1] != NULL);

    char* line;
    while ((line = DequeueString(queue[0])) != NULL) {
        // convert lowercase to uppercase
        int i = 0;
        while (line[i] != '\0') {
            if (islower(line[i])) {
                line[i] = toupper(line[i]);
            }
            i++;
        }
        EnqueueString(queue[1], line);
    }

    // enqueue null sentinel value
    EnqueueString(queue[1], NULL);
    pthread_exit(NULL);
}

/**
 * Writer thread. End of the 'pipeline', this takes strings from the queue and
 * prints them to stdout.
 * 
 * @param inputQueue queue that Writer should read from
 */
void* Writer(Queue* inputQueue) {
    char* line;

    int i = 0;
    while ((line = DequeueString(inputQueue)) != NULL) {
        printf("%s\n", line);
        free(line);
        i++;
    }

    printf("%d\n", i);

    pthread_exit(NULL);  // return successfully
}