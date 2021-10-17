#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>

#define TEST_MACRO_ALLOC_SIZE (8 * 1024 * 1024 * 1024UL)  // 8 GB
#define TEST_ALLOC_FLAG 0xfe

#define DATA_SIZE 4000
#define NUM_NODES 10
typedef struct node {
	char data[DATA_SIZE];
	node* next;
} node_t;

void append(node_t* head, char data[]) {
    node_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (node_t*)((uintptr_t)current + 4008);
	for(int i = 0; i < DATA_SIZE; i++) {
		current->next->data[i] = data[i];
	}
    current->next->next = NULL;
}

int main(int argc, char **argv) {
	char *data_buf = (char *)mmap(NULL, TEST_MACRO_ALLOC_SIZE, PROT_READ | PROT_WRITE, TEST_ALLOC_FLAG, -1, 0);
	node_t* head = (node_t*) data_buf;
	head->next = NULL;
	char data[DATA_SIZE] = "Hello world.";
	for (int i = 0; i < NUM_NODES; i++) {
		append(head, data);
	}
	node_t* curr = head;
	while (curr != NULL) {
		printf("%s\n", curr->data);
		curr = curr->next;
	}
	return 0;
}