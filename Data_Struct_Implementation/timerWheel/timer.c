#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

#define WHEEL_BIN_NUMBER 10
#define GRANULARITY 1000000

typedef void (*timeout_handler)();

typedef struct node {
    struct node *next;
    int timestamp;
    timeout_handler timeout_cb;
} Node, *pNode;

typedef struct timing_wheel {
    int cur_slot;
    int granularity;
    Node nodes[WHEEL_BIN_NUMBER];
} TWheel, *pTWheel;

pTWheel init_time_wheel(int gran) {
    pTWheel new_wheel = (pTWheel) malloc(sizeof(TWheel));
    new_wheel->granularity = gran;
    new_wheel->cur_slot = 0;
    int i;
    for (i; i < WHEEL_BIN_NUMBER; i++) {
        new_wheel->nodes[i].next = NULL;
    }
    
    return new_wheel;
}

int install_handler(pTWheel twheel, int deadline, timeout_handler new_cb) {
    int ret = 0;
    if (deadline/twheel->granularity >= WHEEL_BIN_NUMBER) {
        printf("Deadline exceed timing wheel size\n");
        return -1;
    }
    
    int index = (twheel->cur_slot + ((deadline/(twheel->granularity))))%WHEEL_BIN_NUMBER;
    pNode iterator = &(twheel->nodes[index]);
    
    while(iterator->next) {
        iterator = iterator->next;
    }
    
    pNode new_node = (pNode) malloc(sizeof(Node));
    new_node->timeout_cb = new_cb;
    new_node->timestamp = deadline;
    new_node->next = NULL;
    iterator->next = new_node;
    
    return ret;
}

void tick(pTWheel twheel) {
    int i;
    pNode iterator = &twheel->nodes[twheel->cur_slot];
    while(iterator->next) {
        pNode tmp = iterator->next;
        tmp->timeout_cb();
        printf("Callback at %d deadline triggers\n", tmp->timestamp);
        iterator->next = iterator->next->next;
        free(tmp);
    }
    twheel->cur_slot = (twheel->cur_slot+1)%WHEEL_BIN_NUMBER;
}

void print_task() {
    printf("Hi1\n");
}

void print_task2() {
    printf("Hi2\n");
}

void print_task3() {
    printf("Hi3\n");
}

int main(void) {
    int ret = 0;
    
    pTWheel new_wheel = init_time_wheel(GRANULARITY);
    timeout_handler cb = print_task;
    install_handler(new_wheel, 4*GRANULARITY, cb);
    install_handler(new_wheel, 4.25*GRANULARITY, cb);
    install_handler(new_wheel, 4.9*GRANULARITY, cb);
    
    cb = print_task2;
    install_handler(new_wheel, 8*GRANULARITY, cb);
    
    cb = print_task3;
    install_handler(new_wheel, 8*GRANULARITY, cb);

    install_handler(new_wheel, 12*GRANULARITY, cb);
    
    int count = 0;
    while(count < 15) {
        tick(new_wheel);
        usleep(GRANULARITY);
        printf("Time: %d\n", count++);
        if (count == 8) {
            install_handler(new_wheel, 4*GRANULARITY, cb);
        }
    }
    
    return 0;
}