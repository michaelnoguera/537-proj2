#include <stdio.h>
#include <pthread.h>

#include "queue.h"

extern Queue *Munch1Queue;
extern Queue *Munch2Queue;
extern Queue *WriteQueue;

void *Reader();

void *Munch1();

void *Munch2();

void *Writer();