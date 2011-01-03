/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "queue.h"

char *message;

void conv() {
    int random_number = 0;
    mqd_t log;
    mqd_t statistic;


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

    /*
     * Speicher für Nachricht allokieren.
     */
    if ((message = calloc(1, MQ_MSG_SIZE_SEND)) == NULL) {
        perror("conv()");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        random_number = rand();

        /* Zufallszahl ins Hexadezimalformat konvertieren. */
        sprintf(message, "%x", random_number);

        /* Nachricht sowohl in die Queue zum log Prozess als auch
         * in die Queue zum statistic Prozess schreiben.
         */
        if ((mq_send(log, message, MQ_MSG_SIZE_SEND, 0) == -1) ||
            mq_send(statistic, message, MQ_MSG_SIZE_SEND, 0) == -1){
            perror("conv() mq_send");
            exit(EXIT_FAILURE);
        }

    }
}

void conv_cleanup() {
    printf("conv cleanup\n");
    free(message);
    mq_unlink(MQ_TO_LOG);
    mq_unlink(MQ_TO_STATISTIC);
    _exit(EXIT_SUCCESS);
}
