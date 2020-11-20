#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct priorityQueue {
    uint32_t curIdx;
    uint32_t capacity;
    int *data;
} PRIORITY_QUEUE, *pPRIORITY_QUEUE;

void printPriorityQueue(pPRIORITY_QUEUE pQ) {
    for (int i = 0; i < pQ->curIdx; i++) {
        printf("%d ", pQ->data[i]);
    }
    printf("\n");
}

pPRIORITY_QUEUE createPriorityQueue (uint32_t size) {
    pPRIORITY_QUEUE newPq = (pPRIORITY_QUEUE) malloc (sizeof(PRIORITY_QUEUE));
    if(!newPq) {
        printf("ERROR: Unable to create new priority queue\n");
        exit(EXIT_FAILURE);
    }

    newPq->capacity = size;
    newPq->curIdx = 0;
    newPq->data = (int *) malloc(size * sizeof(int));
    if(!newPq->data) {
        printf("ERROR: Unable to allocate data for new priority queue\n");
        exit(EXIT_FAILURE);
    }

    return newPq;
}

int parent(int i) {
    return (i - 1) / 2;
}

// return the index of the left child 
int left_child(int i) {
    return 2*i + 1;
}

// return the index of the right child 
int right_child(int i) {
    return 2*i + 2;
}

void heapify(pPRIORITY_QUEUE pQ, uint32_t eleIdx){
    // find left child node
    int left = left_child(eleIdx);

    // find right child node
    int right = right_child(eleIdx);

    // find the largest among 3 nodes
    int largest = eleIdx;

    // check if the left node is larger than the current node
    if (left < pQ->curIdx && pQ->data[left] > pQ->data[largest]) {
        largest = left;
    }

    // check if the right node is larger than the current node
    if (right < pQ->curIdx && pQ->data[right] > pQ->data[largest]) {
        largest = right;
    }

    // swap the largest node with the current node 
    // and repeat this process until the current node is larger than 
    // the right and the left node
    if (largest != eleIdx) {
        int temp = pQ->data[eleIdx];
        pQ->data[eleIdx] = pQ->data[largest];
        pQ->data[largest] = temp;
        heapify(pQ, largest);
    }
}

void insert(pPRIORITY_QUEUE pQ, int newEntry) {
    int newElementIdx;
    if(pQ->curIdx == pQ->capacity){
        printf("priority queue is full. Unable to insert data.\n");
        return;
    }

    pQ->data[pQ->curIdx++] = newEntry;
    newElementIdx = pQ->curIdx - 1;
    
    while(newElementIdx != 0 && pQ->data[parent(newElementIdx)] < newEntry) {
        int parentIdx = parent(newElementIdx);
        int temp = pQ->data[parentIdx];
        pQ->data[parentIdx] = newEntry;
        pQ->data[newElementIdx] = temp;
        newElementIdx = parentIdx;
    }
}

uint32_t pop(pPRIORITY_QUEUE pQ) {

    int result;
    if(pQ->curIdx == 0) {
        printf("priority queue is empty");
        return INT_MIN;
    }

    result = pQ->data[0];
    pQ->data[0] = pQ->data[pQ->curIdx - 1];
    pQ->curIdx--;

    heapify(pQ, 0);

    return result;
}

int main() {
    int temp;
    pPRIORITY_QUEUE pQ = createPriorityQueue(10);
    insert(pQ, 1);
    insert(pQ, 5);
    insert(pQ, 3);
    insert(pQ, 2);
    insert(pQ, 4);
    insert(pQ, 6);
    printPriorityQueue(pQ);

    printf("---\n");
    temp = pop(pQ);
    printf("popped %d\n", temp);
    temp = pop(pQ);
    printf("popped %d\n", temp);
    temp = pop(pQ);
    printf("popped %d\n", temp);
    temp = pop(pQ);
    printf("popped %d\n", temp);

    return 0;
}