#include "http.h"
#include <string.h>
#include <stdio.h>

http_request * parse_request(char * request_text) {
    int request_length = strlen(request_text);

    char * request_method = strtok(request_text, " ");
    char * request_path = strtok(NULL, " ");

    // TODO: Create http_request object and return it
    printf("request_method: %s\n", request_method);
    printf("request_path: %s\n", request_path);

    return (void *)0;
}

http_response * build_response(http_request * request) {
    // TODO: Build response based on request and return it

    return (void *)0;
}

void send_response(http_response * response, int cfd) {
    // TODO: Write response to specified cfd
    // TODO: Need to find Content-Length somewhere beforehand
}

http * http_create(config * http_config) {
    http * new_http = malloc(sizeof(http));

    new_http->my_config = http_config;
    new_http->parse_request = parse_request;
    new_http->send_response = send_response;
    new_http->build_response = build_response;

    return new_http;
}

