## Circular Ring Buffer
#### Usage
```
make
./ring_buffer
```

#### Code
```c
//ring_buffer.h
#include <stdio.h>
#include <stdlib.h>

#define RING_BUFFER_SIZE    256
#define BUFFER_IS_FULL      1
#define BUFFER_NOT_FULL     0
#define BUFFER_IS_EMPTY     1
#define BUFFER_NOT_EMPTY    0
#define ACCESS_SUCCESS      1
#define ACCESS_FAIL         0

typedef enum thread_idx {
    READ_THREAD_IDX = 0,
    WRITE_THREAD_IDX = 1,
    MAX_NUM_OF_THREADS = 2
} THREAD_IDX;
```

```c
//ring_buffer.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<signal.h> 

#include "ring_buffer.h"

static uint32_t ringBuffer[RING_BUFFER_SIZE];
static uint32_t headIdx;
static uint32_t tailIdx;

void ringBufferInit() {
    headIdx = 0;
    tailIdx = 0;
}

uint32_t isBufferFull() {
    if((headIdx + 1) % RING_BUFFER_SIZE == tailIdx) {
        return  BUFFER_IS_FULL;
    }

    return BUFFER_NOT_FULL;
}

uint32_t isBufferEmpty() {
    if(headIdx == tailIdx) {
        return BUFFER_IS_EMPTY;
    }

    return BUFFER_NOT_EMPTY;
}

uint32_t writeToBuffer(int data) {
    if(isBufferFull()) {
        return ACCESS_FAIL;
    }

    ringBuffer[headIdx] = data;
    headIdx = (headIdx + 1) % RING_BUFFER_SIZE;
    
    return ACCESS_SUCCESS;
}

uint32_t readFromBuffer(int *data) {
    if(isBufferEmpty()) {
        return ACCESS_FAIL;
    }

    *data = ringBuffer[tailIdx];
    tailIdx = (tailIdx + 1) % RING_BUFFER_SIZE;

    return ACCESS_SUCCESS;
}

void *readHandler(void *max) 
{
    int maxNum = (int) max;
    int buffer, r;

    while(1) {
        r = readFromBuffer(&buffer);
        if(r == ACCESS_SUCCESS) {
            printf("Read value %d\n", buffer);

            if(buffer == maxNum) {
                break;
            }
        }
    }

    pthread_exit(NULL);
}

void *writeHandler(void *max)
{
    int r;
    int maxNum = (int) max;
    int counter = 1;

    while(counter <= maxNum) {
        r = writeToBuffer(counter);
        if(r == ACCESS_SUCCESS) {
            printf("Wrote value %d\n", counter);
            counter++;
        }
    }

    pthread_exit(NULL);
}

void handle_sigint(int sig)
{
    printf("Caught signal %d\n", sig);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv) {
    int ret;
    uint32_t target = atoi(argv[1]);
    ringBufferInit();
    pthread_t threads[MAX_NUM_OF_THREADS];

    signal(SIGINT, handle_sigint);
    
    ret = pthread_create(&threads[READ_THREAD_IDX], NULL, readHandler, (void *) (size_t)target);
    if(ret) {
        printf("ERROR: Reading thread creation failure\n");
        exit(EXIT_FAILURE);
    } else {
        printf("reading thread created\n");
    }

    ret = pthread_create(&threads[WRITE_THREAD_IDX], NULL, writeHandler, (void *) (size_t)target);
    if(ret) {
        printf("ERROR: Reading thread creation failure\n");
        exit(EXIT_FAILURE);
    } else {
        printf("writing thread created\n");
    }

    pthread_join(threads[READ_THREAD_IDX], NULL);
    pthread_join(threads[WRITE_THREAD_IDX], NULL);

    return 0;
}
```

