#include <string.h>
#include <mqueue.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#include "fan_hw.h"

#define ACTIVE_QUERY

#define SERVER_QUEUE_NAME "/fan-control-server"
#define CLIENT_QUEUE_NAME_SUFIX "/fan-control-client"

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 100
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

#define DEBUG_LEVEL 4

#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 1

#define MAX_MODULE_NUM MAX_FAN_NUM
#define MILLISEC 1000

#define OVERHEAT_TEMP 75

#define MAX(a, b) (a > b) ? a : b;

typedef enum msg_type {
    MSG_NORMAL = 0,
    MSG_DETACH,
#ifdef ACTIVE_QUERY
    MSG_ATTACH,
    MSG_URGENT,
    MSG_QUERY,
#endif
} Msg_t;

/* 
 * General sturcture declaration 
 */
typedef struct module Module;
typedef int (*module_fan_op_cb)(Module* module, uint32_t *value, int op);

struct module{
    volatile uint32_t cur_temp;
    volatile short module_id;
    pthread_mutex_t fan_mutex;
    module_fan_op_cb fan_op;
    Fan_hw *fan;
    int fan_num;
#ifdef ACTIVE_QUERY
    mqd_t client_q;
#endif
};

typedef struct {
    int active_fan_num;
    int max_temp;
    Module modules[MAX_MODULE_NUM];
} Fan_group;

typedef struct {
    double temp_val;
    uint8_t pid;
    Msg_t type;  
} Msg;

/*
 * fan operation function for our modules
 */
int module_fan_op(Module* module, uint32_t *value, int op);

/*
 * Log message function to log events calssified by different levels.
 * Can directly print or send to log module. 
 */
int log_msg(int priority, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    if(priority <= DEBUG_LEVEL) {
        vprintf(format, args);
        vprintf("\n", NULL);
    }

    va_end(args);
}

/*
 * Check whether input argument is a number
 */
int isNumber(char *str) {
    int i = 0;
    int sz = strlen(str);

    while (sz--) {
        if (!isdigit(str[i++]))
            return 0;
    }

    return 1;
}

/*
 * Init message data structure function
 */
int msg_init(Msg *message, uint8_t module_id, double temp, Msg_t type) {
    memset(message, 0, sizeof(Msg));

    message->temp_val = temp;
    message->pid = module_id;
    message->type = type;
}