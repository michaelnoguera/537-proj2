#define _GNU_SOURCE

#include <stdlib.h>

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
    
    while (i < 4096) {
        c = getchar();
        if (i == 0 && c == EOF) return NULL;
        if (c == '\n' || c == EOF) {
            buf[i] = '\0';
            return buf;
        }
        buf[i] = c;
        i++;
    }
    
    perror("line too long, skipping");
    
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    return '\0';
}

void *Reader() {
    char* line;
    while((line = readerReadLine()) != NULL) {
        printf("Just read: %s, %p", line, line);
        EnqueueString(Munch1Queue, readerReadLine());
    }
    EnqueueString(Munch1Queue, NULL);

    pthread_exit(NULL); // return successfully
}

void *Munch1() {
    char* line;
    char* occurrence;
    while ((line = DequeueString(Munch1Queue)) != NULL) {
        while ( (occurrence = (char*)index(line, ' ')) ) {
            *occurrence = '*';
        }
        EnqueueString(Munch2Queue, line);
    }
    EnqueueString(Munch2Queue, NULL);
    pthread_exit(NULL); // return successfully
}

void *Munch2() {
    char* line;
    while ((line = DequeueString(Munch2Queue)) != NULL) {
        int index = 0;
        while (line[index] != 0) {
            if(islower(line[index])) {
                line[index] = toupper(line[index]);
            }
            index++;
        }
        EnqueueString(WriteQueue, line);
    }
    EnqueueString(WriteQueue, NULL);
    pthread_exit(NULL); // return successfully
}

void *Writer() {
    char* line;

    while ((line = DequeueString(WriteQueue)) != NULL) {
        printf("%s", line);
        free(line);
    }

    pthread_exit(NULL); // return successfully
}