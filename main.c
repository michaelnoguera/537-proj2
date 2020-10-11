/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera, Julien de Castelnau
 * @date 10/13/2020
 * @file queue.c
 */

#include <stdlib.h>
#include "workers.h"

extern Queue *Munch1Queue;
extern Queue *Munch2Queue;
extern Queue *WriteQueue;

int main() {
   pthread_t reader_id;
   pthread_t munch1_id;
   pthread_t munch2_id;
   pthread_t writer_id;

   Munch1Queue = CreateStringQueue(10);
   Munch2Queue = CreateStringQueue(10);
   WriteQueue = CreateStringQueue(10);

   /*
   char* test = (char*)malloc(sizeof(char) * 16);
   char* test2 = (char*)malloc(sizeof(char) * 16);
   EnqueueString(Munch1Queue, test);
   EnqueueString(Munch1Queue, test2);
   printf("%s", DequeueString(Munch1Queue));
   printf("%s", DequeueString(Munch1Queue));
   */

   pthread_create(&reader_id, NULL, Reader,NULL);
   pthread_create(&munch1_id, NULL, Munch1,NULL);
   pthread_create(&munch2_id, NULL, Munch2,NULL);
   pthread_create(&writer_id, NULL, Writer,NULL);

   pthread_join(reader_id, NULL);
   pthread_join(munch1_id, NULL);
   pthread_join(munch2_id, NULL);
   pthread_join(writer_id, NULL);

   fprintf(stderr, "\nSTATISTICS FOR Munch1Queue\n");
   PrintQueueStats(Munch1Queue);
   fprintf(stderr, "STATISTICS FOR Munch2Queue\n");
   PrintQueueStats(Munch2Queue);
   fprintf(stderr, "STATISTICS FOR WriteQueue\n");
   PrintQueueStats(WriteQueue);

   return 0;
}