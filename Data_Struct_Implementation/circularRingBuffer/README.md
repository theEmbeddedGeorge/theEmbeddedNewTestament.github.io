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

### Multithread Consumer/Producer

```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h> 
#include <stdint.h>
#include <semaphore.h>
#include <unistd.h>

#include "ring_buffer.h"

static uint32_t ringBuffer[RING_BUFFER_SIZE];
static volatile uint32_t headIdx;
volatile sig_atomic_t done = 0;

pthread_mutex_t mtx;
sem_t sem_w, sem_r;

void ringBufferInit() {
    headIdx = 0;
}

uint32_t writeToBuffer(int data) {
    sem_wait(&sem_w);

    pthread_mutex_lock(&mtx);
    ringBuffer[headIdx] = data;
    headIdx++;
    printf("Wrote value %d, headIdx: %d\n", data, headIdx);
    pthread_mutex_unlock(&mtx);

    sem_post(&sem_r);
    
    return ACCESS_SUCCESS;
}

uint32_t readFromBuffer(int *data) {
    sem_wait(&sem_r);
    
    pthread_mutex_lock(&mtx);
    headIdx--;
    *data = ringBuffer[headIdx];
    printf("Read value %d headIdx: %d\n", *data, headIdx);
    pthread_mutex_unlock(&mtx);
    
    sem_post(&sem_w);

    return ACCESS_SUCCESS;
}

void *readHandler(void *max) 
{
    int maxNum = (int) max;
    int buffer, r;

    while(maxNum) {
        r = readFromBuffer(&buffer);
        if(r == ACCESS_SUCCESS) {
            maxNum--;
        }
    }

    pthread_exit(NULL);
}

void *writeHandler(void *max)
{
    int r;
    int maxNum = (int) max;

    while(maxNum) {
        r = writeToBuffer(maxNum);
        maxNum--;
    }

    pthread_exit(NULL);
}

void handle_sigint(int sig)
{
    printf("Caught signal %d\n", sig);
    done = 1;
}


int main(int argc, char **argv) {
    int ret, i;
    int target = atoi(argv[1]);
    ringBufferInit();
    pthread_t producer_threads[MAX_NUM_OF_THREADS];
    pthread_t consumer_threads[MAX_NUM_OF_THREADS];

    signal(SIGINT, handle_sigint);

    pthread_mutex_init(&mtx, 0);
    sem_init(&sem_w, 0, RING_BUFFER_SIZE);
    sem_init(&sem_r, 0, 0);
    
    for (i = 0; i < MAX_NUM_OF_THREADS; i++) {
        ret = pthread_create(&consumer_threads[i], NULL, writeHandler, (void *) target);
        if(ret) {
            printf("ERROR: Reading thread creation failure\n");
            exit(EXIT_FAILURE);
        } else {
            printf("writing thread created\n");
        }
    }

    for (i = 0; i < MAX_NUM_OF_THREADS; i++) {
        ret = pthread_create(&producer_threads[i], NULL, readHandler, (void *) target);
        if(ret) {
            printf("ERROR: Reading thread creation failure\n");
            exit(EXIT_FAILURE);
        } else {
            printf("reading thread created\n");
        }
    }

    for (i = 0; i < MAX_NUM_OF_THREADS; i++) {
        pthread_join(producer_threads[i], NULL);
        pthread_join(consumer_threads[i], NULL);
    }

    pthread_mutex_destroy(&mtx);

    return 0;
}
```






