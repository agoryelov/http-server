#include <curses.h>
#include <menu.h>
#include <form.h>
#include <panel.h>
#include <libconfig.h>
#include <stdlib.h>
#include "../http_protocol/config.h"
#include "ncurses_form.h"
#include "ncurses_menu.h"
#include "ncurses_panel.h"
#include "ncurses_shared.h"

static void print_ascii_art_title(WINDOW *window) {
    if (COLS < 44) {
        return; // return if not enough room to print
    }
    char *rows[5] = {
            "__    __  _____  __________    __  ___  __",
            "/ _\\  /__\\/__   \\/__   \\_   \\/\\ \\ \\/ _ \\/ _\\\n",
            "\\ \\  /_\\    / /\\/  / /\\// /\\/  \\/ / /_\\/\\ \\\n",
            "_\\ \\//__   / /    / //\\/ /_/ /\\  / /_\\\\ _\\ \\\n",
            " \\__/\\__/   \\/     \\/ \\____/\\_\\ \\/\\____/ \\__/ \n"
    };
    print_center_horizontal_strings(window, rows, 5, 2);
}

//static void print_instructions(WINDOW *window) {
//    char *rows[4] = {
//            "[F1]         Exit Program",
//            "[Up Arrow]   Scroll Up",
//            "[Up Arrow]   Scroll Up",
//            "[Enter]      Select Setting",
//    };
//    print_center_horizontal_strings(window, rows, 4, 7);
//}

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
    create_main_menu(&main_menu, &lib_config);
    display_main_menu(main_menu, main_menu_window);

    // Loop
    process_menu_input(main_menu, stdscr, main_menu_window);

    // Cleanups
    config_destroy(&lib_config);
    unpost_menu(main_menu);
    free_menu(main_menu);
    delwin(main_menu_window);

    endwin();
    exit_curses(0);
}