/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

FILE *logfile;
char *message;

void log() {
    int random_number = 0;
    mqd_t log;

    /* Message Queue zur Kommunikation mit dem conv Prozess öffnen.
     */
    if ((log = mq_open(MQ_TO_LOG, O_RDONLY)) == -1) {
        perror("log() mq_open");
        exit(EXIT_FAILURE);
    }

    /* Logdatei öffnen */
    if ((logfile = fopen("log.txt", "w")) == NULL) {
        perror("log() fopen");
        exit(EXIT_FAILURE);
    }

    /*
     * Speicher für Nachricht allokieren.
     */
    if ((message = calloc(1, MQ_MSG_SIZE_RCV)) == NULL) {
        perror("log()");
        exit(EXIT_FAILURE);
    }

    for (;;) {

        /* Nachricht über log Queue empfangen */
        if (mq_receive(log, message, MQ_MSG_SIZE_RCV, NULL) == -1) {
            perror("log() mq_receive");
            exit(EXIT_FAILURE);
        }

        /* Zufallszahl aus dem String zurück zur Dezimalzahl konvertieren */
        sscanf(message, "%x", &random_number);

        /* Die konvertierte Zahl in die Log datei schreiben */
        fprintf(logfile, "%d\n", random_number);

    }
}

void log_cleanup() {
    printf("log cleanup\n");
    free(message);
    mq_unlink(MQ_TO_LOG);
    fclose(logfile);
    _exit(EXIT_SUCCESS);
}
