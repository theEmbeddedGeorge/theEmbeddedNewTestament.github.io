#include "fan_control.h"

#define TIMER_PERIOD 5

volatile sig_atomic_t done = 0;
static mqd_t qd_server;

Fan_group general_control_group;
int module_number = MAX_MODULE_NUM;

/* 
 * Modeule fan operation function
 */ 
int module_fan_op(Module* module, uint32_t *value, int op) {
    int ret = 0;
    int i;
    Fan_hw *fan;

    if (!module && !module->fan) {
            log_msg(LOG_LEVEL_ERROR, "Module not instantiate! Abort fan operation");
            return -1;
    }

    for (i = 0; i < module->fan_num; i++) {
        fan = &module->fan[i];

        // take mutex to ensure atomic manipulation on fan hardware
        pthread_mutex_lock(&module->fan_mutex);

        switch(op) {
            // set speed operation
            case 0:
                if (fan->set_spd)
                    fan->set_spd(*value, fan);
                else
                    ret = -1;
                break;
            // read speed operation
            case 1:
                if (fan->read_spd)
                    fan->read_spd(value, fan);
                else
                    ret = -1;
                break;
            default:
                log_msg(LOG_LEVEL_ERROR, "Unkown operation!");
                break;
        }

        pthread_mutex_unlock(&module->fan_mutex);
    }

    return ret;
}

/* 
 * Init fan group structure with the module number pass in
 * Instantiate module_number number of fans in the fan group
 */ 
static void fan_group_init(int module_number) {
    int i;
    Module *module;

    general_control_group.active_fan_num = 0;
    general_control_group.max_temp = 0;

    /* 
     * Assume one fan per module. (fan 1 -> module 1, fan 2 -> module 2)
     */ 
    module = general_control_group.modules;
    for (i = 0; i < module_number; i++) {
        module[i].fan_num = 1; // assume 1 fan per module
        module[i].fan = &general_fan_list[i]; // assign one fan to each module
        module[i].cur_temp = 0;
        module[i].module_id = -1; // No module is connected yet
        module[i].fan_op = module_fan_op;
        
        pthread_mutex_init(&module[i].fan_mutex, NULL);
    }
}

/* 
 * Convert temperature to duty cycle percentage
 */ 
static uint32_t temp2speed(double val) {
    if (val <= 20)
        return 0;
    else if (val >= 70)
        return 100;
    else {
        return (uint32_t)((val - 20.0) * 2.0);
    }
} 

/* 
 * Display Fan states in the control group
 */ 
static void print_fan_group_info (Fan_group *general_group) {
    Module *module;
    int i;

    printf("=======================\n");
    printf("Active module number: %d\n", general_group->active_fan_num);
    printf("Max temperature: %d\n", general_group->max_temp);
    printf("Temperature by Module: %d\n", general_group->max_temp);
    printf("[");
    for (i = 0; i < module_number; i++) {
        module = &general_group->modules[i];
        printf("%u ", module->cur_temp);
    }
    printf("]\n");
    printf("Fan speed duty cycle: %d\n", temp2speed(general_group->max_temp));
    printf("=======================\n\n");
}

/* 
 * Set all active fan in the same group with the same speed
 * Speed is determined by the max temperature
 */ 
static void group_set_spd(Fan_group *general_group) {
    int i = 0;
    uint32_t value;
    Module *module;

    general_group->max_temp = 0;
    for (i = 0; i < module_number; i++) {
        module = &general_group->modules[i];
        if (module->module_id != -1) {
            general_group->max_temp = MAX(general_group->max_temp, module->cur_temp);
        }
    }

    for (i = 0; i < module_number; i++) {
        module = &general_group->modules[i];
        if (module->module_id != -1) {
            value = temp2speed(general_group->max_temp);
            module->fan_op(module, &value, 0); // set fan speed
        }
    }

    print_fan_group_info(general_group);
}

#ifdef ACTIVE_QUERY
/* 
 * Send temperature reading query to all active modules
 */ 
static void group_send_temp_query(Fan_group *general_group) {
    int i = 0;
    Module *module;
    Msg client_msg;
    char buffer[64];

    log_msg(LOG_LEVEL_DEBUG, "Server: Query all active module for temperature.");
    for (i = 0; i < module_number; i++) {
        module = &general_group->modules[i];
        if (module->module_id != -1) {
            msg_init(&client_msg, getpid(), 0, MSG_QUERY);
            if (mq_send (module->client_q, (char*) &client_msg, sizeof(Msg), 0) == -1) {
                log_msg(LOG_LEVEL_ERROR, "Server: mq_send to client %d error!", module->module_id);
                perror("mq_send error:");
                continue;
            }
        }
    }
}
#endif

