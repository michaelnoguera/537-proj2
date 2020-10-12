#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include "workers.h"

Queue *Munch1Queue;
Queue *Munch2Queue;
Queue *WriteQueue;

/**
 * Custom getline() implementation that meets the assignment requirements.
 * Reads from stdin.
 */
char* readerReadLine() {
    char c;
    unsigned int i = 0;

    const size_t BUFSIZE = 4096;
    char* buf = (char*)calloc(BUFSIZE, sizeof(char));
    
    while (i < BUFSIZE) {
        c = getchar();
        //printf("%c", c);
        if (i == 0 && c == EOF) return NULL;
        if (c == EOF) {
            buf[i] = '\0';
            return buf;
        }
        if(i < BUFSIZE-1 && c == '\n') {
            buf[i] = '\n';
            buf[i+1] = '\0';
            return buf;
        }

        buf[i] = c;
        i++;
    }
    
    fprintf(stderr, "line too long, skipping\n");
    free(buf); // not gonna use it
    
    while (c != EOF && c != '\n') {
        c = getchar();
    }
    //printf("Finished consuming throwaway line: %x", c);
    putc(c, stdin);

    return '\0';
}

void *Reader() {
    char* line;
    while((line = readerReadLine()) != NULL) {
        //printf("[READER] %p = %s\n", line, line);
        EnqueueString(Munch1Queue, line);
    }
    //printf("[READER] Finished, adding NULL sentinel value.\n");
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

    while ((line = DequeueString(WriteQueue)) != NULL) {
        //printf("[WRITER RECIEVED] %p = %s\n", line, line);
        for (int i = 0; i < strlen(line); i++) {
            putchar(line[i]);
        }
        free(line);
    }
    
    printf("%d\n", getCount(WriteQueue->dequeueStat));
    //printf("[WRITER] Finished, recieved NULL sentinel value.\n");

    pthread_exit(NULL); // return successfully
}