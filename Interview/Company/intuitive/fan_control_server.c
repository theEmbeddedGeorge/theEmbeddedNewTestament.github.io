#include "fan_control.h"

volatile sig_atomic_t done = 0;
static mqd_t qd_server, qd_client;
static struct mq_attr attr;

Fan_group general_control_group;

static void fan_group_init() {
    int i;
    Module *module;

    general_control_group.active_fan_num = 0;
    general_control_group.max_temp = 0;

    /* 
     * Assume one fan per module. (fan 1 -> module 1, fan 2 -> module 2)
     */ 
    for (i = 0; i < MAX_MODULE_NUM; i++) {
        module = &(general_control_group.modules);
        module[i].fan = &general_fan_list[i]; // assign one fan to each module
        module[i].cur_temp = 0;
        module[i].module_id = -1; // No module is connected yet
        
        pthread_mutex_init(&module[i].fan_mutex, NULL);
    }
}

static uint32_t temp2speed(double val) {
    if (val <= 20)
        return 0;
    else if (val >= 70)
        return 100;
    else {
        return (uint32_t)(val - 20) * 2;
    }
} 

static void group_set_spd(Fan_group *general_group) {
    int i = 0;
    Module *module;

    general_group->max_temp = 0;
    for (i = 0; i < MAX_MODULE_NUM; i++) {
        module = &general_group->modules[i];
        if (module->module_id != -1) {
            general_group->max_temp = MAX(general_group->max_temp, module->cur_temp);
        }
    }

    for (i = 0; i < MAX_MODULE_NUM; i++) {
        module = &general_group->modules[i];
        if (module->module_id != -1 && module->fan->set_spd) {
            log_msg(LOG_LEVEL_DEBUG, "Set module %d fan speed to %ld.\n", module->module_id, temp2speed(general_group->max_temp));
            module->fan->set_spd(temp2speed(general_group->max_temp), module->fan);
        }
    }
}

static void timer_handler(union sigval val) {
    log_msg(LOG_LEVEL_DEBUG, "timer_handler triggered.\n");
    
    // return if no active module
    if (general_control_group.active_fan_num == 0)
        return;

    // Set fan group speed
    group_set_spd(&general_control_group);
}

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

    /*if (argc == 1 || argc > 3)
        return -1;

    if (atoi(argv[1]) > MAX_MODULE_NUM || atoi(argv[1]) < 0) {
        log_msg(LOG_LEVEL_ERROR, "Module number out of range!\n");
        return -1;
    }

    // Init fangroups
    fan_group_init(atoi(argv[1]));*/
    fan_group_init();
    
    memset(&term_action, 0, sizeof(struct sigaction));
    
    /* Instatiate SIGTERM signal handler */
    term_action.sa_handler = term;
    if (-1 == sigaction(SIGINT, &term_action, NULL)) {
        log_msg(LOG_LEVEL_ERROR, "Failed to initiate SIGTERM signal handler.\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trigger, 0, sizeof(struct itimerspec));

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = timer_handler;

    trigger.it_value.tv_sec = 2;
    trigger.it_value.tv_nsec = 0;

    /* trigger and reload timer every 3 secs */
    trigger.it_interval.tv_sec = 3;
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

    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
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
            }
        } else if (recv > 0) {
            log_msg(LOG_LEVEL_DEBUG, "Server: temp val %f from module %d.", client_msg.temp_val, client_msg.pid);

            if (recv != sizeof(Msg)) {
                log_msg(LOG_LEVEL_ERROR, "Server: a message from module corrupted!");
                continue;
            }

            // Calculate module id based on the module PID 
            mid = client_msg.pid % MAX_MODULE_NUM;

            module = &general_control_group.modules[mid];
            switch(client_msg.type) {
                case MSG_NORMAL:
                    if (module->module_id == -1) {
                        module->module_id = client_msg.pid;
                        general_control_group.active_fan_num++;
                    }
                    module->cur_temp = temp2speed(client_msg.temp_val);

                    module->cur_temp = temp2speed(client_msg.temp_val);
                    if (module->cur_temp == 100) {
                        module->fan->set_spd(module->cur_temp, module->fan);
                        log_msg(LOG_LEVEL_WARNING, "Server: module %d overheat. Adjust speed now!", client_msg.pid);
                    }
                    break;
                case MSG_DETACH:
                    log_msg(LOG_LEVEL_DEBUG, "Server: Receive detach msg from module %d.", client_msg.pid);
                    module->module_id = -1;
                    general_control_group.active_fan_num--;
                    break;
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