/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

FILE *pipe_stat;
char *message;

void monitor() {
    int buffer;

    printf("monitor started\n");

    /* Die Pipe zum lesen öffnen */
    if ((pipe_stat = fdopen(queue[D_STAT_TO_MON][READ], "r")) == NULL) {
        perror("monitor.c fdopen()");
        exit(EXIT_FAILURE);
    }

    /* Speicher für Nachricht reservieren */
    if ((message = calloc(1, sizeof(char)*SIZE_HEX+1)) == NULL) {
        perror("monitor.c calloc()");
        exit(EXIT_FAILURE);
    }

    /*
     * Nachricht aus der Pipe lesen, zurück ins Dezimalsystem
     * konvertieren und auf der stdout ausgeben.
     */
    while (fgets(message, SIZE_HEX+1, pipe_stat)) {
        sscanf(message, "%x\n", &buffer);

        printf("%d\n", buffer);
    }

}

void monitor_cleanup() {
    printf("monitor cleanup\n");
    fclose(pipe_stat);
    free(message);
    close(queue[D_STAT_TO_MON][READ]);
    _exit(EXIT_SUCCESS);
}
