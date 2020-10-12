#include <time.h>
#include <pthread.h>

#ifndef __STAT__
typedef struct q_stat_t {
    pthread_mutex_t lock;

    unsigned int count;
    clock_t time; // total
} Stat;

Stat* CreateQueueStatistic();

void incrementCount(Stat* s);
void setCount(Stat* s, unsigned int newValue);
int getCount(Stat* s);

void addTime(Stat* s, clock_t timeToAdd);
clock_t getTime(Stat* s);
#define __STAT__
#endif
