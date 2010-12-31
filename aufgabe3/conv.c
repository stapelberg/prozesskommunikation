/*
 * vim:ts=4:sw=4:expandtab
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

void conv() {
    int random_number = 0;

    sprintf(stderr, "conv() was started");

    for (;;) {
        rand();
    }
}

void conv_cleanup() {
    printf("conv cleanup\n");
    _exit(EXIT_SUCCESS);
}
