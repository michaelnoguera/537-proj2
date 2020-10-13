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
    free(NULL);
     
    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    //printf("Finished consuming throwaway line: %x", c);
    return NULL;
}

void *Reader() {
    char* line;
    bool overflow;
    bool endOfFile;
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
    // OUT OF LINES
    EnqueueString(Munch1Queue, NULL);

    pthread_exit(NULL); // return successfully
}

void *Munch1() {
    char* line;
    char* occurrence;
    while ((line = DequeueString(Munch1Queue)) != NULL) {
        //printf("[MUNCH1] RECIEVED %p = %s\n", line, line);
        while ( (occurrence = (char*)index(line, ' ')) ) {
            *occurrence = '*';
        }
        //printf("[MUNCH1] OUTPUT %p = %s\n", line, line);
        EnqueueString(Munch2Queue, line);
    }
    //printf("[MUNCH1] Finished, adding NULL sentinel value.\n");

    EnqueueString(Munch2Queue, NULL);
    pthread_exit(NULL); // return successfully
}

void *Munch2() {
    char* line;
    while ((line = DequeueString(Munch2Queue)) != NULL) {
        //printf("[MUNCH2] RECIEVED %p = %s\n", line, line);
        int index = 0;
        while (line[index] != 0) {
            if(islower(line[index])) {
                line[index] = toupper(line[index]);
            }
            index++;
        }
        //printf("[MUNCH2] OUTPUT %p = %s\n", line, line);
        EnqueueString(WriteQueue, line);
    }
    //printf("[MUNCH2] Finished, adding NULL sentinel value.\n");
    EnqueueString(WriteQueue, NULL);
    pthread_exit(NULL); // return successfully
}

void *Writer() {
    char* line;

    int i = 0;
    while ((line = DequeueString(WriteQueue)) != NULL) {
        //printf("[WRITER RECIEVED] %p = %s\n", line, line);
        printf("%s\n", line);
        i++;
        free(line);
    }
    
    //printf("%d\n", i);
    //printf("[WRITER] Finished, recieved NULL sentinel value.\n");

    pthread_exit(NULL); // return successfully
}