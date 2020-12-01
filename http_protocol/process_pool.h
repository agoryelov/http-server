#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H


#include "./http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h> 
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <errno.h>

#define POOL_SIZE 10
#define SEM_WORKER_READY "/worker_ready"
#define SEM_WORKER_BINDED "/worker_binded"
#define SEM_WAKE_WORKER "/wale_worker"
#define SOCKET_PATH "/tmp/fd-pass.socket"

typedef struct {
    sem_t * worker_ready;
    sem_t * worker_binded;
    sem_t * wake_worker;
} semaphores;

typedef struct {
    semaphores * sem;
    http * http_handler;
} process_pool;

void process_pool_start(process_pool * pool);
void process_pool_stop(process_pool * pool);
void process_pool_destroy(process_pool * pool);
process_pool * process_pool_create(http * http_hander);
void process_pool_notify(process_pool * pool, int cfd);

#endif
