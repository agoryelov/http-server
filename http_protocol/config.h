#ifndef CONFIG_H
#define CONFIG_H

#include <libconfig.h>

/**
 * The config struct.
 */
typedef struct {
    char * root_dir;
    char * index_page;
    char * not_found_page;
    char * mode;
    int port;
} config;

/**
 * Frees memory in the config.
 * @param cfg - the config
 */
void destroy_config(config *cfg);

/**
 * Sets the config based on values in the order (smaller is set first):
 * 1) Default hard-coded values
 * 2) The config file
 * 3) Environment variables
 * 4) Command line arguments
 * @return - pointer to the config
 */
config * get_config(int argc, char **argv);

#endif
