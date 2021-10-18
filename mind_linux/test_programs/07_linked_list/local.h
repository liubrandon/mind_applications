#ifndef PTR_CHASING_LOCAL_H
#define PTR_CHASING_LOCAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>
#include "common.h"


void* local_chase_handler(void* ptr, bool (*endFunc)(void*), void* (*nextFunc)(void*)) {
    void * cur = ptr;
    while(!endFunc(cur)) {
        cur = nextFunc(cur);
    }
    return cur;
}

#endif // PTR_CHASING_LOCAL_H
