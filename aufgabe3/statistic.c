/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

#define VALUES_STORED 20

char *message;

void statistic() {
    int random_number = 0, values[VALUES_STORED];
    int i = 0, mi=0; /* Indexe für das Array und die Mittelwertsberechnung */
    int mittelwert = 0;
    mqd_t statistic, monitor;

    /*
     * Message Queue zur Kommunikation mit dem conv Prozess öffnen.
     */
    if ((statistic = mq_open(MQ_TO_STATISTIC, O_RDONLY)) == -1) {
        perror("statistic() mq_open(MQ_TO_STATISTIC)");
        exit(EXIT_FAILURE);
    }

    /*
     * Message Queue zur Kommunikation mit dem monitor Prozess öffnen.
     */
    if ((monitor = mq_open(MQ_TO_MONITOR, O_WRONLY)) == -1) {
        perror("statistic() mq_open(MQ_TO_MONITOR)");
        exit(EXIT_FAILURE);
    }

    /*
     * Speicher für Nachricht allokieren.
     */
    if ((message = calloc(1, MQ_MSG_SIZE_RCV)) == NULL) {
        perror("statistic()");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        /* Nachricht empfangen */
        if (mq_receive(statistic, message, MQ_MSG_SIZE_RCV, NULL) == -1) {
            perror("statistic() mq_receive");
            exit(EXIT_FAILURE);
        }

        /* Zahl aus dem String zurück zur Dezimalzahl konvertieren */
        sscanf(message, "%x", &random_number);

        /*
         * Berechnung des Mittelwertes der letzten VALUES_STORED Werten.
         * Bis das Array erstmals gefüllt ist, wird
         * der Wert nur näherungsweise berechnet.
         */
        values[i] = random_number;

        /* Werte in mittelwert aufsummieren, unsichere Methode,
         * Pufferüberlauf bei zu vielen Werten.
         */
        for (mi=0; mi<VALUES_STORED; mi++) {
            mittelwert += values[mi];
        }
        mittelwert /= VALUES_STORED;

        /* Berechneten Wert ins Hexadezimalformat konvertieren. */
        sprintf(message, "%x", mittelwert);

        /* Nachricht sowohl in die Queue zum monitor Prozess schreiben.
         */
        if ((mq_send(monitor, message, MQ_MSG_SIZE_SEND, 0)) == -1) {
            perror("statistic() mq_send(MQ_TO_MONITOR)");
            exit(EXIT_FAILURE);
        }
        i = (i+1)%VALUES_STORED;
    }
}

void statistic_cleanup() {
    printf("statistic cleanup\n");
    free(message);
    mq_unlink(MQ_TO_STATISTIC);
    mq_unlink(MQ_TO_MONITOR);
    _exit(EXIT_SUCCESS);
}
