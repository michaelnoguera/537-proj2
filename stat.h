#include <time.h>
#include <pthread.h>

typedef struct q_stat_t {
    pthread_mutex_t lock;

    unsigned int count;
    clock_t time; // total
} Stat;

Stat* CreateQueueStatistic();

int incrementCount(Stat* s);
int setCount(Stat* s, unsigned int newValue);
int getCount(Stat* s);

int addTime(Stat* s, clock_t timeToAdd);
clock_t getTime(Stat* s);
