#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <dc/semaphore.h> 
#include <dc/pthread.h>
#include <dc/unistd.h>
#include "./http.h"

#define NUM_THREADS 10
/**
 * A client fd can be passed to a thread through the use of a shared_data struct.
 * The struct contains semaphores that should be used to allow only a single
 * thread to have access to the client_fd data at a time.
 */
struct shared_data
{
    int client_fd;
    sem_t occupied_semaphore;
    sem_t empty_semaphore;
    sem_t put_semaphore;
    sem_t get_semaphore;
    sem_t killed_semaphore;
};
typedef struct shared_data shared_data;
/**
 * Thread pool struct is used to control a pool of threads and should be created with
 * thread_pool_create.
 */
struct thread_pool {
    struct shared_data * data;
    pthread_t threads [NUM_THREADS];
    bool is_running;
    config *cfg;
};
typedef struct thread_pool thread_pool;

/**
 * Creates NUM_THREADS number of worker threads where each thread will wait
 * the thread_loop function. Sets running to true in the thread_pool struct
 * @param pool
 */
void thread_pool_start(thread_pool* pool);
/**
 * Detaches all of the running threads. Sets running to false in the thread_pool
 * struct. Posts occupied semaphore NUM_THREADS number of times to break each
 * thread out of their wait. Where they will see running is false and pthread_exit.
 * @param pool
 */
void thread_pool_stop(thread_pool* pool);
/**
 * Destroys the semaphores, frees the thread pool struct and its contents.
 * @param pool
 */
void thread_pool_destroy(thread_pool* pool);
/**
 * Creates thread_pool struct and sets it's values, the struct will be used to
 * control the threads.
 * @param config
 * @return thread pool
 */
thread_pool * thread_pool_create(config *cfg);
/**
 * Used to pass a client over the thread pool struct and notify a single thread that
 * it can access the client fd through the uses of semaphores.
 * @param argc
 * @param argv
 * @return
 */
void thread_pool_notify(thread_pool* pool, int cfd);

#endif