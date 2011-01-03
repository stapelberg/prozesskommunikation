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

/*
 * Message Queue mit dem gegebenen Namen erstellen.
 */
mqd_t mqueue_init(const char *queue_name) {
    mqd_t msgqueue_id;

    /* Eventuell existierende unbenutzte Message Queue terminieren,
     * um Fehlern durch - eventuell bestehende - Datenreste vorzubeugen.
     */
    mq_unlink(queue_name);

    /*
     * Festlegung der Attribute.
     */
    struct mq_attr *attributes = calloc(1, sizeof(struct mq_attr));
    if (attributes != NULL) { /* NULL als Attribut nimmt die Standard Werte */
        attributes->mq_flags = 0; /* Queue blockiert bei mq_send()/mq_receive() */
        attributes->mq_maxmsg = 4; /* maximal 4 Messages in der Queue */
        attributes->mq_msgsize = MQ_MSG_SIZE_SEND; /* Maximale Länge einer Nachricht */
        attributes->mq_curmsgs = 0; /* Anzahl der Messages momentan in der Queue */
    }

    /*
     * Erstellung der Message Queue mit den zuvor definierten Attributen.
     */
    msgqueue_id = mq_open(queue_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, attributes);
    if (msgqueue_id == -1) {
        perror("mq_init");
        exit(EXIT_FAILURE);
    }

    return msgqueue_id;
}

/*
 * main-Funktion. Startet die 4 Prozesse, registriert einen signal-handler für
 * SIGTERM (dieser sendet SIGINT an die 4 Prozesse) und wartet schließlich auf
 * das Ende der Prozesse.
 */
int main() {

    /*
     * Erstellung der Queues.
     */
    mqueue_init(MQ_TO_LOG);
    mqueue_init(MQ_TO_STATISTIC);
    mqueue_init(MQ_TO_MONITOR);

    pconv = fork_child(conv, conv_cleanup);
    plog = fork_child(logmsg, logmsg_cleanup);
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
