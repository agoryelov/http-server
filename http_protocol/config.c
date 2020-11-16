#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "config.h"

#define CONFIG_PATH "../config.cfg"

void set_default_config(config *cfg);

void set_file_config(config *cfg);

void set_env_config(config *cfg);

void set_cmd_line_config(config *cfg, int argc, char **argv);

config *get_config(int argc, char **argv) {
    config *cfg = malloc(sizeof(config));
    set_default_config(cfg);
    set_file_config(cfg);
    set_env_config(cfg);
    set_cmd_line_config(cfg, argc, argv);
    return cfg;
}

void destroy_config(config *cfg) {
    free(cfg->root_dir);
    free(cfg->not_found_page);
    free(cfg->index_page);
    free(cfg->mode);
    free(cfg);
}

void set_default_config(config *cfg) {
    cfg->root_dir = strdup("../server_directory");
    cfg->index_page = strdup("/index.html");
    cfg->not_found_page = strdup("/404.html");
    cfg->mode = strdup("processes");
    cfg->port = 80;
}

/**
 * Sets values based on the config file for the config.
 * @param cfg - the config
 */
void set_file_config(config *cfg) {
    config_t lib_config;
    config_init(&lib_config);

    if (!config_read_file(&lib_config, CONFIG_PATH)) {
        printf("%s:%d - %s\n", config_error_file(&lib_config), config_error_line(&lib_config),
               config_error_text(&lib_config));
        config_destroy(&lib_config);
        return;
    }

    config_lookup_int(&lib_config, "port", &cfg->port);

    const char *root_dir, *index_page, *not_found_page, *mode;
    if (config_lookup_string(&lib_config, "mode", &mode) != CONFIG_FALSE) {
        free(cfg->mode);
        cfg->mode = strdup(mode);
    }
    if (config_lookup_string(&lib_config, "directories.root", &root_dir) != CONFIG_FALSE) {
        free(cfg->root_dir);
        cfg->root_dir = strdup(root_dir);
    }
    if (config_lookup_string(&lib_config, "pages.index", &index_page) != CONFIG_FALSE) {
        free(cfg->index_page);
        cfg->index_page = strdup(index_page);
    }
    if (config_lookup_string(&lib_config, "pages.not_found", &not_found_page) != CONFIG_FALSE) {
        free(cfg->not_found_page);
        cfg->not_found_page = strdup(not_found_page);
    }

    config_destroy(&lib_config);
}

/**
 * Sets the environment variables as values for the config.
 * @param cfg - the config
 */
void set_env_config(config *cfg) {
    char *env_var;
    if ((env_var = getenv("DC_HTTP_PORT")) != NULL) {
        char *ptr;
        int port = (int) strtoul(env_var, &ptr, 0);
        if (*env_var != '\0' && *ptr == '\0') {
            cfg->port = port;
        }
    }
    if ((env_var = getenv("DC_HTTP_MODE")) != NULL) {
        free(cfg->mode);
        cfg->mode = strdup(env_var);
    }
    if ((env_var = getenv("DC_HTTP_ROOT_DIR")) != NULL) {
        free(cfg->root_dir);
        cfg->root_dir = strdup(env_var);
    }
    if ((env_var = getenv("DC_HTTP_INDEX_PAGE")) != NULL) {
        free(cfg->index_page);
        cfg->index_page = strdup(env_var);
    }
    if ((env_var = getenv("DC_HTTP_NOT_FOUND_PAGE")) != NULL) {
        free(cfg->not_found_page);
        cfg->not_found_page = strdup(env_var);
    }
}

/**
 * Parses command line arguments for any options passed in,
 * and sets any valid values for the config.
 * Valid options are: port, mode, root-dir, index-page, not-found-page
 * @param cfg - the config
 * @param argc - arg count
 * @param argv - arg values
 */
void set_cmd_line_config(config *cfg, int argc, char **argv) {
    int opt;
    int opt_index = 0;
    struct option long_options[] = {
            {"port",           optional_argument, 0, 'p'},
            {"mode",           optional_argument, 0, 'm'},
            {"root-dir",       optional_argument, 0, 'r'},
            {"index-page",     optional_argument, 0, 'i'},
            {"not-found-page", optional_argument, 0, 'n'},
    };

    while ((opt = getopt_long(argc, argv, "", long_options, &opt_index)) != -1) {
        if (optarg == NULL) {
            continue;
        }

        switch (opt) {
            case 'p': {
                char *ptr;
                int port = (int) strtoul(optarg, &ptr, 0);
                if (*optarg != '\0' && *ptr == '\0') {
                    cfg->port = port;
                }
                break;
            }
            case 'm':
                free(cfg->mode);
                cfg->mode = strdup(optarg);
                break;
            case 'r':
                free(cfg->root_dir);
                cfg->root_dir = strdup(optarg);
                break;
            case 'i':
                free(cfg->index_page);
                cfg->index_page = strdup(optarg);
                break;
            case 'n':
                free(cfg->not_found_page);
                cfg->not_found_page = strdup(optarg);
                break;
            default:
                break;
        }
    }
}
