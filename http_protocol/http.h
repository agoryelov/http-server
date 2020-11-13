#ifndef HTTP_H
#define HTTP_H

#include "config.h"

#include "../libs/str_map.h"
#include <stdint.h>

#define METHOD_UNSUPPORTED 0
#define METHOD_HEAD 1
#define METHOD_GET 2

typedef struct  {
    int method;
    int response_code;
    char * request_path;
    str_map * header_fields;
    char * response_body;
} http_response;

typedef struct  {
    int method;
    char * request_uri;
    char * http_version;
    str_map * header_fields;
    char * request_body;
} http_request;

typedef struct {
    http_request * (*parse_request)(char *, size_t);
    http_response * (*build_response)(http_request *);
    void (*send_response)(http_response *, int);
    config * my_config;
} http;

http * http_create(config * http_config);
http_request * parse_request(char * request_text, size_t request_len);
http_response * build_response(http_request * request);
void send_response(http_response * response, int cfd);
void http_destroy(http * http);
void http_request_destroy(http_request * request);
void http_response_destroy(http_response * response);

#endif