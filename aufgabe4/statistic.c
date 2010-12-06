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

static uint8_t saved[5];
static uint8_t cnt;

/*
 * Statistik-Prozess. Empfängt je 5 Werte, bildet das arithmetische Mittel und
 * schickt dieses an den Monitor-Prozess.
 *
 */
void statistic() {
    uint8_t c, avg;

    cnt = 0;
    for (;;) {
        /* Wir warten auf Nachrichten des CONV-Prozesses */
        while (queue_get_dir() != D_CONV_TO_STAT)
            usleep(1);

        uint8_t data = queue_get_data();
        saved[cnt] = data;
        if (cnt == 4) {
            /* Wir haben 5 Werte gespeichert, können nun also den Mittelwert
             * bilden. */
            avg = 0;
            for (c = 0; c < 5; c++)
                avg += saved[c];
            avg /= 5;
            queue_write(D_STAT_TO_MON, avg);
            cnt = 0;
        } else cnt++;

    }
}

/*
 * Cleanup-Funktion. Beendet den Prozess.
 *
 */
void statistic_cleanup() {
    _exit(EXIT_SUCCESS);
}
