#ifndef NCURSES_SHARED_H
#define NCURSES_SHARED_H
#define CONFIG_PATH "../config.cfg"
#include <form.h>

#define MARGIN 1
#define ASCII_TITLE_HEIGHT 5
#define INSTRUCTIONS_HEIGHT 1

typedef struct config_item {
    char *name;
    char *path;
    int config_type;
    FIELDTYPE *field_type;
} config_item_t;

void mvwprintw_center_justify(WINDOW *window, int row, char *fmt);

/**
 * "Trims" the trailing whitespace of the char array
 * by looking for the location after the last non-whitespace
 * character, and setting that as the null terminator.
 * @param str - the char array to trim
 */
void trim_trailing_whitespace(char *str);

/**
 * Converts the number to its string representation and
 * sets the string in the buffer.
 * @param num - the number to convert
 * @param buffer - the buffer to store the string
 */
void convert_int_to_string(int num, char **buffer);

#endif //NCURSES_SHARED_H
