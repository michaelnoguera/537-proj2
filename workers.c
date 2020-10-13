/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera (noguera) <mnoguera(at)wisc.edu>
 * @author Julien de Castelnau (de-castelnau) <decastelnau(at)wisc.edu>
 * @date 10/13/2020
 * @file workers.c
 */

#define _GNU_SOURCE

#include "workers.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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
static char* readerReadLine(bool* overflow, bool* endOfFile) {
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
 * @param outputQueue void* wrapping a Queue*, which is the queue where Reader
 *  should put strings it reads
 */
void* Reader(void* outputQueue) {
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
        EnqueueString((Queue*)outputQueue, line);
    }
    // OUT OF LINES -> enqueue null sentinel value
    EnqueueString((Queue*)outputQueue, NULL);

    pthread_exit(NULL);
}

/**
 * First Muncher. Replaces spaces with asterisks as it reads from Munch1Queue,
 * and enqueues results to Munch2Queue.
 * 
 * @param queues void* wrapping a Queue*[2], where `queue[0]` is used for input
 *  to this function and `queue[1]` is used for output.
 */
void* Munch1(void* queues) {
    Queue* in = ((Queue**)queues)[0];
    Queue* out = ((Queue**)queues)[1];
    assert(in != NULL && out != NULL);

    char* line;
    char* occurrence;

    while ((line = DequeueString(in)) != NULL) {
        // replace spaces with asterisks
        while ((occurrence = (char*)index(line, ' '))) {
            *occurrence = '*';
        }
        EnqueueString(out, line);
    }

    // enqueue null sentinel value
    EnqueueString(out, NULL);
    pthread_exit(NULL);
}

/**
 * Second Muncher. Capitalizes all lowercase letters as it reads from
 * Munch2Queue, and enqueues the results in WriteQueue.
 * 
 * @param queues void* wrapping a Queue*[2], where `queue[0]` is used for input
 *  to this function and `queue[1]` is used for output.
 */
void* Munch2(void* queues) {
    Queue* in = ((Queue**)queues)[0];
    Queue* out = ((Queue**)queues)[1];
    assert(in != NULL && out != NULL);

    char* line;
    while ((line = DequeueString(in)) != NULL) {
        // convert lowercase to uppercase
        int i = 0;
        while (line[i] != '\0') {
            if (islower(line[i])) {
                line[i] = toupper(line[i]);
            }
            i++;
        }
        EnqueueString(out, line);
    }

    // enqueue null sentinel value
    EnqueueString(out, NULL);
    pthread_exit(NULL);
}

/**
 * Writer thread. End of the 'pipeline', this takes strings from the queue and
 * prints them to stdout.
 * 
 * @param inputQueue void* wrapping a Queue*, which is the queue that Writer
 *  should read from
 */
void* Writer(void* inputQueue) {
    char* line;
    int i = 0;

    // write and free each string
    while ((line = DequeueString((Queue*)inputQueue)) != NULL) {
        printf("%s\n", line);
        free(line);
        i++;
    }

    printf("%d\n", i);  // print number of strings output

    pthread_exit(NULL);  // return successfully
}