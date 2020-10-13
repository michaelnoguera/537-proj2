/**
 * CS 537 Programming Assignment 2 (Fall 2020)
 * @author Michael Noguera (noguera) <mnoguera(at)wisc.edu>
 * @author Julien de Castelnau (de-castelnau) <decastelnau(at)wisc.edu>
 * @date 10/13/2020
 * @file stat.c
 */

#include "stat.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

Stat* CreateQueueStatistic() {
    Stat* s = (Stat*)malloc(sizeof(Stat));
    if (s == NULL) {
        perror("Error allocating memory for Queue statistic tracker.\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&s->lock, NULL) != 0) {
        perror("Error initializing Queue statistic mutex lock.\n");
        exit(EXIT_FAILURE);
    }

    s->count = 0;
    s->time = 0;

    return s;
}

void incrementCount(Stat* s) {
    if (s == NULL) {
        perror("Not a valid statistic.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&s->lock);
    s->count++;
    pthread_mutex_unlock(&s->lock);
}

void setCount(Stat* s, unsigned int newValue) {
    if (s == NULL) {
        perror("Not a valid statistic.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&s->lock);
    s->count = newValue;
    pthread_mutex_unlock(&s->lock);
}

int getCount(Stat* s) {
    if (s == NULL) {
        perror("Not a valid statistic.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&s->lock);
    int currentValue = s->count;
    pthread_mutex_unlock(&s->lock);
    return currentValue;
}

void addTime(Stat* s, clock_t timeToAdd) {
    if (s == NULL) {
        perror("Not a valid statistic.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&s->lock);
    s->time += timeToAdd;
    pthread_mutex_unlock(&s->lock);
}

clock_t getTime(Stat* s) {
    if (s == NULL) {
        perror("Not a valid statistic.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&s->lock);
    time_t currentTime = s->time;
    pthread_mutex_unlock(&s->lock);
    return currentTime;
}
