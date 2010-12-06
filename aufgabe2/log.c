/*
 * vim:ts=4:sw=4:expandtab
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void log() {
    printf("log started\n");
    for (;;) {
    }
}

void log_cleanup() {
    printf("log cleanup\n");
    _exit(EXIT_SUCCESS);
}
