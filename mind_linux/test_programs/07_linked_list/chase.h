#ifndef PTR_CHASING_CHASE_H
#define PTR_CHASING_CHASE_H

#include <stdint.h>
#include "common.h"
#include "mind.h"
#include "local.h"

void* Chase(void* ptr, bool (*endFunc)(void*), void* (*nextFunc)(void*), enum backend_t backend_type) {
    void* res;
    if(backend_type == PAGING) {
        res = mind_chase_handler(ptr, endFunc, nextFunc);
    } else if (backend_type == RDMA) {
        // TODO
        res = NULL;
    } else if (backend_type == FPGA) {
        // TODO
        res = NULL;
    } else if (backend_type == LOCAL) {
        res = local_chase_handler(ptr, endFunc, nextFunc);
    }
    return res;

}


#endif // PTR_CHASING_CHASE_H
