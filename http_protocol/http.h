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
} http_response;

typedef struct  {
    int method;
    char * request_uri;
    char * http_version;
    str_map * header_fields;
    char * request_body;
} http_request;

/**
 * Parses an http request of size request_len and returns request content
 * formatted into a new http_request struct.
 */
http_request * parse_request(char * request_text, size_t request_len);

/**
 * Builds an http_response based on the passed in http_request.
 */
http_response * build_response(config * conf, http_request * request);

/**
 * Sends an http_response to the socket file descriptor specified by cfd
 */
void send_response(http_response * response, int cfd);

/**
 * Destroys an http_request and performs any other necessary clean up.
 */
void http_request_destroy(http_request * request);

/**
 * Destroys an http_response and performs any other necessary clean up.
 */
void http_response_destroy(http_response * response);

/**
 * High-level interface to handle an http request from a client on socket. This function
 * makes use of parse_request, build_response, and send_response to handle a request
 * from a socket specified by cfd.
 */
void http_handle_client(config * conf, int cfd);

#endif