#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#define CONFIG_PATH "../config.cfg"

config * get_config();
void destroy_config(config *cfg);
void set_default_config(config *cfg);
void set_file_config(config *cfg);
void set_env_config(config *cfg);
void set_cmd_line_config(config *cfg);

/**
 * Sets the config based on values in the order:
 * 1) Default hard-coded values
 * 2) The config file
 * 3) Environment variables
 * 4) Command line arguments
 * @return - pointer to the config
 */
config * get_config() {
    config * cfg = malloc(sizeof(config));
    set_default_config(cfg);
    set_file_config(cfg);
    return cfg;
}

/**
 * Frees memory in config.
 * @param cfg - the config
 */
void destroy_config(config *cfg) {
    free(cfg->root_dir);
    free(cfg->not_found_page);
    free(cfg->index_page);
}

/**
 * Sets hard coded values for the config.
 * @param cfg - the config
 */
void set_default_config(config *cfg) {
    cfg->root_dir = "../server_directory";
    cfg->index_page = "/index.html";
    cfg->not_found_page = "/404.html";
    cfg->mode = 1;
}

/**
 * Sets values based on the config file for the config.
 * @param cfg - the config
 */
void set_file_config(config *cfg) {
    config_t lib_config;
    config_init(&lib_config);

    if (!config_read_file(&lib_config, CONFIG_PATH)) {
        printf("%s:%d - %s\n", config_error_file(&lib_config), config_error_line(&lib_config), config_error_text(&lib_config));
        config_destroy(&lib_config);
        return;
    }
    config_lookup_int(&lib_config, "mode", &cfg->mode);

    const char *root_dir, *index_page, *not_found_page;
    config_lookup_string(&lib_config, "directories.root", &root_dir);
    config_lookup_string(&lib_config, "pages.index", &index_page);
    config_lookup_string(&lib_config, "pages.not_found", &not_found_page);

    cfg->root_dir = malloc(sizeof(*root_dir) + 1);
    cfg->index_page = malloc(sizeof(*index_page) + 1);
    cfg->not_found_page = malloc(sizeof(*not_found_page) + 1);

    strncpy(cfg->root_dir, root_dir, strlen(root_dir) + 1);
    strncpy(cfg->index_page, index_page, strlen(index_page) + 1);
    strncpy(cfg->not_found_page, not_found_page, strlen(not_found_page) + 1);

    config_destroy(&lib_config);
}
