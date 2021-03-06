/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

FILE *pipe_conv;
FILE *logfile;
char *message;

void log() {
    /* ungenutzte Pipeköpfe schließen */
    close(queue[D_CONV_TO_LOG][WRITE]);
    close(queue[D_CONV_TO_STAT][READ]);
    close(queue[D_CONV_TO_STAT][WRITE]);
    close(queue[D_STAT_TO_MON][READ]);
    close(queue[D_STAT_TO_MON][WRITE]);


    printf("log started\n");

    /* Die Pipe zum lesen öffnen */
    if ((pipe_conv = fdopen(queue[D_CONV_TO_LOG][READ], "r")) == NULL) {
        perror("log.c fdopen()");
        exit(EXIT_FAILURE);
    }

    /* Logdatei öffnen */
    if ((logfile = fopen("log.txt", "w")) == NULL) {
        perror("log.c fopen()");
        exit(EXIT_FAILURE);
    }

    /* Speicher für Nachricht reservieren */
    if ((message = calloc(1, sizeof(char)*SIZE_HEX+1)) == NULL) {
        perror("log.c calloc()");
        exit(EXIT_FAILURE);
    }

    /* Nachricht aus der Pipe lesen, als Endian abhängiger
     * HEX-Wert in die Log Datei schreiben. Für portable
     * Rückkonvertierung ins Dezimalsystem siehe Aufgabe 3,
     * oder bei Aufgabe 2 in statistic.c und monitor.c
     */
    while (fgets(message, SIZE_HEX+1, pipe_conv)) {
        fputs(message, logfile);
    }
}

void log_cleanup() {
    printf("log cleanup\n");
    fclose(logfile);
    fclose(pipe_conv);
    free(message);
    close(queue[D_CONV_TO_LOG][READ]);
    _exit(EXIT_SUCCESS);
}
