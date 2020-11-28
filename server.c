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
#include <sys/un.h>
#include <sys/wait.h>
#include <errno.h>

#include "http_protocol/thread_pool.h"
#include "shared.h"
#include "http_protocol/http.h"
// TODO: fix backlog issue  
#define BACKLOG 5

#define SOCKET_PATH "/tmp/fd-pass.socket"

void monitor(int socket) {
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE(sizeof(int))], dup[256];
        memset(buf, '\0', sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);
        while(1) {
            if (recvmsg(socket, &msg, 0) < 0)
                printf("Failed to receive mesage\n");

            cmsg = CMSG_FIRSTHDR(&msg);

            int *fdptr = (int *)CMSG_DATA(cmsg);

            
            printf("Got fd: %d\n", *fdptr);
            char request_buf[2048];
            ssize_t num_read;
            int cfd = *fdptr;
            memset(request_buf, 0, 2048); // You will regret removing this line

            num_read = read(cfd, request_buf, 2048);
            
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

void process_pool_loop(){
    int sfd, cfd;
    struct sockaddr_un addr;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sfd == -1)
        printf("Failed to create socket\n");

    if (unlink (SOCKET_PATH) == -1 && errno != ENOENT)
        printf ("Removing socket file failed\n");

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        printf("Failed to bind to socket\n");

    if (listen(sfd, 5) == -1)
        printf("Failed to listen on socket\n");

    cfd = accept(sfd, NULL, NULL);

    monitor(cfd);
    close(sfd);
}

int process_pool_create(){
    pid_t pid = fork();
    if(pid == -1){
        printf("Fork failed\n");
        return -1;
    } else if (pid == 0) {
        return 0;
        //process_pool_loop(); // child

    } else {
        return 0; // parent
    }
    return -1;
}


void process_pool_notify(int sfd, int cfd){
    printf("tried to notify...\n");
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(sizeof(int))], *dup = "hello world";
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof (buf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));

    int *fdptr = (int *)CMSG_DATA(cmsg);
    *fdptr = cfd;

    if (sendmsg(sfd, &msg, 0) == -1)
        printf("Error sending client stdout\n");
    printf("process notified...\n");
}

int main(void) {
    
    struct sockaddr_in addr;
    int sfd;
    signal(SIGPIPE, SIG_IGN);

    sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int optval = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, BACKLOG);
    
    bool mode_threads = false;
    thread_pool* pool = thread_pool_create();
    
    thread_pool_start(pool);


    process_pool_create();
    sleep(2);
    struct sockaddr_un process_address;
    int process_sfd;
    
    process_sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(&process_address, 0, sizeof(struct sockaddr_un));
    process_address.sun_family = AF_UNIX;
    strcpy(process_address.sun_path, SOCKET_PATH);

    if (connect(process_sfd, (struct sockaddr *) &process_address, sizeof(struct sockaddr_un)) == -1)
        printf("Failed to connect to socket\n");


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
        while(!mode_threads) {
            int new_socket_fd = accept(sfd, NULL, NULL);
            process_pool_notify(process_sfd, new_socket_fd);
        }
    }

    //dc_close(sfd); <- never reached because for(;;) never ends.

    return EXIT_SUCCESS;
}

