#include <curses.h>
#include <menu.h>
#include <form.h>
#include <panel.h>
#include <libconfig.h>
#include <stdlib.h>
#include <string.h>
#include "../http_protocol/config.h"
#include "ncurses_form.h"
#include "ncurses_menu.h"
#include "ncurses_panel.h"
#include "ncurses_shared.h"

static void print_ascii_art_title(WINDOW *window) {
    int title_cols = 44;
    if (COLS < title_cols) {
        return; // return if not enough room to print
    }
    char *rows[5] = {
        "__    __  _____  __________    __  ___  __",
        "/ _\\  /__\\/__   \\/__   \\_   \\/\\ \\ \\/ _ \\/ _\\\n",
        "\\ \\  /_\\    / /\\/  / /\\// /\\/  \\/ / /_\\/\\ \\\n",
        "_\\ \\//__   / /    / //\\/ /_/ /\\  / /_\\\\ _\\ \\\n",
        "\\__/\\__/   \\/     \\/ \\____/\\_\\ \\/\\____/ \\__/ \n"
    };
    int midpoint_x = getmaxx(window) / 2;
    mvwprintw(window, 2, midpoint_x - title_cols / 2, rows[0]);
    mvwprintw(window, 3, midpoint_x - title_cols / 2, rows[1]);
    mvwprintw(window, 4, midpoint_x - title_cols / 2, rows[2]);
    mvwprintw(window, 5, midpoint_x - title_cols / 2, rows[3]);
    mvwprintw(window, 6, midpoint_x - title_cols / 2, rows[4]);
}

static void print_instructions(WINDOW *window) {
    char *text = "[F1] Exit    [^] Scroll Up    [v] Scroll Down    [Enter] Select";
    int midpoint_x = getmaxx(window) / 2;
    mvwprintw(window, 8, midpoint_x - 63 / 2, text);
}

int main() {
    // Declaration
    MENU *main_menu = NULL;
    WINDOW *main_menu_window = NULL;
    config_t lib_config;

    // Setup
    initscr();
    refresh();
    set_keyboard_menu();
    print_ascii_art_title(stdscr);
    print_instructions(stdscr);
    create_main_menu(&main_menu, &lib_config);
    display_main_menu(main_menu, main_menu_window);

    // Loop
    process_menu_input(main_menu, &lib_config, main_menu_window);

    // Cleanup
    config_destroy(&lib_config);
    unpost_menu(main_menu);
    free_menu(main_menu);
    delwin(main_menu_window);

    endwin();
    exit_curses(0);
}
