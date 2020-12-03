#include <curses.h>
#include <menu.h>
#include <libconfig.h>
#include <string.h>
#include "ncurses_menu.h"
#include "ncurses_shared.h"

void exit_curses(int);

static void print_ascii_art_title() {
    int title_cols = 44;
    if (COLS < title_cols) {
        return; // return if not enough room to print
    }
    char *rows[ASCII_TITLE_HEIGHT] = {
        " __    __  _____  __________    __  ___  __",
        "/ _\\  /__\\/__   \\/__   \\_   \\/\\ \\ \\/ _ \\/ _\\\n",
        "\\ \\  /_\\    / /\\/  / /\\// /\\/  \\/ / /_\\/\\ \\\n",
        "_\\ \\//__   / /    / //\\/ /_/ /\\  / /_\\\\ _\\ \\\n",
        "\\__/\\__/   \\/     \\/ \\____/\\_\\ \\/\\____/ \\__/ \n"
    };
    int midpoint_x = COLS / 2;
    int start_row = MARGIN;
    for (int i = 0; i < ASCII_TITLE_HEIGHT; ++i) {
        mvwprintw(stdscr, start_row + i, midpoint_x - title_cols / 2, rows[i]);
    }
}

static void print_instructions() {
    int instruction_cols = 63;
    if (COLS < instruction_cols) {
        return; // return if not enough room to print
    }
    char *text = "[F1] Exit    [^] Scroll Up    [v] Scroll Down    [Enter] Select";
    int midpoint_x = COLS / 2;
    mvwprintw(stdscr, MARGIN * 2 + ASCII_TITLE_HEIGHT, midpoint_x - instruction_cols / 2, text);
}

int main() {
    // Declaration
    MENU *main_menu = NULL;
    WINDOW *main_menu_window = NULL;
    config_item_t *config_items[NUM_ITEMS + 1] = { NULL };
    ITEM *items[NUM_ITEMS + 1] = { NULL };
    config_t lib_config;

    // Setup
    initscr();
    refresh();
    print_ascii_art_title();
    print_instructions();
    create_main_menu(&main_menu, &lib_config, config_items, items);
    main_menu_window = derwin(stdscr, LINES - 11, COLS - 8, 10, 4);
    display_main_menu(main_menu, main_menu_window);
    box(stdscr, 0, 0);

    // Process
    process_menu_input(main_menu, &lib_config, main_menu_window);

    // Cleanup
    config_destroy(&lib_config);
    unpost_menu(main_menu);
    delete_main_menu(main_menu, items, config_items);
    delwin(main_menu_window);
    endwin();
    exit_curses(0);
}
