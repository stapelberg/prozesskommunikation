/*
 * vim:ts=4:sw=4:expandtab
 *
 * © 2010 Michael Stapelberg
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

FILE *file;

/*
 * Logprozess. Schreibt die empfangenen Daten in eine Datei.
 *
 */
void log() {
    /* Datei zum Schreiben öffnen */
    if ((file = fopen("log.txt", "w")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        /* Wir warten auf Nachrichten des CONV-Prozesses */
        while (queue_get_dir() != D_CONV_TO_LOG)
            usleep(1);

        uint8_t data = queue_get_data();
        fprintf(file, "%d\n", data);
    }
}

/*
 * Cleanup-Funktion (wird als Signalhandler gesetzt). Schließt die Logdatei.
 *
 */
void log_cleanup() {
    printf("log cleanup\n");
    fclose(file);
    _exit(EXIT_SUCCESS);
}
