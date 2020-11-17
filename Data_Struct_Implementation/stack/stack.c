#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct stack {
    int topIdx;
    int capacity;
    int *data;
} STACK, *pSTACK;

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