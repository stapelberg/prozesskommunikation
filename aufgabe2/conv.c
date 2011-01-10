/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "queue.h"

FILE *pipe_log;
FILE *pipe_stat;

void conv() {
    int random_number = 0;

    /* Die beiden Pipes zum schreiben öffnen */
    if (((pipe_log = fdopen(queue[D_CONV_TO_LOG][WRITE], "w")) == NULL) ||
        ((pipe_stat = fdopen(queue[D_CONV_TO_STAT][WRITE], "w")) == NULL)) {
        perror("conv.c fdopen()");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        /* Generierung der Zufallszahl, um Fehler im
         * statistic Prozess zu vermeiden mit geringerem
         * Wertebereich [0, SHRT_MAX] (quasi als short).
         */
        random_number = rand()%SHRT_MAX;

        /* Nachricht sowohl in die Queue zum log Prozess als auch
         * in die Queue zum statistic Prozess schreiben.
         */
        fprintf(pipe_log, "%x\n", random_number);
        fprintf(pipe_stat, "%x\n", random_number);
    }
}

void conv_cleanup() {
    printf("conv cleanup\n");
    fclose(pipe_log);
    fclose(pipe_stat);
    close(queue[D_CONV_TO_LOG][WRITE]);
    close(queue[D_CONV_TO_STAT][WRITE]);
    _exit(EXIT_SUCCESS);
}
