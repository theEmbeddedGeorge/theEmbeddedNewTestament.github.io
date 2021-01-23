#include "fan_control.h"

#define CLIENT_POLLING_PERIOD_MS 1000
#define TEMP_THRESHOLD 90

volatile sig_atomic_t done = 0;

static void print_usage() {
    printf("Usage: sudo ./fan_control_client <module-id>\n");
    printf("Module id cannot exceed %d.", MAX_MODULE_NUM);
}

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
    double temp_val;
    int mid;
    short prior;

#ifdef ACTIVE_QUERY
    struct mq_attr attr;
    mqd_t qd_client;
    Msg server_msg;
    int recv = 0;
    char client_queue_name[64];
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
    * */
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

        mid = atoi(argv[1]);
        log_msg(LOG_LEVEL_DEBUG, "Module ID: %d", mid);
    }

    memset(&term_action, 0, sizeof(struct sigaction));
    
    /* Instatiate SIGTERM signal handler */
    term_action.sa_handler = term;
    if (-1 == sigaction(SIGINT, &term_action, NULL)) {
        log_msg(LOG_LEVEL_ERROR, "Failed to initiate SIGTERM signal handler.\n");
        exit(EXIT_FAILURE);
    }

    //mid = getpid();
    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        log_msg(LOG_LEVEL_ERROR, "Client %d: mq_open (server) failed", mid);
        perror("mq_open error:");
        exit (EXIT_FAILURE);
    }

#ifdef ACTIVE_QUERY
    /* Instatiate server message queue */
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(Msg);
    attr.mq_curmsgs = 0;

    sprintf(client_queue_name, "%s-%d", CLIENT_QUEUE_NAME_SUFIX, mid%MAX_MODULE_NUM);
    if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK, QUEUE_PERMISSIONS, &attr)) == -1) {
        log_msg(LOG_LEVEL_ERROR, "Client %d: mq_open (client) failed", mid);
        perror("mq_open error:");
        exit (EXIT_FAILURE);
    }

    /* CAUTION: Must send attach message after client queue creation! Otherwise server cannot find it! */
    log_msg(LOG_LEVEL_DEBUG, "Client %d: Send module ATTACH message.", mid);
    msg_init(&client_msg, mid, 0, MSG_ATTACH);
    if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), prior) == -1) {
        log_msg(LOG_LEVEL_ERROR, "Client: mq_send error! MSG type: %d", client_msg.type);
    }

#endif

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
        log_msg(LOG_LEVEL_DEBUG, "Client %d Temp_val: %d", mid, temp_val);
    
#ifdef ACTIVE_QUERY
        recv = mq_receive (qd_client, (char*) &server_msg, sizeof(Msg), NULL);
        if (-1 == recv) {
            if (errno != EINTR && errno != EAGAIN) {
                log_msg(LOG_LEVEL_ERROR, "Client %d: mq_receive error!", mid);
                perror("mq_receive error:");
            }
        } else if (recv > 0) {
            log_msg(LOG_LEVEL_DEBUG, "Client %d: Receive msg from server. Reply..", mid);

            // Receive a active querry from server, send most recent temperature out
            if (server_msg.type == MSG_QUERY) {
                msg_init(&client_msg, mid, temp_val, MSG_NORMAL);
                if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), prior) == -1) {
                    log_msg(LOG_LEVEL_ERROR, "Client: mq_send error! MSG type: %d", client_msg.type);
                    continue;
                }
            }
        }

        // Or send out temperature reading immediately when it is above certain threshold (90 degree for example)
        if (temp_val > TEMP_THRESHOLD) {
            log_msg(LOG_LEVEL_DEBUG, "Client %d: Module Temperature above threshold! Send urgent message!.", mid);
            msg_init(&client_msg, mid, temp_val, MSG_URGENT);
            if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), prior) == -1) {
                log_msg(LOG_LEVEL_ERROR, "Client: mq_send error! MSG type: %d", client_msg.type);
                continue;
            }
        }
#else
        msg_init(&client_msg, mid, temp_val, MSG_NORMAL);
        if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), prior) == -1) {
            log_msg(LOG_LEVEL_ERROR, "Client: mq_send error! MSG type: %d", client_msg.type);
            continue;
        }
#endif

        usleep(CLIENT_POLLING_PERIOD_MS*MILLISEC);
    }
    
    /* Send detach message to server to notify module detach event */
    log_msg(LOG_LEVEL_DEBUG, "Client %d: send detach message to the server.", mid);
    msg_init(&client_msg, mid, read_temperature(), MSG_DETACH);
    
    if (mq_send (qd_server, (char*) &client_msg, sizeof(Msg), 0) == -1) {
        log_msg(LOG_LEVEL_ERROR, "Client %d: mq_send error! MSG type: %d", mid, client_msg.type);
    }

#ifdef ACTIVE_QUERY
    if (mq_close (qd_client) == -1) {
        log_msg(LOG_LEVEL_DEBUG, "Client %d: mq_close", mid);
        exit (EXIT_FAILURE);
    }

    sprintf(client_queue_name, "%s-%d", CLIENT_QUEUE_NAME_SUFIX, mid%MAX_MODULE_NUM);
    if (mq_unlink (client_queue_name) == -1) {
        log_msg(LOG_LEVEL_DEBUG, "Client %d: mq_unlink", mid);
        exit (EXIT_FAILURE);
    }
#endif

EXIT:
    log_msg(LOG_LEVEL_INFO, "Client %d Terminates", mid); 
    exit (EXIT_SUCCESS);
}