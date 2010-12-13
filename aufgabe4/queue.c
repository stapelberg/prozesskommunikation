/*
 * vim:ts=4:sw=4:expandtab
 * 
 * © 2010 Michael Stapelberg
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>

#include "queue.h"

static const int queue_size = sizeof(struct queueheader) + (sizeof(struct msg) * 255);

static uint8_t *shm;
static struct queueheader *shmheader;
static struct msg *shmdata;

/*
 * Sperrt den Semaphor: Nun kann im Shared Memory-Bereich geschrieben werden.
 *
 */
static void lock() {
    while (1) {
        if (sem_wait(&(shmheader->sem)) == -1) {
            if (errno == EINTR)
                continue;
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
        break;
    }
}

/*
 * Entsperrt den Semaphor.
 *
 */
static void unlock() {
    sem_post(&(shmheader->sem));
}

/*
 * Initialisiert das Shared Memory-Segment, bildet es in den Speicher ab und
 * initialisiert den Semaphor.
 *
 */
void queue_init() {
    int c;
    int fd;
    int flags = O_RDWR | O_CREAT | O_TRUNC;
    if ((fd = shm_open("/bts-sem", flags, S_IREAD | S_IWRITE)) == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, queue_size);

    if ((shm = mmap(0, queue_size, (PROT_READ | PROT_WRITE),
                    MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    close(fd);

    shmheader = (struct queueheader*)shm;
    shmdata = (struct msg*)(shm + sizeof(struct queueheader));

    /* Speicher mit 0 initialisieren */
    memset(shmheader, 0, sizeof(struct queueheader));
    struct msg *curmsg = shmdata;
    for (c = 0; c < 255; c++) {
        curmsg->dir = D_INVALID;
        curmsg++;
    }

    /* Semaphor initialisieren */
    if (sem_init(&(shmheader->sem), 1, 1) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
}

/*
 * Hängt den übergebenen Wert in die Queue.
 *
 */
struct msg *queue_write(uint8_t dir, uint8_t data) {
    struct msg msg;
    msg.dir = dir;
    msg.data = data;

    uint8_t next;
    if (shmheader->cur == 255)
        next = 0;
    else next = shmheader->cur + 1;

    lock();
    struct msg *curmsg = shmdata + (shmheader->cur * sizeof(struct msg));
    struct msg *nextmsg = shmdata + (next * sizeof(struct msg));
    memcpy(nextmsg, &msg, sizeof(struct msg));
    if (curmsg->dir == D_INVALID)
        shmheader->cur = next;
    unlock();

    return nextmsg;
}

/*
 * Gibt die Quelle der derzeit zu verarbeitenden Nachricht zurück.
 *
 */
uint8_t queue_get_dir() {
    struct msg *curmsg = shmdata + (shmheader->cur * sizeof(struct msg));
    return curmsg->dir;
}

/*
 * Gibt den Wert der derzeit zu verarbeitenden Nachricht zurück und invalidiert
 * diese.
 *
 */
uint8_t queue_get_data() {
    uint8_t data;

    struct msg *curmsg = shmdata + (shmheader->cur * sizeof(struct msg));
    data = curmsg->data;

    lock();
    curmsg->dir = D_INVALID;
    if (shmheader->cur == 255)
        shmheader->cur = 0;
    else shmheader->cur++;
    unlock();

    return data;
}
