/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

char *message;

void monitor() {
    int random_number = 0;
    mqd_t monitor;

    /*
     * Message Queue zur Kommunikation mit dem statistic Prozess öffnen.
     */
    if ((monitor = mq_open(MQ_TO_MONITOR, O_RDONLY)) == -1) {
        perror("monitor() mq_open");
        exit(EXIT_FAILURE);
    }

    /*
     * Speicher für Nachricht allokieren.
     */
    if ((message = calloc(1, MQ_MSG_SIZE_RCV)) == NULL) {
        perror("monitor()");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        /* Nachricht über monitor Queue vom statistic Prozess empfangen */
        if (mq_receive(monitor, message, MQ_MSG_SIZE_RCV, NULL) == -1) {
            perror("monitor() mq_receive");
            exit(EXIT_FAILURE);
        }

        /* Zufallszahl aus dem String zurück zur Dezimalzahl konvertieren */
        sscanf(message, "%x", &random_number);

        /* Die konvertierte Zahl auf der stdout ausgeben */
        printf("%d\n", random_number);
    }
}

void monitor_cleanup() {
    printf("monitor cleanup\n");
    free(message);
    mq_unlink(MQ_TO_MONITOR);
    _exit(EXIT_SUCCESS);
}
