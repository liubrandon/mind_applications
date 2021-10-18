#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mind.h"
#include "local.h"
#include "chase.h"

#define DATA_SIZE 4000
#define NUM_NODES 10
typedef struct node {
    char data[DATA_SIZE];
	int num;
    node* next;
} Node;

bool localEndFunc (void *ptr) {
    printf("#%d: %s (END)\n", curr->num, curr->data);
    return ((Node*)ptr)->num == NUM_NODES;
}

void *localNextFunc(void * ptr) {
	Node* curr = (Node*)ptr;
    printf("#%d: %s\n", curr->num, curr->data);
    return (void*)curr->next;
}

int main() {
	backend_t mode = PAGING;
	int remote_flag = mode != PAGING;
    printf("remote flag: %d", remote_flag);
    Node* list = (Node*)allocate(remote_flag);
    int i;
	char data[DATA_SIZE] = "Hello world";
    for (i = 0; i < NUM_NODES; i++) {
        memcpy((void*)list[i].data, data, DATA_SIZE);
		list[i].num = i+1;
        list[i].next = &list[i+1];
    }
	struct timeval t;
	measure_time_start(&t);
    void* ret = Chase((void*)&list[0], localEndFunc, localNextFunc, mode);
	unsigned long duration = measure_time_end(&t);
	printf("Time total (μs) to perform pointer chasing: %lu\n", duration);
	printf("Time per chase (μs): %f\n", (double)duration/(double)NUM_NODES);
    assert(((Node*)ret)->num == NUM_NODES);
    deallocate(remote_flag, (char*)list);
    return 0;
}