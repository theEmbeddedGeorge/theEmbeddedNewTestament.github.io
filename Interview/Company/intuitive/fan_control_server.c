#include "fan_control.h"

#define MAX(a, b) (a > b) ? a : b;

volatile sig_atomic_t done = 0;
static mqd_t qd_server, qd_client;
static struct mq_attr attr;

Fan_group general_group;

static int general_read_speed(uint32_t* value, uint32_t REG_addr) {
    return 0;
}

static int general_set_speed(uint32_t PWM_counts, uint32_t REG_addr) {
    return 0;
}

static void FAN_init(int fan_num) {
    int i;

    /* 
     * Assume all Fans are from the same vendor and have the same
     * read_speed and write_speed functions
     */ 
    general_group.fan_num = fan_num;
    general_group.max_speed = 0;

    for (i = 0; i < MAX_FAN_NUM; i++) {
        if (i < fan_num) {
            /* For known module */
            general_group.Fans[i].module_id = i;
            general_group.Fans[i].current_spd = 0;
            general_group.Fans[i].rd_reg = 0x0;
            general_group.Fans[i].wt_reg = 0x0;
            general_group.Fans[i]->read_speed = general_read_speed;
            general_group.Fans[i]->set_speed = general_set_speed;
        } else {
            /* for unsigned fans */
            general_group.Fans[i].module_id = -1;
            general_group.Fans[i].current_spd = 0;
            general_group.Fans[i].rd_reg = 0x0;
            general_group.Fans[i].wt_reg = 0x0;
            general_group.Fans[i]->read_speed = NULL;
            general_group.Fans[i]->set_speed = NULL;
        }
    }
}

static uint8_t temp2speed(double temp_val) {
    if (temp_val <= 20)
        return 0;
    else if (temp_val >= 70)
        return 100;
    else {
        return (int)(temp_val - 20) * 2;
    }
} 

static void timer_handler(void *arg) {
    int i = 0;

    // send reply message to client
    /*if ((qd_client = mq_open (in_buffer, O_WRONLY)) == 1) {
        log_msg("Server: Not able to open client queue", LOG_LEVEL_ERROR);
        return;
    }

    if (mq_send (qd_client, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
        log_msg ("Server: Not able to send message to client", LOG_LEVEL_ERROR);
        return;
    }*/
    log_msg(LOG_LEVEL_DEBUG, "timer_handler triggered.\n");

    for (i; i < MAX_FAN_NUM; i++) {
        general_group.max_speed = MAX(general_group.max_speed, general_group.Fan[i].current_spd);
    }

    for (i = 0; i < MAX_FAN_NUM; i++) {
        if (-1 != general_group.Fans[i].module_id) {
            general_group.Fans[i]->set_speed(general_group.max_speed, general_group.Fans[i].wt_reg);
        }
    }

}

static void term(int signum) {
    done = 1;
}

int main (int argc, char **argv)
{

    timer_t timerid;
    struct sigevent sev;
    struct itimerspec trigger;
    struct sigaction request_action, term_action;
    char in_buffer [MSG_BUFFER_SIZE];
    char out_buffer [MSG_BUFFER_SIZE];
    Temp_val module_msg;
    ssize_t recv;

    if (argc == 0 || argc > 2)
        return -1;

    if (atoi(argv[i]) > MAX_FAN_NUM) {
        log_msg(LOG_LEVEL_ERROR, "Can have at most %d fans!\n", MAX_FAN_NUM);
        return -1;
    }

    FAN_init(argv[1]);
    
    memset(&request_action, 0, sizeof(struct sigaction));
    
    /* Instatiate SIGTERM signal handler */
    term_action.sa_handler = term;
    if (-1 == sigaction(SIGTERM, &term_action, NULL)) {
        log_msg(LOG_LEVEL_ERROR, "Failed to initiate SIGTERM signal handler.\n");
        exit(EXIT_FAILURE);
    }

    /*
    memset(&term_action, 0, sizeof(struct sigaction));

    term_action.sa_flags = SA_SIGINFO;
    term_action.sa_sigaction = temp_request_handler;

    if (-1 == sigaction(SIGALARM, &request_action, NULL)) {
        log_msg("Failed to initiate timer signal handler.\n", LOG_LEVEL_ERROR);
        exit(EXIT_FAILURE);
    }
    */
    
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trigger, 0, sizeof(struct itimerspec));

    /* 
    * Set the notification method as SIGEV_THREAD:
    * Upon timer expiration, `sigev_notify_function` (thread_handler()),
    * will be invoked as if it were the start function of a new thread.
    */
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = &timer_handler;
    //sev.sigev_value.sival_ptr = &info;

    /* 
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for
    * this timer.
    */
    timer_create(CLOCK_REALTIME, &sev, &timerid);

    trigger.it_value.tv_sec = 2;
    trigger.it_value.tv_nsec = 0;

    /* trigger and reload timer every 3 secs */
    trigger.it_interval.tv_sec = 3;
    trigger.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &trigger, NULL);

    if( -1 == timer_create(CLOCK_REALTIME, &sev, &timerid) ) {
        log_msg(LOG_LEVEL_ERROR, "timer_create failed");
        goto EXIT;
    }

    if( -1 == timer_settime(&timerid, 0, &trigger, NULL) ) {
        log_msg(LOG_LEVEL_ERROR, "timer_settime failed");
        goto EXIT;
    }

    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        log_msg (LOG_LEVEL_ERROR, "Server: mq_open (server)");
        goto EXIT;
    }

    while (!done) {
        /* Keep reading, block when queue is empty */
        recv = mq_receive (qd_server, (char*) &module_msg, sizeof(Temp_val), NULL);
        if (-1 == recv) {
            if (errno != EINTR) {
                log_msg(LOG_LEVEL_ERROR, "Server: mq_receive error!");
            }
        } else if (recv > 0) {
            log_msg(LOG_LEVEL_DEBUG, "Server: temp val %f from module %d.\n", module_msg->temp_val, module_msg->module_id);

            if (module_msg->module_id > 0 && module_msg->module_id < MAX_MODULE_NUM)
                general_group.Fans[module_msg.module_id].current_spd = temp2speed(module_msg.temp_val);
            else
                log_msg(LOG_LEVEL_ERROR, "Server: temp val %f from Unknown module %d.\n", module_msg->temp_val, module_msg->module_id);
        }
        msleep(50);
    }

    return EXIT_SUCCESS;
EXIT:
    /* Delete (destroy) the timer */
    timer_delete(timerid);

    return -1;
}