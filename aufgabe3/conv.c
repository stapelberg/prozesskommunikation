/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "queue.h"

void conv() {
    int random_number = 0;
    mqd_t log;
    mqd_t statistic;
    size_t message_size = sizeof(int)*2+1;
    char *msg_one, *msg_two;

    /*
     * Message Queues zur Kommunikation mit dem log
     * und dem statstic Prozess öffnen.
     */
    log = mq_open(MQ_TO_LOG, O_WRONLY);
    statistic = mq_open(MQ_TO_STATISTIC, O_WRONLY);
    if ((log == -1) || (statistic == -1)) {
        perror("conv() mq_open");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        random_number = rand();

        msg_one = calloc(1, message_size);
        msg_two = calloc(1, message_size);

        /* Message im Hexadezimalformat generieren. */
        sprintf(msg_one, "%x", random_number);
        sprintf(msg_two, "%x", random_number);

        printf("conv:\t%s\t\t%s\n", msg_one, msg_two);
//        fgetc(stdin);

        /* Message sowohl in die Queue zum log Prozess als auch
         * in die Queue zum statistic Prozess schreiben.
         */
        if ((mq_send(log, msg_one, strlen(msg_one)+1, 0) == -1) ||
            mq_send(statistic, msg_two, strlen(msg_two)+1, 0) == -1){
            perror("conv() mq_send");
            exit(EXIT_FAILURE);
        }

    }
}

void conv_cleanup() {
    printf("conv cleanup\n");
    mq_unlink(MQ_TO_LOG);
    mq_unlink(MQ_TO_STATISTIC);
    _exit(EXIT_SUCCESS);
}
