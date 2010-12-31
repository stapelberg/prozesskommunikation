/*
 * vim:ts=4:sw=4:expandtab
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
    printf("forked\n");
    return pid;
}

mqd_t mqueue_init() {
    mqd_t msgqueue_id;

    /*
     * Festlegung der Attribute.
     */
    struct *mq_attr attributes = calloc(1, sizeof(mq_attr));
    if (attributes == NULL) {
        sprintf(stderr, "main.c mq_init: calloc failed\n");
        return -1;
    }

    attributes->mq_flags = 0; /* Queue blockiert bei mq_send()/mq_receive() */
    attributes->mq_maxmsg = 50; /* maximal 50 Messages in der Queue */
    attributes->mq_msgsize = MAX_QMSG_SIZE; /* Maximale Länge einer Nachricht */
    attributes->mq_curmsgs = 0; /* Anzahl der Messages momentan in der Queue */

    /*
     * Erstellung der Message Queue.
     * O_CREAT zur Erstellung,
     * O_EXCL zur Fehlererzeugung, falls es bereits eine Queue mit diesem
     *  Namen gibt.
     * 660 Schreib- und Leserechte für Prozessnutzer und -gruppe.
     */
    msgqueue_id = mq_open(MQ_ID, O_CREAT | O_EXCL, 660, attributes);
    if (msgqueue_id == -1)
        sprintf(stderr, "main.c mq_init: mq_open failed\n");

    return msgqueue_id;
}

/*
 * main-Funktion. Startet die 4 Prozesse, registriert einen signal-handler für
 * SIGTERM (dieser sendet SIGINT an die 4 Prozesse) und wartet schließlich auf
 * das Ende der Prozesse.
 */
int main() {

    mqd_t mq_id = mqueue_init();
    if (mq_id == -1)
        return EXIT_FAILURE;

    pconv = fork_child(conv, conv_cleanup);
    plog = fork_child(log, log_cleanup);
    pstatistic = fork_child(statistic, statistic_cleanup);
    pmonitor = fork_child(monitor, monitor_cleanup);

    signal(SIGTERM, sigterm);

    int status;
    waitpid(pconv, &status, 0);
    waitpid(plog, &status, 0);
    waitpid(pstatistic, &status, 0);
    waitpid(pmonitor, &status, 0);

    return EXIT_SUCCESS;
}
