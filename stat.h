/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera (noguera) <mnoguera(at)wisc.edu>
 * @author Julien de Castelnau (de-castelnau) <decastelnau(at)wisc.edu>
 * @date 10/13/2020
 * @file stat.h
 */

#include <pthread.h>
#include <time.h>

#ifndef __STAT__
typedef struct q_stat_t {
    pthread_mutex_t lock;

    unsigned int count;
    clock_t time;  // total
} Stat;

Stat* CreateQueueStatistic();

void incrementCount(Stat* s);
void setCount(Stat* s, unsigned int newValue);
int getCount(Stat* s);

void addTime(Stat* s, clock_t timeToAdd);
clock_t getTime(Stat* s);
#define __STAT__
#endif
