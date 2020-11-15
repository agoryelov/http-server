#ifndef CONFIG_H
#define CONFIG_H

#include <libconfig.h>

typedef struct {
    char * root_dir;
    char * index_page;
    char * not_found_page;
    int mode;
} config;

void destroy_config(config *cfg);
config * get_config();

#endif
