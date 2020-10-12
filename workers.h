#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <pthread.h>

#include "queue.h"

#ifndef __WORKERS__
extern Queue *Munch1Queue;
extern Queue *Munch2Queue;
extern Queue *WriteQueue;

void *Reader();

void *Munch1();

void *Munch2();

void *Writer();
#define __WORKERS__
#endif