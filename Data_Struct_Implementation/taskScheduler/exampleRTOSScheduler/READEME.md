## Example RTOS task scheduler

This is an example RTOS task schedular with round-robin + priority prermption scheduling policy:

[What is "real-time"? Preemptive, priority-based scheduling](https://www.youtube.com/watch?v=kLxxXNCrY60&list=PLC8H7gcKF_bSt4vGkUq6mwlPbf2fwnqy6&index=7&ab_channel=QuantumLeaps%2CLLC)


### Code Analysis

#### **Data Structure**
In this example, there are three types of important data structures:

1. ***OSThread*** Strcture which is essentially the task control block that records the stack pointer address, timeout and priority information.
2. ***OSThread *OS_thread[32 + 1]*** The array of OSThread pointers, which is index by the task's priority value. It implies that there are 33 static priorities and only no two tasks can have the same priority.
3.  ***uint32_t OS_readySet, OS_delayedSet*** Bitmaps that represent the number of available tasks in "Ready" and "Block" set. It implies that there could be multiple tasks that are available to be scheduled at any time.

```c
/* Thread Control Block (TCB) */
typedef struct {
    void *sp; /* stack pointer */
    uint32_t timeout; /* timeout delay down-counter */
    uint8_t prio; /* thread priority */
    /* ... other attributes associated with a thread */
} OSThread;

OSThread * volatile OS_curr; /* pointer to the current thread */
OSThread * volatile OS_next; /* pointer to the next thread to run */

OSThread *OS_thread[32 + 1]; /* array of threads started so far */
uint32_t OS_readySet; /* bitmask of threads that are ready to run */
uint32_t OS_delayedSet; /* bitmask of threads that are delayed */
```

#### **Methods Declaration**

```c
void OS_init(void *stkSto, uint32_t stkSize);

/* callback to handle the idle condition */
void OS_onIdle(void);

/* this function must be called with interrupts DISABLED */
void OS_sched(void);

/* transfer control to the RTOS to run the threads */
void OS_run(void);

/* blocking delay */
void OS_delay(uint32_t ticks);

/* process all timeouts */
void OS_tick(void);

/* callback to configure and start interrupts */
void OS_onStartup(void);
```

#### **Method Implementation**

The most important function is os_tick and os_delay. OS_tick function is the round-robin shceduling part for every systick event, it will decrement the timeout value of tasks in the delaySet by one. Once the timeout value reaches zero, tasks will be removed from the delaySet to the ReadySet by simply set/clear the bitmap bits in the two bitmap variable.

    OSThread *t = OS_thread[LOG2(workingSet)];

This is the function to look for the next thread to schedule in the bit map. 

    #define LOG2(x) (32U - __clz(x))  // __clz(x) count the number of leading zeros

This operation basically calculate the first set bit from the MSB, which is the highest priority task index. We can use this index to find the corresponding task pointer from the task pointer array since they are all indexed by its priorities. Please refer to the reference link for more details.

```c
void OS_init(void *stkSto, uint32_t stkSize) {
    /* set the PendSV interrupt priority to the lowest level 0xFF */
    *(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);

    /* start idleThread thread */
    OSThread_start(&idleThread,
                   0U, /* idle thread priority */
                   &main_idleThread,
                   stkSto, stkSize);
}

void OS_sched(void) {
    /* OS_next = ... */
    if (OS_readySet == 0U) { /* idle condition? */
        OS_next = OS_thread[0]; /* the idle thread */
    }
    else {
        OS_next = OS_thread[LOG2(OS_readySet)];
        Q_ASSERT(OS_next != (OSThread *)0);
    }

    /* trigger PendSV, if needed */
    if (OS_next != OS_curr) {
        *(uint32_t volatile *)0xE000ED04 = (1U << 28);
    }
}

void OS_run(void) {
    /* callback to configure and start interrupts */
    OS_onStartup();

    __disable_irq();
    OS_sched();
    __enable_irq();

    /* the following code should never execute */
    Q_ERROR();
}

void OS_tick(void) {
    uint32_t workingSet = OS_delayedSet;
    while (workingSet != 0U) {
        OSThread *t = OS_thread[LOG2(workingSet)];
        uint32_t bit;
        Q_ASSERT((t != (OSThread *)0) && (t->timeout != 0U));

        bit = (1U << (t->prio - 1U));
        --t->timeout;
        if (t->timeout == 0U) {
            OS_readySet   |= bit;  /* insert to set */
            OS_delayedSet &= ~bit; /* remove from set */
        }
        workingSet &= ~bit; /* remove from working set */
    }
}

void OS_delay(uint32_t ticks) {
    uint32_t bit;
    __disable_irq();

    /* never call OS_delay from the idleThread */
    Q_REQUIRE(OS_curr != OS_thread[0]);

    OS_curr->timeout = ticks;
    bit = (1U << (OS_curr->prio - 1U));
    OS_readySet &= ~bit;
    OS_delayedSet |= bit;
    OS_sched();
    __enable_irq();
}
```