#include <dc/stdio.h>
#include <dc/unistd.h>
#include <dc/sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fcntl.h>
#include "shared.h"
#include "http_protocol/http.h"

#define BACKLOG 5

int main(void) {
    struct sockaddr_in addr;
    int sfd;

    sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, BACKLOG);
    
    for(;;)
    {
        int cfd;
        ssize_t num_read;
        char request_buf[BUF_SIZE];
        memset(request_buf, 0, BUF_SIZE); // You will regret removing this line

        cfd = dc_accept(sfd, NULL, NULL);
        num_read = dc_read(cfd, request_buf, BUF_SIZE);

        http * my_http = http_create(NULL);
        http_request * request = my_http->parse_request(request_buf, num_read);
        http_response * response = build_response(request);

        send_response(response, cfd);
        http_request_destroy(request);
        http_response_destroy(response);
        http_destroy(my_http);

        dc_close(cfd);
    }

    // //dc_close(sfd); <- never reached because for(;;) never ends.

    return EXIT_SUCCESS;
}
