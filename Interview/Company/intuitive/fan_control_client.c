#include "fan_control.h"

volatile sig_atomic_t done = 0;

static void term(int signum) {
    done = 1;
}

static double read_temperature() {
    /* mock temperature read function, generate random value 0 ~ 100 */
    return rand() % 100;
}

int main (int argc, char **argv)
{
    struct sigaction term_action;
    mqd_t qd_server;  
    Msg client_msg;
    struct mq_attr attr;
    double temp_val;
    int recv = 0;
    int mid;
    short prior;
    char client_queue_name[64];
   
    memset(&term_action, 0, sizeof(struct sigaction));
    
    /* Instatiate SIGTERM signal handler */
    term_action.sa_handler = term;
    if (-1 == sigaction(SIGINT, &term_action, NULL)) {
        log_msg(LOG_LEVEL_ERROR, "Failed to initiate SIGTERM signal handler.\n");
        exit(EXIT_FAILURE);
    }

    /* Instatiate server message queue */
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(Msg);
    attr.mq_curmsgs = 0;

    mid = getpid();
    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        log_msg(LOG_LEVEL_ERROR, "Client %d: mq_open (server) failed", mid);
        exit (1);
    }

    while (!done) {
        /* Get temperature value */
        temp_val = read_temperature();

        /*
         * Classified message priority by temperature readings.
         * Higher priority will be placed in front in the queue.
         * 
         * Priority 0 -> 0 ~ 24
         * Priority 1 -> 24 ~ 49
         * Priority 2 -> 50 ~ 74
         * Priority 3 -> 75 ~ 100
         * 
         */ 
        prior = ((int)temp_val / 25) % 4;

        /* 
         * Instatiate message to be sent to server by populating
         * module_id, temperature_value and message type fields
         */
        msg_init(&client_msg, mid, temp_val, MSG_NORMAL);
        if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), prior) == -1) {
            log_msg(LOG_LEVEL_ERROR, "Client: mq_send error! MSG type: %d", client_msg.type);
            continue;
        }
        
        usleep(1000*MILLISEC);
    }
    
    /* Send detach message to server to notify module detach event */
    log_msg(LOG_LEVEL_DEBUG, "Client %d: send detach message to the server.", mid);
    msg_init(&client_msg, mid, read_temperature(), MSG_DETACH);
    if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), 0) == -1) {
        log_msg(LOG_LEVEL_ERROR, "Client %d: mq_send error! MSG type: %d", mid, client_msg.type);
    }

EXIT:
    log_msg(LOG_LEVEL_INFO, "Client %d Terminates", mid); 
    exit (EXIT_SUCCESS);
}