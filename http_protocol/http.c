#include "http.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CRLF "\r\n"
#define CRLF_CRLF "\r\n\r\n"

static int parse_request_method(char * method);
static char * substring(const char * string, size_t start, size_t end);

http_request * parse_request(char * request_text, size_t request_len) {
    char * end_of_header = strstr(request_text, "\r\n\r\n");
    if (end_of_header == NULL) {
        return NULL;
    }

    char * end_of_request_line = strstr(request_text, "\r\n");
    if (end_of_request_line == NULL) {
        return NULL;
    }
    
    // Parsing according to example at: https://linux.die.net/man/3/strtok_r
    // It aint pretty but it works ¯\_(ツ)_/¯

    char * request_body = strdup(end_of_header);
    int request_body_len = strlen(end_of_header);

    char * request_header = substring(request_text, 0, request_len - request_body_len);
    char * saveptr1, * saveptr2, * saveptr3;
    char * request_line = strtok_r(request_header, "\r\n", &saveptr1);
    
    char * method_str = strtok_r(request_line, " ", &saveptr2);
    char * uri_str = strtok_r(NULL, " ", &saveptr2);
    char * version_str = strtok_r(NULL, " ", &saveptr2);

    str_map * fields_map = sm_create(4);
    char * header_field = strtok_r(NULL, "\r\n", &saveptr1);
    while (header_field != NULL) {
        char * lhs = strtok_r(header_field, ":", &saveptr3);
        char * rhs = strtok_r(NULL, ":", &saveptr3);
        //printf("field= %s:%s\n", lhs, rhs);
        sm_put(fields_map, lhs, rhs);
        header_field = strtok_r(NULL, "\r\n", &saveptr1);
    }

    http_request * request = malloc(sizeof(http_request));
    
    request->method = parse_request_method(method_str);
    request->request_path = strdup(uri_str);
    request->http_version = strdup(version_str);
    request->request_body = request_body;
    request->header_fields = fields_map;

    free(request_header);

    return request;
}

static char * substring(const char * string, size_t start, size_t end) {
    size_t out_len = end - start;
    char * out = malloc(out_len + 1);
    for (size_t i = 0; i < out_len; i++) {
        out[i] = string[start + i];
    }
    out[out_len] = '\0';
    return out;
}

static int parse_request_method(char * method) {
    if (strcmp(method, "GET") == 0) {
        return METHOD_GET;
    } 
    
    if (strcmp(method, "HEAD") == 0) {
        return METHOD_HEAD;
    }

    return METHOD_UNSUPPORTED;
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

void http_request_destroy(http_request * request) {
    if (request == NULL) return;

    sm_destroy(request->header_fields);
    free(request->http_version);
    free(request->request_path);
    free(request->request_body);
    free(request);
}

void http_destroy(http * http) {
    free(http);
}
