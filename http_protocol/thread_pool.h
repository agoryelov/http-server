#include <semaphore.h> 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "./http.h"

#define NUM_THREADS 10

struct shared_data
{
    int client_fd;
    sem_t occupied_semaphore;
    sem_t empty_semaphore;
    sem_t put_semaphore;
    sem_t get_semaphore;
};
typedef struct shared_data shared_data;

struct thread_pool {
    struct shared_data *data;
    pthread_t threads [NUM_THREADS];
    int num_threads;
    bool is_running;
    http * http_handler;
};
typedef struct thread_pool thread_pool;

void thread_pool_start(thread_pool* pool);
void thread_pool_stop(thread_pool* pool);
void thread_pool_destroy(thread_pool* pool);
thread_pool * thread_pool_create();
void thread_pool_notify(thread_pool* pool, int cfd);
