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