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
#include <signal.h>

#include "http_protocol/thread_pool.h"
#include "shared.h"
#include "http_protocol/http.h"
// TODO: fix backlog issue  
#define BACKLOG 5

int main(void) {
    
    struct sockaddr_in addr;
    int sfd;
    signal(SIGPIPE, SIG_IGN);

    sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, BACKLOG);
    
    bool mode_threads = true;
    thread_pool* pool = thread_pool_create();

    thread_pool_start(pool);
    for(;;){
        while(mode_threads){
            // poll for config
            // if(0){ // config switches to processes
            //     thread_pool_stop(pool);
            //     mode_threads = false;
            // }
            int new_socket_fd = accept(sfd, NULL, NULL);
            thread_pool_notify(pool, new_socket_fd);

        }
        while(!mode_threads){
            //get config
            // process with processes
        }
    }

    //dc_close(sfd); <- never reached because for(;;) never ends.

    return EXIT_SUCCESS;
}
