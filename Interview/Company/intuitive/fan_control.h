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

#define SERVER_QUEUE_NAME "/fan-control-server"

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
} Msg_t;

/* 
 * General sturcture declaration 
 */
typedef struct module{
    pthread_mutex_t fan_mutex;
    Fan_hw *fan;
    volatile uint32_t cur_temp;
    volatile short module_id;
} Module;

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
 * Log message function to log events calssified by different levels.
 * Can directly print or send to log module. 
 */
static int log_msg(int priority, const char *format, ...)
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
 * Init message data structure 
 */
static int msg_init(Msg *message, uint8_t module_id, double temp, Msg_t type) {
    message->temp_val = temp;
    message->pid = module_id;
    message->type = type;
}