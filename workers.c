#define _GNU_SOURCE
#include "workers.h"

Queue *Munch1Queue;
Queue *Munch2Queue;
Queue *WriteQueue;

void *Reader() {
    size_t bufsize = 0;
    char *line;
    while(1) {
        // read line into temp heap-alloc'd buffer
        // "If *lineptr is set to NULL and *n is set 0 before the call, 
        // then getline() will allocate a buffer for storing the line.  
        // This buffer should be freed by the user program even if getline() failed." See getline(3)
        line = NULL;
        bufsize = 0;
        if (getline(&line, &bufsize, stdin) < 0) {
            // Enqueue NULL pointer. This indicates there is no more input.
            EnqueueString(Munch1Queue, NULL); 
            break;
        } else {
            //printf("1: %x\n", line);
            EnqueueString(Munch1Queue, line);
        }        
	}
    pthread_exit(NULL); // return successfully
}

void *Munch1() {
    char* line;
    char* occurrence;
    while ( (line = DequeueString(Munch1Queue)) ) {
        //printf("4: %x\n", line);
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
    while ( (line = DequeueString(Munch2Queue)) ) {
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
    while ( (line = DequeueString(WriteQueue)) ) {
        printf("%s", line);
    }
    pthread_exit(NULL); // return successfully
}