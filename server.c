#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <errno.h>

#include "http_protocol/thread_pool.h"
#include "http_protocol/process_pool.h"
#include "shared.h"
#include "http_protocol/http.h"

#define BACKLOG 5

static int create_server_fd();

int main(void) {
    int server_fd = create_server_fd();
    http * my_http = http_create(NULL);
    process_pool * p_pool = process_pool_create(my_http);
    process_pool_start(p_pool);
    for(;;) {
        int client_fd = accept(server_fd, NULL, NULL);
        process_pool_notify(p_pool, client_fd);
    }
    return EXIT_SUCCESS;
}

static int create_server_fd() {
    struct sockaddr_in addr;
    int sfd;
    signal(SIGPIPE, SIG_IGN);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int optval = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    listen(sfd, BACKLOG);
    return sfd;
}