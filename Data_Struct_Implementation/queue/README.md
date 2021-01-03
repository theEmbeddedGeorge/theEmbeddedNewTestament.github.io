## Queue (Linked List Implementation)
#### Usage
```
make
./queue
```

#### Code
```c
// A C program to demonstrate linked list based implementation of queue 
#include <stdio.h> 
#include <stdlib.h> 
  
// A linked list (LL) node to store a queue entry 
struct QNode { 
    int key; 
    struct QNode* next; 
}; 
  
// The queue, front stores the front node of LL and rear stores the 
// last node of LL 
struct Queue { 
    struct QNode *front, *rear; 
}; 
  
// A utility function to create a new linked list node. 
struct QNode* newNode(int k) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->key = k; 
    temp->next = NULL; 
    return temp; 
} 
  
// A utility function to create an empty queue 
struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void enQueue(struct Queue* q, int k) 
{ 
    // Create a new LL node 
    struct QNode* temp = newNode(k); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
// Function to remove a key from given queue q 
void deQueue(struct Queue* q) 
{ 
    // If queue is empty, return NULL. 
    if (q->front == NULL) 
        return; 
  
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
  
    free(temp); 
} 
  
// Driver Program to test anove functions 
int main() 
{ 
    struct Queue* q = createQueue(); 
    enQueue(q, 10); 
    enQueue(q, 20); 
    deQueue(q); 
    deQueue(q); 
    enQueue(q, 30); 
    enQueue(q, 40); 
    enQueue(q, 50); 
    deQueue(q); 
    printf("Queue Front : %d \n", q->front->key); 
    printf("Queue Rear : %d\n", q->rear->key); 
    return 0; 
} 
```

#### Advance queue with fornt(), back(), empty() methods
```C
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
```


#### Reference
https://www.geeksforgeeks.org/queue-linked-list-implementation/
