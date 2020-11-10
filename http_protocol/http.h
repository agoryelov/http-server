#include "http_request.h"
#include "http_response.h"
#include "config.h"

typedef struct {
    http_request * (*parse_request)(char *);
    http_response * (*build_response)(http_request *);
    void (*send_response)(http_response *, int);
    config * my_config;
} http;

http * http_create(config * http_config);
http_request * parse_request(char * request_text);
http_response * build_response(http_request * request);
void send_response(http_response * response, int cfd);