/*
 * vim:ts=4:sw=4:expandtab
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void conv() {
    printf("conv started\n");
    for (;;) {
    }
}

void conv_cleanup() {
    printf("conv cleanup\n");
    _exit(EXIT_SUCCESS);
}
