#include "ncurses_shared.h"
#include <curses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void mvwprintw_center_justify(WINDOW *window, int row, char *fmt) {
    size_t midpoint_x = getmaxx(window) / 2;
    mvwprintw(window, row, midpoint_x - strlen(fmt) / 2, fmt);
}

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

void convert_int_to_string(int num, char **buffer) {
    int length = snprintf(NULL, 0, "%d", num);
    *buffer = calloc(length + 1, sizeof(char));
    sprintf(*buffer, "%d", num);
}
