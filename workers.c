/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera, Julien de Castelnau
 * @date 10/13/2020
 * @file workers.c
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "workers.h"

Queue *Munch1Queue;
Queue *Munch2Queue;
Queue *WriteQueue;


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

    const size_t BUFSIZE = 4096;
    char* buf = (char*)calloc(BUFSIZE, sizeof(char));
    if (buf == NULL) {
        perror("Error allocating memory to hold line, or BUFSIZE was changed to zero.");
        exit(EXIT_FAILURE);
    }
    
    while (i < BUFSIZE) {
        c = getchar();
        //printf("%c", c);
        if (i == 0 && c == EOF) {
            *endOfFile = true;
            free(buf);
            return NULL;
        }
        if (c == EOF || c == '\n') {
            buf[i] = '\0';
            return buf;
        }

        buf[i] = c;
        i++;
    }

    *overflow = true;
    fprintf(stderr, "line too long, skipping\n");
    free(buf);
     
    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    //printf("Finished consuming throwaway line: %x", c);
    return NULL;
}

/**
 * Reader thread. Consumes input from stdin and parses it into the first
 * Muncher's queue.
 */
void *Reader() {
    char* line;
    bool overflow = false;
    bool endOfFile = false;
    while(1) {
        // READ A LINE
        overflow = false;
        line = readerReadLine(&overflow, &endOfFile);

        // PARSE THE LINE
        if (overflow) continue;
        if (endOfFile) break;
        
        // STORE THE LINE
        EnqueueString(Munch1Queue, line);
    }
    // OUT OF LINES -> enqueue null sentinel value
    EnqueueString(Munch1Queue, NULL);

    pthread_exit(NULL);
}

/**
 * First Muncher. Replaces spaces with asterisks as it reads from Munch1Queue,
 * and 
 */
void *Munch1() {
    char* line;
    char* occurrence;

    while ((line = DequeueString(Munch1Queue)) != NULL) {
        // replace spaces with asterisks
        while ( (occurrence = (char*)index(line, ' ')) ) {
            *occurrence = '*';
        }
        EnqueueString(Munch2Queue, line);
    }

    // enqueue null sentinel value
    EnqueueString(Munch2Queue, NULL);
    pthread_exit(NULL);
}

void *Munch2() {
    char* line;
    while ((line = DequeueString(Munch2Queue)) != NULL) {
        // convert lowercase to uppercase
        int i = 0;
        while (line[i] != '\0') {
            if(islower(line[i])) {
                line[i] = toupper(line[i]);
            }
            i++;
        }
        EnqueueString(WriteQueue, line);
    }

    // enqueue null sentinel value
    EnqueueString(WriteQueue, NULL);
    pthread_exit(NULL);
}

void *Writer() {
    char* line;

    int i = 0;
    while ((line = DequeueString(WriteQueue)) != NULL) {
        printf("%s\n", line);
        free(line);
        i++;
    }
    
    //printf("%d\n", i);

    pthread_exit(NULL); // return successfully
}