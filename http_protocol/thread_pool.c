#include "thread_pool.h"

#include "./config.h"
/**
 * The loop uses semaphores to post that a thread is ready for work then waits until a thread
 * should be woken. Once woken the thread will exit if is_running is false in the thread_pool object.
 * It will proceed to grab the client fd from the shared data then posts that it has the client fd.
 * Once it has the client fd it will handle the http request.
 * @param pool
 */
static void * thread_loop(void * arg){
    thread_pool *pool = arg;
    shared_data *data = pool->data;

    for(;;) {
        dc_sem_wait(&data->occupied_semaphore);
        if(pool->is_running == false) {
            dc_sem_post(&data->killed_semaphore);
            pthread_exit(NULL);
        }
        dc_sem_wait(&data->get_semaphore);
        
        int cfd = data->client_fd;
        
        dc_sem_post(&data->get_semaphore);
        dc_sem_post(&data->empty_semaphore);

        config * conf = get_config(pool->cfg);
        http_handle_client(conf, cfd);
        destroy_config(conf);

        close(cfd);
    }
}
void thread_pool_start(thread_pool* pool){
    pool->is_running = true;
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_t thread;
        pool->threads[i] = dc_pthread_create(&thread, NULL, thread_loop, pool);   
    }
}

void thread_pool_stop(thread_pool* pool){
    shared_data *data = pool->data;

    for(int i = 0; i < NUM_THREADS; i++) {
        dc_pthread_detach(pool->threads[i]);
    }
    
    pool->is_running = false;

    for(int i = 0; i < NUM_THREADS; i++) {
        dc_sem_post(&data->occupied_semaphore);
    }    
}

void thread_pool_destroy(thread_pool * pool) {
    shared_data *data = pool->data;

    for(int i = 0; i < NUM_THREADS; i++) {
        dc_sem_wait(&data->killed_semaphore);
    }
    
    dc_sem_destroy(&data->occupied_semaphore);
    dc_sem_destroy(&data->empty_semaphore);
    dc_sem_destroy(&data->put_semaphore);
    dc_sem_destroy(&data->get_semaphore);
    dc_sem_destroy(&data->killed_semaphore);

    free(data);
    free(pool);
}

thread_pool * thread_pool_create(config *cfg) {
    thread_pool *pool = calloc(1, sizeof(thread_pool));
    shared_data *data = calloc(1, sizeof(shared_data));
    pool->is_running = false;
    pool->cfg = cfg;

    dc_sem_init(&data->occupied_semaphore, 0, 0);
    dc_sem_init(&data->empty_semaphore, 0, 1);
    dc_sem_init(&data->put_semaphore, 0, 1);
    dc_sem_init(&data->get_semaphore, 0, 1);
    dc_sem_init(&data->killed_semaphore, 0, 0);

    pool->data = data;
    return pool;
}

void thread_pool_notify(thread_pool* pool, int cfd){
    shared_data *data;
    data = pool->data;
    dc_sem_wait(&data->empty_semaphore);
    dc_sem_wait(&data->put_semaphore);
    
    data->client_fd = cfd;

    dc_sem_post(&data->put_semaphore);
    dc_sem_post(&data->occupied_semaphore);
}