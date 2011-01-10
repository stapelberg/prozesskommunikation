#ifndef _QUEUE_H
#define _QUEUE_H

/*
 * Der erste Deskriptor des mit pipe() gefüllten Arrays referenziert
 * zum Lesekopf, der zweite zum Schreibkopf der pipe.
 */
#define WRITE 1
#define READ 0

/*
 * Es ist einfacher die Zahlenwerte im Hexformat zu transportieren.
 */
#define SIZE_HEX (sizeof(int)*2)

enum { D_CONV_TO_LOG = 0, D_CONV_TO_STAT, D_STAT_TO_MON};

int queue[3][2];

#endif
