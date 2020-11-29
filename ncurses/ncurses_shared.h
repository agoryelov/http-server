#ifndef NCURSES_SHARED_H
#define NCURSES_SHARED_H
#define CONFIG_PATH "../config.cfg"
#include <form.h>

typedef struct config_entry {
    int type;
    char *path;
    FIELDTYPE *field_type;
} config_entry_t;

void print_center_horizontal_strings(WINDOW *window, char **fmt, int fmt_count, int row);
void print_center_horizontal_string(WINDOW *window, char *fmt, int row);

/**
 * "Trims" the trailing whitespace of the char array
 * by looking for the location after the last non-whitespace
 * character, and setting that as the null terminator.
 * @param str - the char array to trim
 */
void trim_trailing_whitespace(char *str);

#endif //NCURSES_SHARED_H