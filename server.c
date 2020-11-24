#include <dc/stdio.h>
#include <dc/unistd.h>
#include <dc/sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h> 
#include <pthread.h>
#include <fcntl.h>
#include "shared.h"
#include "http_protocol/http.h"
// TODO: fix backlog issue  
#define BACKLOG 5


struct Data
{
    int client_fd;
    int thread_id;
    sem_t occupied_semaphore;
    sem_t empty_semaphore;
    sem_t put_semaphore;
    sem_t get_semaphore;
    bool running;
};

void* thread_func(void* arg){
    struct Data *data;
    data = arg;
    int my_thread_id = -1;
    

    sem_wait(&data->occupied_semaphore);
    sem_wait(&data->get_semaphore);
    my_thread_id = data->thread_id;
    printf("thread #%d created\n", my_thread_id);
    sem_post(&data->get_semaphore);
    sem_post(&data->empty_semaphore);

    sleep(2);
    char request_buf[BUF_SIZE];
    for(;;) {
        sem_wait(&data->occupied_semaphore);
        sem_wait(&data->get_semaphore);
        
        ssize_t num_read;
        
        memset(request_buf, 0, BUF_SIZE); // You will regret removing this line
        int x = data->client_fd;
        //printf("thread #%d...\n", my_thread_id);
        
        sem_post(&data->get_semaphore);
        sem_post(&data->empty_semaphore);
        num_read = dc_read(x, request_buf, BUF_SIZE);
        //printf("request: %s\n", request_buf);

        http * my_http = http_create(NULL);
        http_request * request = my_http->parse_request(request_buf, num_read);
        http_response * response = build_response(request);
        printf("thread #%d sleeping...\n", my_thread_id);
        sleep(5);
        printf("thread #%d woke up...\n", my_thread_id);
        //char * response = "HTTP/1.0 404 Not Found\r\n\r\n";
        //int response_len = strlen(response);
        //dc_write(x, response, response_len);
        send_response(response, x);
        
        http_request_destroy(request);
        http_response_destroy(response);
        http_destroy(my_http);
        dc_close(x);

    }
}

int main(void) {
    int new_socket;
    struct sockaddr_in addr;
    struct sockaddr_storage addr_storage;
    int sfd;

    sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, BACKLOG);
    
    struct Data data;
    data.client_fd = 0;

    sem_init(&data.occupied_semaphore, 0, 0);
    sem_init(&data.empty_semaphore, 0, 1);
    sem_init(&data.put_semaphore, 0, 1);
    sem_init(&data.get_semaphore, 0, 1);

    data.running = 0;
    for(int i = 0; i < 20; i++) {
        pthread_t thread;
        sem_wait(&data.empty_semaphore);
        sem_wait(&data.put_semaphore);
        data.thread_id = i;
        pthread_create(&thread, NULL, thread_func, &data);
        sem_post(&data.put_semaphore);
        sem_post(&data.occupied_semaphore);
        
        
    }
    // sem_wait(&data.empty_semaphore);
    // sem_wait(&data.put_semaphore);
    // data.running = 1;
    // sem_post(&data.put_semaphore);
    // sem_post(&data.occupied_semaphore);

    socklen_t addr_size;
    for(;;)
    {
        //addr_size = sizeof(addr_storage);
        
        sem_wait(&data.empty_semaphore);
        sem_wait(&data.put_semaphore);
        new_socket = accept(sfd, NULL, NULL);
        data.client_fd = new_socket;
        
        sem_post(&data.put_semaphore);
        sem_post(&data.occupied_semaphore);
    }

    // //dc_close(sfd); <- never reached because for(;;) never ends.

    return EXIT_SUCCESS;
}
