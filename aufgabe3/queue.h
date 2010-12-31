#ifndef _QUEUE_H
#define _QUEUE_H
#include <sys/stat.h>
#include <mqueue.h>

/*
 * Definition des Namens der verwendeten Message Queue.
 * Erspart bei einer Änderung das editieren an mehreren
 * Stellen im Sourcecode.
 */
#define MQ_NAME "/mq_bts_pk"

#endif
