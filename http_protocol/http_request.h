
#define HEAD 0
#define GET 1

typedef struct  {
    int request_type;
    char * request_path;
} http_request;