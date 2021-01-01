#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Qnode {
    int val;
    struct Qnode* next;
} Qnode;

typedef struct Queue {
    int size;
    int cap;
    Qnode *head;
    Qnode *tail;
} Queue;

Queue* create_Q (int size) {
    if (size <= 0)
        return NULL;

    Queue* new_Q = (Queue*) malloc(sizeof(Queue));
    if (!new_Q)
        return NULL;

    new_Q->cap = size;
    new_Q->size = 0;
    new_Q->head = new_Q->tail = NULL;
}

int pushQ(Queue* queue, int val) {
    if (!queue)
        return -1;
    
    if (queue->size >= queue->cap) {
        printf("Queue full! cannot push more!\n");
        return -1;
    }

    Qnode* new_node = (Qnode*) malloc(sizeof(Qnode));
    new_node->val = val;
    new_node->next = NULL;

    queue->size ++;

    if (queue->size == 1) {
        queue->head = queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    return 0;
}

Qnode* front(Queue* queue) {
    if (!queue)
        return NULL;

    return queue->head;
}

Qnode* back(Queue* queue) {
    if (!queue)
        return NULL;

    return queue->tail;
}

void pop(Queue* queue) {
    Qnode* tmp; 
    
    if (!queue)
        return;
    
    tmp = queue->head;
    queue->head = tmp->next;

    free(tmp);
    queue->size--;
}

int is_empty(Queue* queue) {
    if (!queue)
        return -1;

    return queue->size == 0 ? 1 : 0;
}

int size(Queue* queue) {
    if (!queue)
        return -1;

    return queue->size;
}

int main(int argc, int** argv) {
    Queue *new_queue = create_Q(10); 
    Qnode *new_node;

    pushQ(new_queue, 1);
    pushQ(new_queue, 2);
    pushQ(new_queue, 3);
    pushQ(new_queue, 4);
    pushQ(new_queue, 5);
    pushQ(new_queue, 6);

    new_node = front(new_queue);
    printf("Front val: %d\n", new_node->val);

    new_node = back(new_queue);
    printf("Back val: %d size: %d\n", new_node->val, new_queue->size);
    
    pop(new_queue);
    pop(new_queue);

    new_node = front(new_queue);
    printf("Front val: %d\n", new_node->val);

    new_node = back(new_queue);
    printf("Back val: %d size: %d\n", new_node->val, new_queue->size);
    
    return 0;
}
