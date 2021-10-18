#ifndef PTR_CHASING_MIND_H
#define PTR_CHASING_MIND_H
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

#define MMAP_ADDR_MASK 0xffffffffffff
#define TEST_MACRO_ALLOC_SIZE (8 * 1024 * 1024 * 1024UL) // 8GB
#define TEST_ALLOC_FLAG 0xfe

#define PAGE_SIZE 4096LU
#define DEBUG_ALLOC_SIZE 1024 * 1024 * 1024 * 2L


#define MAX_NUM_THREAD 16
#define MAX_NUM_NODES 16



/* Example of read write to random remote address
    // Remote address (page granularity)
    addr = GetRandom(0, num_pages, &seedp) * PAGE_SIZE;
    // READ
    memcpy(zeros, &data_buf[addr], PAGE_SIZE);
    // WRITE
    memcpy(&data_buf[addr], zeros, PAGE_SIZE);
*/


char* allocate(int remote_flag) {
    char *buf = NULL;
    if (!remote_flag) {
        // remote
        buf = (char *) mmap(NULL, TEST_MACRO_ALLOC_SIZE, PROT_READ | PROT_WRITE, TEST_ALLOC_FLAG, -1, 0);
    } else {
        // local
        buf = (char *)mmap(NULL, DEBUG_ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON | MAP_NORESERVE, -1, 0); //TEST PURPOSE
    }
    if (!buf || buf == (void *) -1) {
        int errnum = errno;
        printf("Error: cannot allocate buffer [0x%lx] %s\n", (unsigned long) buf, strerror(errnum));
        return (char *)NULL;
    }
    return buf;
}

void deallocate(int remote_flag, char* buf) {
    if (!remote_flag) {
        // remote
        munmap(buf, TEST_MACRO_ALLOC_SIZE);
    } else {
        // local
        munmap(buf, DEBUG_ALLOC_SIZE);
    }
}

void* mind_chase_handler(void* ptr, bool (*endFunc)(void*), void* (*nextFunc)(void*)) {
    void * cur = ptr;
    while(!endFunc(cur)) {
        cur = nextFunc(cur);
    }
    return cur;
}

#endif // PTR_CHASING_MIND_H
