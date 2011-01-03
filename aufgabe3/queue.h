#ifndef _QUEUE_H
#define _QUEUE_H
#include <errno.h>
#include <fcntl.h>
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

/*
 * Definition der Größe des zum Empfang und Versand
 * der Nachrichten verwendeten Speichers.
 * Um garantiert jede Nachricht empfangen zu können,
 * muss der zum Empfang genutzte Puffer mindestens 1
 * Byte größer sein, als der zum Versand genutzte.
 */
#define MQ_MSG_SIZE_RCV (sizeof(int)*2+1)
#define MQ_MSG_SIZE_SEND (sizeof(int)*2)

#endif
