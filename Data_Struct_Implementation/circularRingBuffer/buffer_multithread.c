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





