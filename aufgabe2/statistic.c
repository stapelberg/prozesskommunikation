/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

FILE *pipe_conv;
FILE *pipe_mon;
char *message;

void statistic() {
    int current_value = 0, last_value = 0, result;

    /* ungenutzte Pipeköpfe schließen */
    close(queue[D_CONV_TO_LOG][READ]);
    close(queue[D_CONV_TO_LOG][WRITE]);
    close(queue[D_CONV_TO_STAT][WRITE]);
    close(queue[D_STAT_TO_MON][READ]);


    printf("statistic started\n");

    /* Die Pipe zum conv Prozess lesend öffnen, die zum monitor schreibend */
    if (((pipe_conv = fdopen(queue[D_CONV_TO_STAT][READ], "r")) == NULL) ||
        ((pipe_mon = fdopen(queue[D_STAT_TO_MON][WRITE], "w")) == NULL)) {
        perror("statistic.c fdopen()");
        exit(EXIT_FAILURE);
    }

    /* Speicher für Nachricht reservieren */
    if ((message = calloc(1, sizeof(char)*SIZE_HEX+1)) == NULL) {
        perror("statistic.c calloc()");
        exit(EXIT_FAILURE);
    }

    /*
     * Auslesen der Nachrichten vom conv Prozess, Hälfte des Wertest mit der
     * des letzten Wert addieren und an den monitor Prozess schicken.
     */
    while (fgets(message, SIZE_HEX+1, pipe_conv)) {
        sscanf(message, "%x\n", &current_value); /* HEX -> DEC */

        result = current_value/2 + last_value/2;
        fprintf(pipe_mon, "%x\n", result);

        last_value = current_value;
    }
}

void statistic_cleanup() {
    printf("statistic cleanup\n");
    fclose(pipe_conv);
    fclose(pipe_mon);
    free(message);
    close(queue[D_CONV_TO_STAT][READ]);
    close(queue[D_STAT_TO_MON][WRITE]);
    _exit(EXIT_SUCCESS);
}
