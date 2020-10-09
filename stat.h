#include <time.h>

typedef struct q_stat_t {
    // TODO add synchronization field
    unsigned int count;
    time_t time; // total
} Stat;

Stat CreateQueueStatistic();

int incrementCount();
int setCount(unsigned int newValue);
int getCount();

int addTime(time_t timeToAdd);
time_t getTime();
