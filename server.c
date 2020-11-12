#include <dc/stdio.h>
#include <dc/unistd.h>
#include <dc/sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "shared.h"
#include "http_protocol/http.h"

#define BACKLOG 5

void print_http_request(http_request * request) {
    printf("method (0 = ERROR, 1 = HEAD, 2 = GET): %d\n", request->method);
    printf("path: %s\n", request->request_path);
    printf("http_version: %s\n", request->http_version);
    sm_print(request->header_fields);
    printf("body: %s\n", request->request_body);
}

int main(void)
{
    char * request_static = "GET / HTTP/1.1\r\nHost: localhost\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n";
    
    for (size_t i = 0; i < 10; i++) {
        char * request = strdup(request_static);
        int request_len = strlen(request);

        http * test_http = http_create(NULL);
        http_request * test_request = test_http->parse_request(request, request_len);
        print_http_request(test_request);

        free(request);
        http_request_destroy(test_request);
        http_destroy(test_http);
    }

    // struct sockaddr_in addr;
    // int sfd;

    // sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    // memset(&addr, 0, sizeof(struct sockaddr_in));
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(PORT);
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    // dc_listen(sfd, BACKLOG);
    
    // for(;;)
    // {
    //     int cfd;
    //     ssize_t num_read;
    //     char buf[BUF_SIZE];
    //     memset(buf, 0, BUF_SIZE); // You will regret removing this line

    //     cfd = dc_accept(sfd, NULL, NULL);
    //     num_read = dc_read(cfd, buf, BUF_SIZE);

    //     //dc_write(STDOUT_FILENO, buf, num_read);
    //     if (num_read > 0) {
    //         http * my_http = http_create(NULL);
    //         http_request * new_request = my_http->parse_request(buf, num_read);
    //         print_http_request(new_request);
    //         http_request_destroy(new_request);
    //         http_destroy(my_http);
    //     }

    //     char * response = "HTTP/1.0 404 Not Found\r\n\r\n";
    //     int response_length = strlen(response);
    //     dc_write(cfd, response, response_length);

    //     dc_close(cfd);
    // }

    //dc_close(sfd); <- never reached because for(;;) never ends.

    return EXIT_SUCCESS;
}
