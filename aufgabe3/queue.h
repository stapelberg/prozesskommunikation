#ifndef _QUEUE_H
#define _QUEUE_H
#include <errno.h>
#include <sys/stat.h>
#include <mqueue.h>

/*
 * Definition der Namen der verwendeten Message Queues.
 * Erspart bei einer Änderung das editieren an mehreren
 * Stellen im Sourcecode.
 */
#define MQ_TO_LOG "/mq_bts_pk_log"
#define MQ_TO_STATISTIC "/mq_bts_pk_statistic"
#define MQ_TO_MONITOR "/mq_bts_pk_monitor"

#endif
