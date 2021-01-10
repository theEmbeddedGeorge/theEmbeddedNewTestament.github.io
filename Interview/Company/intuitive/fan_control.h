#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>

#define SERVER_QUEUE_NAME   "/fan-control-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 100
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

#define DEBUG_LEVEL 4

#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 1

#define MAX_FAN_NUM 10
#define MAX_MODULE_NUM MAX_FAN_NUM
#define MILLISEC 1000

#define MAX(a, b) (a > b) ? a : b;

/* 
 * Fan structure to control fan speed. Components:
 * 
 * 1. Current fan speed set last time. 
 * 2. Read_speed callback function to read current fan speed.
 * 3. Set_speed callback function to set fan speed.
 * 4. Module ID to indicate which module it belongs to.
 * 
 * set_speed callback function takes two arguments:
 * 1. PWM_counts.  0 for 0 duty cycle, 100 for 100% duty cycle.
 * 2. Register address to map for control speed.
 * 
 * read_speed callback function takes two arguments:
 * 1. value.  speed value read back from fan.
 * 2. Register address to map for read speed.
 * 
 * Speed callback function return status code: 0 for success, 
 * failed otherwise.
 * 
 * Use callback function so that
 * fan from different vendors can have their own set_speed callback
 * that converts PWM_counts to corresponding duty cycle.
 */
typedef struct {
    uint8_t module_id;
    uint8_t current_spd;
    uint32_t rd_reg;
    uint32_t wt_reg;
    int (*read_speed)(uint8_t* value, uint32_t REG_addr);
    int (*set_speed)(uint8_t PWM_counts, uint32_t REG_addr);
} Fan, *pFan;

typedef struct {
    int fan_num;
    int max_speed;
    Fan Fans[MAX_FAN_NUM];
} Fan_group;

/*
 * temperature measurement message from sub module to fan_control module
 */
typedef struct {
    int module_id;
    double temp_val;
} Temp_val;

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