#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H


#include "./http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h> 
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <errno.h>

#define NUM_PROCESSES 10
/**
 * The semaphores struct holds the named semaphores that can be used in other
 * process to control the process.
 */
typedef struct {
    sem_t * worker_ready;
    sem_t * worker_binded;
    sem_t * wake_worker;
} semaphores;
/**
 * The memory struct holds a is_running value that will be stored in shared memory
 * for the processes to check if they should continue running.
 */
typedef struct memory {
    bool is_running;
} memory;

/**
 * The process pool struct contains everything you need to control the processes.
 */
typedef struct {
    semaphores * sem;
    memory * mem;
    char ** argv;
    int argc;
} process_pool;

/**
 * Forks NUM_PROCESSES number of worker processes where each forked process will wait
 * the worker_loop function.
 * @param pool
 */
void process_pool_start(process_pool * pool);
/**
 * Sets the shared memory is_running bool to false then posts every worker processes
 * to wake up. Once woken the worker processes will exit success.
 * @param pool
 */
void process_pool_stop(process_pool * pool);
/**
 * Frees the process pool struct.
 * @param pool
 */
void process_pool_destroy(process_pool * pool);
/**
 * Sets up everything the process pool needs before starting and returns a
 * process pool struct holding the command line args and created shared memory for
 * managing whether or not the processes are running or not.
 * @param argc
 * @param argv
 * @return process_pool
 */
process_pool * process_pool_create(int argc, char ** argv);

/**
 * Used to pass a client to a process through the uses of semaphores and domain sockets.
 * @param pool
 * @param cfd
 */
void process_pool_notify(process_pool * pool, int cfd);

#endif
