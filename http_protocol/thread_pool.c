#include "thread_pool.h"

#define CLIENT_READ_BUF 2048

static void * process_thread_loop(void * arg){
    thread_pool *pool = arg;
    shared_data *data = pool->data;

    char request_buf[CLIENT_READ_BUF];
    for(;;) {
        if(pool->is_running == false) return NULL;
        sem_wait(&data->occupied_semaphore);
        sem_wait(&data->get_semaphore);
        
        int cfd = data->client_fd;
        
        sem_post(&data->get_semaphore);
        sem_post(&data->empty_semaphore);
        
        ssize_t num_read;
        
        memset(request_buf, 0, CLIENT_READ_BUF); // You will regret removing this line

        num_read = read(cfd, request_buf, CLIENT_READ_BUF);
        
        http * my_http = http_create(NULL); //get config?
        http_request * request = my_http->parse_request(request_buf, num_read);
        http_response * response = build_response(request);

        send_response(response, cfd);
        
        http_request_destroy(request);
        http_response_destroy(response);
        http_destroy(my_http);
        close(cfd);
    }
}
void thread_pool_start(thread_pool* pool){
    pool->is_running = true;
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_t thread;
        pool->threads[i] = pthread_create(&thread, NULL, process_thread_loop, pool);   
    }
}

void thread_pool_stop(thread_pool* pool){
    shared_data *data = pool->data;
    pool->is_running = false;

    for(int i = 0; i < NUM_THREADS; i++){
        sem_post(&data->put_semaphore);
        sem_post(&data->occupied_semaphore);
    }
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(pool->threads[i], NULL);
    }
    
}
void thread_pool_destroy(thread_pool* pool){
    free(pool->data);
    free(pool);
}

thread_pool * thread_pool_create(){
    thread_pool *pool = malloc(sizeof(thread_pool));
    shared_data *data = malloc(sizeof(shared_data));
    pool->is_running = false;

    sem_init(&data->occupied_semaphore, 0, 0);
    sem_init(&data->empty_semaphore, 0, 1);
    sem_init(&data->put_semaphore, 0, 1);
    sem_init(&data->get_semaphore, 0, 1);

    pool->data = data;
    return pool;
}

void thread_pool_notify(thread_pool* pool, int cfd){
    shared_data *data;
    data = pool->data;
    sem_wait(&data->empty_semaphore);
    sem_wait(&data->put_semaphore);
    
    data->client_fd = cfd;

    sem_post(&data->put_semaphore);
    sem_post(&data->occupied_semaphore);
}