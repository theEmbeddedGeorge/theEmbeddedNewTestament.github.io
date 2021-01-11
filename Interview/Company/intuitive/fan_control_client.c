#include "fan_control.h"

static double read_temperature() {
    return rand() % 100;
}

int main (int argc, char **argv)
{
    mqd_t qd_server, qd_client;   // queue descriptors
    Temp_val module_msg, server_msg;

    // create the client queue for receiving messages from server
    int pid = getpid();
    //sprintf (client_queue_name, "/sp-example-client-%d", getpid ());

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(Temp_val);
    attr.mq_curmsgs = 0;

    if ((qd_client = mq_open (CLIENT_QUEUE_NAME(pid), O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Client: mq_open (client)");
        exit (1);
    }

    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        perror ("Client: mq_open (server)");
        exit (1);
    }

    while (!done) {
        log_msg(LOG_LEVEL_DEBUG, "Start receiving from message queue!");

        /* Keep reading, block when queue is empty to save CPU useage */
        recv = mq_receive (qd_client, (char*) &server_msg, sizeof(Temp_val), NULL);
        if (-1 == recv) {
            if (errno != EINTR) {
                log_msg(LOG_LEVEL_ERROR, "Client: mq_receive error!");
            }
        } else if (recv > 0) {
            log_msg(LOG_LEVEL_DEBUG, "Client: temp val %f from module %d.", module_msg.temp_val, module_msg.module_id);
            
            // send message to server
            module_msg.temp_val = read_temperature();
            module_msg.module_id = pid;
            if (mq_send (qd_server, (char*) module_msg, sizeof(Temp_val), NULL) == -1) {
                log_msg(LOG_LEVEL_ERROR, "Client: mq_send error!");
                continue;
            }
        }
        
        usleep(50*MILLISEC);
    }

    if (mq_close (qd_client) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }

    if (mq_unlink (CLIENT_QUEUE_NAME(pid)) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    printf ("Client: bye\n");

    exit (0);
}