## Heap (Binary Implementation)

In a heap the highest (or lowest) priority element is always stored at the root, hence the name "heap". A heap is not a sorted structure and can be regarded as partially ordered. As you see from the picture, there is no particular relationship among nodes on any given level, even among the siblings.

Since a heap is a complete binary tree, it has a smallest possible height - a heap with N nodes always has O(log N) height.

A heap is useful data structure when you need to remove the object with the highest (or lowest) priority. A common use of a heap is to implement a priority queue.

***Insert***

The new element is initially appended to the end of the heap (as the last element of the array). The heap property is repaired by comparing the added element with its parent and moving the added element up a level (swapping positions with the parent). This process is called "***percolation up/heapify up***". The comparison is repeated until the parent is larger than or equal to the percolating element.

***Pop***

The minimum/maximum element can be found at the root, which is the first element of the array. We remove the root and replace it with the last element of the heap and then restore the heap property by ***percolating/heapify down***. Similar to insertion, the worst-case runtime is O{log n).

***Time Complexity***

Implementation  | Insert | Pop | Remove | getMax/getMin
----------------|-------|----------|----------|---
 binary heap|	 O(log n)	| O(log n)	| O(log n) |	 O(1)


### Usage
```
make
./binaryHeap
```

### Code

### **Max Heap**
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct max_heap {
    int curIndex;
    int capacity;
    int *data;
} MAX_HEAP, *pMAX_HEAP;

static void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

#define LEFT_CHILD(i) (2*i+1)
#define RIGHT_CHILD(i) (2*i+2)
#define PARENT(i) ((i-1)/2)

pMAX_HEAP max_head_init(int size) {
    pMAX_HEAP new_heap = (pMAX_HEAP) malloc(sizeof(MAX_HEAP));

    new_heap->curIndex = 0;
    new_heap->capacity = size;
    new_heap->data = malloc(sizeof(int)*size);

    return new_heap;
}

int get_max(pMAX_HEAP heap) {
    if (heap->curIndex == 0) {
        printf("Heap is Empty!\n");
        return -1;
    }

    return heap->data[0];
}

void up_heapify(pMAX_HEAP heap, int index) {
    int parent_index = PARENT(index);

    if (index == 0 || heap->data[parent_index] >= heap->data[index])
        return;

    swap(&heap->data[parent_index], &heap->data[index]);
    up_heapify(heap, parent_index);
}

void down_heapify(pMAX_HEAP heap, int index) {
    int left_i = LEFT_CHILD(index);
    int right_i = RIGHT_CHILD(index);
    int target_index = index;

    if (left_i < heap->curIndex && heap->data[left_i] > heap->data[index])
        target_index = left_i;

    if (right_i < heap->curIndex && heap->data[right_i] > heap->data[index])
        target_index = right_i;

    if (target_index == index)
        return;

    swap(&heap->data[target_index], &heap->data[index]);
    down_heapify(heap, target_index);
}

int insert(pMAX_HEAP heap, int value) {
    if (heap->curIndex == heap->capacity) {
        printf("Heap is Full!\n");
        return -1;
    }

    heap->data[heap->curIndex] = value;
    heap->curIndex ++;

    up_heapify(heap, heap->curIndex-1);

    return 0;
}

int pop(pMAX_HEAP heap) {
    if (heap->curIndex == 0) {
        printf("Heap is Empty!\n");
        return -1;
    }

    swap(&heap->data[0], &heap->data[heap->curIndex-1]);
    heap->curIndex --;

    down_heapify(heap, 0);

    return 0;
}

int main(int argc, int **argv) {
    pMAX_HEAP max_heap = max_head_init(5);

    insert(max_heap, 1);
    printf("Max val: %d\n", get_max(max_heap));

    insert(max_heap, 2);
    printf("Max val: %d\n", get_max(max_heap));

    insert(max_heap, 3);
    printf("Max val: %d\n", get_max(max_heap));

    insert(max_heap, 4);
    printf("Max val: %d\n", get_max(max_heap));

    insert(max_heap, 5);
    printf("Max val: %d\n", get_max(max_heap));

    insert(max_heap, 6);
    printf("Max val: %d\n", get_max(max_heap));

    pop(max_heap);
    printf("Max val: %d\n", get_max(max_heap));

    pop(max_heap);
    printf("Max val: %d\n", get_max(max_heap));

    pop(max_heap);
    printf("Max val: %d\n", get_max(max_heap));

    pop(max_heap);
    printf("Max val: %d\n", get_max(max_heap));

    pop(max_heap);
    printf("Max val: %d\n", get_max(max_heap));

    pop(max_heap);
    printf("Max val: %d\n", get_max(max_heap));

    insert(max_heap, 6);
    printf("Max val: %d\n", get_max(max_heap));

    return 0;
}
```

#### **Priority Queue**
```c
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
```

### Reference

[CMU binary Heap](https://www.andrew.cmu.edu/course/15-121/lectures/Binary%20Heaps/heaps.html)