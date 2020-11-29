#include "ncurses_shared.h"
#include <curses.h>
#include <string.h>
#include <ctype.h>

/**
 * "Trims" the trailing whitespace of the char array
 * by looking for the location after the last non-whitespace
 * character, and setting that as the null terminator.
 * @param str - the char array to trim
 */
void trim_trailing_whitespace(char *str) {
    if (str[0] == '\0') {
        return;
    }
    char *ptr = &str[0];
    for (size_t i = 0; str[i+1] != '\0'; ++i) {
        if (!isspace(str[i]) && isspace(str[i+1])) {
            ptr = &str[i+1];
        }
    }
    *ptr = '\0';
}

void print_center_horizontal_strings(WINDOW *window, char **fmt, int fmt_count, int row) {
    size_t midpoint_x = getmaxx(window) / 2;
    for (int i = 0; i < fmt_count; ++i) {
        mvwprintw(window, row + i, midpoint_x - strlen(fmt[i]) / 2, fmt[i]);
    }
}

void print_center_horizontal_string(WINDOW *window, char *fmt, int row) {
    size_t midpoint_x = getmaxx(window) / 2;
    mvwprintw(window, row, midpoint_x - strlen(fmt) / 2, fmt);
}