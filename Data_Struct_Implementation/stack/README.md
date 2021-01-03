## Stack (Array Implementation)
#### Usage
```
make
./stack
```

#### Analysis
```Stack is implemented with ARRAY (if capacity is given).```

In contrary to Queue, for stack implementation, we simply need to use a array to hold all the data and keep track of the last element index as it only need to pop the LRU element which can be simply realized by decrementing the last element index.

Note that this method will always pre-allocate a fix chunk memory since array cannot be dynamically allocated. But this also makes our stack operation very fast since we are only manipulating with the array indexes.

#### Code
```c
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct stack {
    int topIdx;
    int capacity;
    int *data;
} STACK, *pSTACK;  // also define a pointer to this struct, this is a very good style

pSTACK createStack(int capacity) {
    pSTACK newStack = (pSTACK) malloc (sizeof(STACK));
    if(newStack == NULL) {
        printf("ERROR: cannot allocate memeory for stack\n");
        exit(EXIT_FAILURE);
    }
    newStack->data = (int *) malloc (capacity * sizeof(int));
    if(newStack->data == NULL) {
        printf("ERROR: cannot allocate memeory for stack data\n");
        exit(EXIT_FAILURE);
    }

    newStack->topIdx = -1;
    newStack->capacity = capacity;
    return newStack;
}

int isFull(pSTACK curStack) {
    return curStack->topIdx >= curStack->capacity - 1;
}

int isEmpty(pSTACK curStack) {
    return curStack->topIdx == -1;
}

void push(pSTACK curStack, int data) {
    if(isFull(curStack)){
        return;
    }
    curStack->data[++(curStack->topIdx)] = data;
    printf("%d pushed to stack\n", data);
}

int pop(pSTACK curStack) {
    if (isEmpty(curStack)){
        printf("stack is empty\n");
        return INT_MIN;
    }

    return curStack->data[(curStack->topIdx)--];
}

int top(pSTACK curStack) {
    if (isEmpty(curStack)){
        printf("stack is empty\n");
        return INT_MIN;
    }
    return curStack->data[curStack->topIdx];
}

int main() {
    pSTACK myStack = createStack(10);
    push(myStack, 10);
    push(myStack, 20);
    push(myStack, 30);

    printf("cur top %d\n", top(myStack));
    printf("popped element %d\n", pop(myStack));
    printf("popped element %d\n", pop(myStack));
    printf("popped element %d\n", pop(myStack));

    return 0;
}
```