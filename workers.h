/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera (noguera) <mnoguera(at)wisc.edu>
 * @author Julien de Castelnau (de-castelnau) <decastelnau(at)wisc.edu>
 * @date 10/13/2020
 * @file workers.h
 */

#ifndef __WORKERS__
#include "queue.h"

/**
 * Reader thread. Consumes input from stdin and parses it into the first
 * Muncher's queue.
 * 
 * @param outputQueue void* wrapping a Queue*, which is the queue where Reader
 *  should put strings it reads
 */
void* Reader(void* outputQueue);

/**
 * First Muncher. Replaces spaces with asterisks as it reads from Munch1Queue,
 * and enqueues results to Munch2Queue.
 * 
 * @param queues void* wrapping a Queue*[2], where `queue[0]` is used for input
 *  to this function and `queue[1]` is used for output.
 */
void* Munch1(void* queues);

/**
 * Second Muncher. Capitalizes all lowercase letters as it reads from
 * Munch2Queue, and enqueues the results in WriteQueue.
 * 
 * @param queues void* wrapping a Queue*[2], where `queue[0]` is used for input
 *  to this function and `queue[1]` is used for output.
 */
void* Munch2(void* queues);

/**
 * Writer thread. End of the 'pipeline', this takes strings from the queue and
 * prints them to stdout.
 * 
 * @param inputQueue void* wrapping a Queue*, which is the queue that Writer
 *  should read from
 */
void* Writer(void* inputQueue);
#define __WORKERS__
#endif