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

/*
 * Monitor-Prozess. Gibt die empfangenen Daten aus.
 *
 */
void monitor() {
    while (1) {
        /* Wir warten auf Nachrichten des CONV-Prozesses */
        while (queue_get_dir() != D_STAT_TO_MON)
            usleep(1);

        uint8_t data = queue_get_data();
        printf("Empfangenes Datum: %d\n", data);
        fflush(stdout);
    }
}

/*
 * Cleanup-Funktion. Beendet den Prozess.
 *
 */
void monitor_cleanup() {
    _exit(EXIT_SUCCESS);
}
