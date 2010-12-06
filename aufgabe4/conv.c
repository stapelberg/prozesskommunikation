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
 * Conv-Prozess. Generiert Zufallszahlen und schickt sie an den Log-Prozess
 * sowie an den Statistik-Prozess.
 *
 */
void conv() {
    int number;
    struct msg *first;
    struct msg *second;
    while (1) {
        /* Zufallszahl generieren */
        number = rand();

        /* Zahl zweimal in die Queue schreiben: Einmal zum loggen, einmal für
         * den Statistik-Prozess */
        first = queue_write(D_CONV_TO_LOG, number);
        second = queue_write(D_CONV_TO_STAT, number);

        /* Warten, bis die Zufallszahl aus der Queue geholt wurde */
        while (first->dir == D_CONV_TO_LOG ||
               second->dir == D_CONV_TO_STAT)
            usleep(1);
    }
}

/*
 * Cleanup-Funktion. Beendet den Prozess.
 *
 */
void conv_cleanup() {
    _exit(EXIT_SUCCESS);
}