/* 
 * Handler function invoked when timer expires
 */ 
static void timer_handler(union sigval val) {
    log_msg(LOG_LEVEL_DEBUG, "timer_handler triggered.\n");
    
    // return if no active module
    if (general_control_group.active_fan_num == 0) {
        log_msg(LOG_LEVEL_DEBUG, "No active module. Continue.\n");
        return;
    }

    // Set fan group speed
    group_set_spd(&general_control_group);

#ifdef ACTIVE_QUERY
    // Query all connected module about their temperature readings
    group_send_temp_query(&general_control_group);
#endif
}

static void print_usage() {
    printf("Usage: sudo ./fan_control_server <number-of-modules>|<Enter>\n");
    printf("Module number cannot exceed %d.\n", MAX_MODULE_NUM);
    printf("If number-of-module not specified, max number of module allowed is default as %d\n.", MAX_MODULE_NUM);
}

/* 
 * Signal handler function
 */ 
static void term(int signum) {
    done = 1;
}

int main (int argc, char **argv)
{

    timer_t timerid;
    struct sigevent sev;
    struct itimerspec trigger;
    struct sigaction term_action;
    Msg client_msg;
    ssize_t recv;
    Module *module;
    int mid;
    uint32_t value;
    struct mq_attr attr;

#ifdef ACTIVE_QUERY
    char buffer[64];
#endif

    /*
    * Check input arguments
    */
    if (argc < 1 || argc > 2) {
        log_msg(LOG_LEVEL_ERROR, "Incorrect input arguments!");
        print_usage();
        exit(EXIT_FAILURE);
    }

    /*
    *  If user specifies the number of max modules will be connected
    */
    if (argc == 2) {
        if (!isNumber(argv[1])) {
            log_msg(LOG_LEVEL_ERROR, "Incorrect input arguments! Expect a number!");
            print_usage();
            exit(EXIT_FAILURE);
        } else if (atoi(argv[1]) > MAX_MODULE_NUM) {
            log_msg(LOG_LEVEL_ERROR, "At most %d modules allowed!", MAX_MODULE_NUM);
            print_usage();
            exit(EXIT_FAILURE);
        }

        module_number = atoi(argv[1]);
        log_msg(LOG_LEVEL_DEBUG, "Max module number to be conncted: %d", module_number);
    }

    // Init fangroups
    fan_group_init(module_number);
    
    memset(&term_action, 0, sizeof(struct sigaction));
    
    /* Instatiate SIGTERM signal handler */
    term_action.sa_handler = term;
    if (-1 == sigaction(SIGINT, &term_action, NULL)) {
        log_msg(LOG_LEVEL_ERROR, "Failed to initiate SIGTERM signal handler.\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trigger, 0, sizeof(struct itimerspec));

    /* When timer expires, invoke function as a thread */
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = timer_handler;

    /* arm the timer after 1sec */
    trigger.it_value.tv_sec = 1;
    trigger.it_value.tv_nsec = 0;

    /* trigger and reload timer every 3 secs */
    trigger.it_interval.tv_sec = TIMER_PERIOD;
    trigger.it_interval.tv_nsec = 0;

    if( -1 == timer_create(CLOCK_REALTIME, &sev, &timerid) ) {
        log_msg(LOG_LEVEL_ERROR, "timer_create failed");
        goto EXIT;
    }

    if( -1 == timer_settime(timerid, 0, &trigger, NULL) ) {
        log_msg(LOG_LEVEL_ERROR, "timer_settime failed");
        goto EXIT;
    }

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(Msg);
    attr.mq_curmsgs = 0;

    /* create server queue to receive client message */
    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT | O_EXCL, QUEUE_PERMISSIONS, &attr)) == -1) {
        log_msg (LOG_LEVEL_ERROR, "Server: mq_open (server) fail!");
        goto EXIT;
    }

    while (!done) {
        log_msg(LOG_LEVEL_DEBUG, "Start receiving from message queue!");

        /* Keep reading, block when queue is empty to save CPU useage */
        recv = mq_receive (qd_server, (char*) &client_msg, sizeof(Msg), NULL);
        if (-1 == recv) {
            if (errno != EINTR) {
                log_msg(LOG_LEVEL_ERROR, "Server: mq_receive error!");
                perror("mq_receive error:");
            }
        } else if (recv > 0) {
            if (recv != sizeof(Msg)) {
                log_msg(LOG_LEVEL_ERROR, "Server: a message from module corrupted!");
                continue;
            }

            // Calculate module id based on the module PID 
            mid = client_msg.pid % module_number;
            log_msg(LOG_LEVEL_DEBUG, "Server: temp val %f from module %d.", client_msg.temp_val, mid);

            module = &general_control_group.modules[mid];
            switch(client_msg.type) {
                case MSG_NORMAL:
                    log_msg(LOG_LEVEL_DEBUG, "Received module %d normal temperature msg.", mid);
                    if (module->module_id == -1) {
                        module->module_id = mid;
                        general_control_group.active_fan_num++;
#ifdef ACTIVE_QUERY
                        sprintf(buffer, "%s-%d", CLIENT_QUEUE_NAME_SUFIX, mid);
                        if ((module->client_q = mq_open (buffer, O_WRONLY)) == -1) {
                            log_msg(LOG_LEVEL_ERROR, "Server: open client %d queue failed", mid);
                        }
#endif
                    }
                    module->cur_temp = client_msg.temp_val;

                    /*if (module->cur_temp >= OVERHEAT_TEMP) {
                        value = temp2speed(module->cur_temp);
                        module->fan_op(module, &value, 0); // set fan speed
                        log_msg(LOG_LEVEL_WARNING, "Server: module %d overheat. Adjust speed now!", mid);
                    }*/
                    break;
                case MSG_DETACH:
                    log_msg(LOG_LEVEL_INFO, "Server: Receive detach msg from module %d.", mid);
                    general_control_group.active_fan_num = \
                        (general_control_group.active_fan_num > 0) ? (general_control_group.active_fan_num - 1) : 0;
#ifdef ACTIVE_QUERY                
                    if (mq_close (module->client_q) == -1) {
                        log_msg(LOG_LEVEL_DEBUG, "Server: mq_close client %d queue failed", module->module_id);
                        perror("mq_close error");
                    }
#endif
                    module->module_id = -1;
                    module->cur_temp = 0;
                    break;
#ifdef ACTIVE_QUERY
                case MSG_ATTACH:
                    log_msg(LOG_LEVEL_INFO, "Received module %d attach request.", mid);
                    if (module->module_id == -1) {
                        module->module_id = mid;
                        general_control_group.active_fan_num++;

                        sprintf(buffer, "%s-%d", CLIENT_QUEUE_NAME_SUFIX, mid);
                        log_msg(LOG_LEVEL_DEBUG, "Server: client %d queue name %s", mid, buffer);
                        if ((module->client_q = mq_open (buffer, O_WRONLY)) == -1) {
                            log_msg(LOG_LEVEL_ERROR, "Server: open client %d queue failed", mid);
                            perror("mq_open error:");
                        }

                    } else {
                        log_msg(LOG_LEVEL_WARNING, "Module %d already attached! Ignore attach request.", mid);
                    }
                    break;
                case MSG_URGENT:
                    log_msg(LOG_LEVEL_WARNING, "Server: Received urgent msg from module %d. Adjust speed now!", mid);
                    if (module->module_id == -1) {
                        module->module_id = mid;
                        general_control_group.active_fan_num++;

                        sprintf(buffer, "%s-%d", CLIENT_QUEUE_NAME_SUFIX, mid);
                        if ((module->client_q = mq_open (buffer, O_WRONLY)) == -1) {
                            log_msg(LOG_LEVEL_ERROR, "Server: open client %d queue failed", mid);
                        }
                    }
                    module->cur_temp = client_msg.temp_val;

                    value = temp2speed(module->cur_temp);
                    module->fan_op(module, &value, 0); // set fan speed
                    break;
#endif
                default:
                    break;
            }
        }
        
        usleep(50*MILLISEC);
    }

    if (mq_close (qd_server) == -1) {
        log_msg(LOG_LEVEL_DEBUG, "Server: mq_close");
        exit (EXIT_FAILURE);
    }

    if (mq_unlink (SERVER_QUEUE_NAME) == -1) {
        log_msg(LOG_LEVEL_DEBUG, "Server: mq_unlink");
        exit (EXIT_FAILURE);
    }
    log_msg(LOG_LEVEL_INFO, "Server Terminates");
    return EXIT_SUCCESS;
EXIT:
    timer_delete(timerid);

    return EXIT_FAILURE;
}