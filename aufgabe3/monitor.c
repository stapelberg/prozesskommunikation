/*
 * vim:ts=4:sw=4:expandtab
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void monitor() {
    printf("monitor started\n");
    for (;;) {
    }
}

void monitor_cleanup() {
    printf("monitor cleanup\n");
    _exit(EXIT_SUCCESS);
}
