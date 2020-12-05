#ifndef CONFIG_H
#define CONFIG_H

#include <libconfig.h>

#define MAX_PORT 65535

/**
 * The config struct.
 */
typedef struct {
    char *root_dir;
    char *index_page;
    char *not_found_page;
    char mode;
    int port;
} config;

/**
 * Frees memory in the config.
 * @param cfg - the config
 */
void destroy_config(config *cfg);

/**
 * Gets the cmd line config.
 * @param argc - argc
 * @param argv - argv
 * @return - the cmd line config
 */
config *get_cmd_config(int argc, char **argv);

/**
 * Sets the config based on values in the order (smaller is set first):
 * 1) Default hard-coded values
 * 2) The config file
 * 3) Environment variables
 * 4) Command line arguments
 * @param config - the config
 * @return - pointer to the config
 */
config *get_config(config *cmd_cfg);

#endif
