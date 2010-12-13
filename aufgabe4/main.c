/*
 * vim:ts=4:sw=4:expandtab
 *
 * © 2010 Michael Stapelberg
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "conv.h"
#include "log.h"
#include "monitor.h"
#include "statistic.h"
#include "queue.h"

typedef void (*funcptr)();

pid_t pconv;
pid_t plog;
pid_t pstatistic;
pid_t pmonitor;

/*
 * Signalhandler für SIGTERM. Sendet SIGINT an die 4 Prozesse.
 *
 */
void sigterm() {
    kill(pconv, SIGINT);
    kill(plog, SIGINT);
    kill(pstatistic, SIGINT);
    kill(pmonitor, SIGINT);
}

/*
 * fork()t einen neuen Prozess. In diesem Prozess wird die übergebene Funktion
 * work() aufgerufen, außerdem wird der Signalhandler cleanup() installiert für
 * das Signal SIGINT.
 *
 */
pid_t fork_child(funcptr work, funcptr cleanup) {
    pid_t pid = fork();
    if (pid == 0) {
        /* child process */
        signal(SIGINT, cleanup);

        work();

        /* Die Prozesse laufen endlos. Daher beenden wir mit Fehlercode, sofern
         * die Prozessfunktion doch zurückkehrt. */
        exit(EXIT_FAILURE);
    }
    
    if (pid == -1) {
        /* error */
        perror("fork()");
        exit(EXIT_FAILURE);
    }

    /* parent process */
    printf("forked, pid = %d\n", pid);
    fflush(stdout);
    return pid;
}

/*
 * main-Funktion. Startet die 4 Prozesse, registriert einen signal-handler für
 * SIGTERM (dieser sendet SIGINT an die 4 Prozesse) und wartet schließlich auf
 * das Ende der Prozesse.
 *
 */
int main() {

    /* Shared Memory-Bereich initialisieren */
    queue_init();

    pconv = fork_child(conv, conv_cleanup);
    plog = fork_child(log, log_cleanup);
    pstatistic = fork_child(statistic, statistic_cleanup);
    pmonitor = fork_child(monitor, monitor_cleanup);

    signal(SIGTERM, sigterm);

    /* Warten, bis 4 Prozesse (Reihenfolge egal) sich beendet haben. */
    int status;
    int c;
    for (c = 0; c < 4; c++)
        wait(&status);

    return 0;
}
