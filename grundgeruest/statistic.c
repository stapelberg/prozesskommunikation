/*
 * vim:ts=4:sw=4:expandtab
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void statistic() {
    printf("statistic started\n");
    for (;;) {
    }
}

void statistic_cleanup() {
    printf("statistic cleanup\n");
    _exit(EXIT_SUCCESS);
}
