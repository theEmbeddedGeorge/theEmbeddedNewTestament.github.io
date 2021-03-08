/* Very simplistic timer framework by amallory@qnx.com */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/time.h>
#include <inttypes.h>
#include <sys/queue.h>

#define NUM_TIMERS 10
#define MAX_RANDOM_TIME_MS  20000

enum timer_callback_retval {
    CB_RETURN_NORMAL = 0,
    CB_RETURN_FREE_TIMER,
    CB_RETURN_INVALID,
};

enum timer_type {
    TT_RELATIVE = 0,
    TT_ABSOLUTE,
    TT_INVALID,
};

/* Define our timer list type */
TAILQ_HEAD(timer_list, timer_node);

/* Master tick clock/count */
uint64_t tick_cnt = 0;

/*
Timer data structure:
-the linked list
-the monotonic fire time (saved as an absolute time)
-the user callback handler to run on expiry of timer
-the registered data pointer to pass to the user callback
*/
struct timer_node {
    TAILQ_ENTRY(timer_node) entries;
    uint64_t fire;
    int (*cb)(void* user_data);
    void *user_data;
};

/* Our global timer lists */
struct timer_list active_timers;
struct timer_list free_timers;
struct timer_node *timer_memory;

#ifdef DEBUG
/* print out the contents of a given timer list */
void print_list(struct timer_list *list) {
    struct timer_node *np;
    TAILQ_FOREACH(np, list, entries) {
        printf("timer fire %llu\n", np->fire);
    }
}
#endif

/* put a timer onto the free list */
static void free_timer(struct timer_node *timer) {
    TAILQ_INSERT_HEAD(&free_timers, timer, entries);
}

/* pull an available timer off the free list */
static struct timer_node* alloc_timer(void) {
    struct timer_node *np;
    if(TAILQ_EMPTY(&free_timers)) return NULL;

    np=TAILQ_FIRST(&free_timers);
    TAILQ_REMOVE(&free_timers, np, entries);
    return np;
}

/* put a timer onto the actives timer queue */
static void arm_timer(struct timer_node* timer) {
    struct timer_node *np;

    if(TAILQ_EMPTY(&active_timers)) {
        TAILQ_INSERT_HEAD(&active_timers, timer, entries);
    } else {
        for(np=TAILQ_FIRST(&active_timers) ; np ; np=TAILQ_NEXT(np, entries)) {
            if(timer->fire < np->fire) {
                TAILQ_INSERT_BEFORE(np, timer, entries);
                return;
            }
        }
        TAILQ_INSERT_TAIL(&active_timers, timer, entries);
    }
}

/* remove a timer from the actives timer queue */
static void disarm_timer(struct timer_node* timer) {
    TAILQ_REMOVE(&active_timers, timer, entries);
return;
}

/*  set timer attributes such as relative/absolute fire timer,
fire timer, callback and user data passed to callback
*/
static int set_timer(struct timer_node* timer, enum timer_type tt, uint64_t fire, int (*cb)(void*), void* user_data) {
    switch(tt) {
        case TT_RELATIVE:
            fire+=tick_cnt;
            break;
        case TT_ABSOLUTE:
            break; /* do nothing */
        case TT_INVALID:
        default:
            return -1;
    }
    timer->fire = fire;
    timer->cb = cb;
    timer->user_data = user_data;
    return 0;
}

/* initialisation of the timer subsystem */
static void init_timers(void) {
    unsigned i;

    TAILQ_INIT(&active_timers);
    TAILQ_INIT(&free_timers);

    /*  We're preallocating the memory and using a fixed timer pool size to keep
    things simpler and avoid cluttering this exercise with lots of error checking
    for bad memory conditions.  This works or we're toast.
    */
    if((timer_memory = malloc(sizeof(struct timer_node)*NUM_TIMERS)) == NULL) {
        perror("Fatal! Can't allocate our block of timers!");
        exit(EXIT_FAILURE);
    }

    /* seed our free timer list */  
    for(i=0 ; i < NUM_TIMERS; i++) {
        free_timer(&timer_memory[i]);
    }
}

/* Our clock handling routine that runs each clock tick */
static void clock_tick(int signo) {
    struct timer_node *np;

    tick_cnt++; 
    while(!TAILQ_EMPTY(&active_timers) && (np=TAILQ_FIRST(&active_timers)) && np->fire  <= tick_cnt) {
        disarm_timer(np);
        if(np->cb(np->user_data) == CB_RETURN_FREE_TIMER) free_timer(np);
    }

}

/*  Setup a simulated clock tick that functions much like a real clock
interrupt might.  We're using *NIX signals and a process timer as
it is a very common service available on almost any *NIX like system.
While not perfect, it's good enough to be illustrative.
*/
static int init_ticker(unsigned ms) {
    struct itimerval it;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = ms*1000;

    it.it_interval = it.it_value = tv;
    signal(SIGALRM, clock_tick);
    return setitimer(ITIMER_REAL, &it, NULL);
}

/* The user timer callback function */
int tcb(void *data) {
    struct timer_node *np = data;

    /* Normally you wouldn't only printf() as a result of a timer
    but it is sufficient to be illustrative.
    */  
    printf("Timer Callback : %llu\n", np->fire);
    return CB_RETURN_FREE_TIMER;
}

int main(int argc, char* argv[]) {
    struct timer_node *np;
    int i;

    init_timers(); /* init the timer subsystem */
    init_ticker(1); /* 1ms tick simulating a hw clock */

    /* Create a bunch of timers from 1 to 5000ms in time and arm them */
    for (i=0 ; i < NUM_TIMERS ; i++) {
        if((np = alloc_timer()) == NULL) {
            perror("Fatal! we ran out of timers?");
            exit(EXIT_FAILURE);
        }

        if(set_timer(np, 0, (rand()+1) % MAX_RANDOM_TIME_MS , tcb, np) == -1) {
            perror("Fatal! Bad timer set!");
            exit(EXIT_FAILURE);
        }
        arm_timer(np);
    }

    /* Sit around letting the timers expire - not pretty but simple */
    while(1) {
        sleep(100);
    }

    /* never reached */

    return 0;
}