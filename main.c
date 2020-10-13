/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera, Julien de Castelnau
 * @date 10/13/2020
 * @file queue.c
 */

#include <stdlib.h>
#include "workers.h"

int main() {
   pthread_t reader_id;
   pthread_t munch1_id;
   pthread_t munch2_id;
   pthread_t writer_id;

   const int QUEUE_SIZE = 10;
   Queue* Munch1Queue = CreateStringQueue(QUEUE_SIZE);
   Queue* Munch2Queue = CreateStringQueue(QUEUE_SIZE);
   Queue* WriteQueue = CreateStringQueue(QUEUE_SIZE);

   // Spawn threads
   if (pthread_create(&reader_id, NULL, Reader, Munch1Queue) != 0) {
      perror("Error in Reader thread creation.");
      exit(EXIT_FAILURE);
   }
   
   // Munchers take an array of {inputQueue, outputQueue} as parameters
   Queue* munch1Queues[2] = {Munch1Queue, Munch2Queue};
   if (pthread_create(&munch1_id, NULL, Munch1, munch1Queues) != 0) {
      perror("Error in Munch1 thread creation.");
      exit(EXIT_FAILURE);
   }

   Queue* munch2Queues[2] = {Munch2Queue, WriteQueue};
   if (pthread_create(&munch2_id, NULL, Munch2, munch2Queues) != 0) {
      perror("Error in Munch2 thread creation.");
      exit(EXIT_FAILURE);
   }
   if (pthread_create(&writer_id, NULL, Writer, WriteQueue) != 0) {
      perror("Error in Writer thread creation.");
      exit(EXIT_FAILURE);
   }

   // Merge threads
   if (pthread_join(reader_id, NULL) != 0) {
      perror("Error in Reader thread termination.");
      exit(EXIT_FAILURE);
   }
   if (pthread_join(munch1_id, NULL) != 0) {
      perror("Error in Munch1 thread termination.");
      exit(EXIT_FAILURE);
   }
   if (pthread_join(munch2_id, NULL) != 0) {
      perror("Error in Munch2 thread termination.");
      exit(EXIT_FAILURE);
   }
   if (pthread_join(writer_id, NULL) != 0) {
      // remember Writer prints total # lines upon termination
      perror("Error in Writer thread termination.");
      exit(EXIT_FAILURE);
   }

   // Print statistics
   fprintf(stderr, "STATISTICS FOR Munch1Queue\n");
   PrintQueueStats(Munch1Queue);
   fprintf(stderr, "STATISTICS FOR Munch2Queue\n");
   PrintQueueStats(Munch2Queue);
   fprintf(stderr, "STATISTICS FOR WriteQueue\n");
   PrintQueueStats(WriteQueue);

   return 0;
}