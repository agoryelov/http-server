
#include <curses.h>
#include <menu.h>
#include <libconfig.h>
#include <stdlib.h>

#define CONFIG_PATH "../config.cfg"

static int row = 0;

void print_ascii_art_title();
void print_instructions();
void setup();
void init_menu(ITEM **items, MENU **menu, config_t *lib_config);
void set_menu_properties(MENU *menu, WINDOW *sub);
void process_user_input(MENU *menu);

int main() {
    setup();
    ITEM **items = NULL;
    MENU *menu = NULL;
    WINDOW *sub = NULL;
    config_t lib_config;

    print_ascii_art_title();
    print_instructions();

    init_menu(items, &menu, &lib_config);
    set_menu_properties(menu, sub);
    post_menu(menu);

    box(stdscr, 0, 0);

    process_user_input(menu);

    config_destroy(&lib_config);
    unpost_menu(menu);

    endwin();
}

void print_ascii_art_title() {
    int title_col_count = 44;
    if (COLS < title_col_count) {
        return; // return if not enough room to print
    }
    char *row0 = " __    __  _____  __________    __  ___  __";
    char *row1 = "/ _\\  /__\\/__   \\/__   \\_   \\/\\ \\ \\/ _ \\/ _\\\n";
    char *row2 = "\\ \\  /_\\    / /\\/  / /\\// /\\/  \\/ / /_\\/\\ \\\n";
    char *row3 = "_\\ \\//__   / /    / //\\/ /_/ /\\  / /_\\\\ _\\ \\\n";
    char *row4 = "\\__/\\__/   \\/     \\/ \\____/\\_\\ \\/\\____/ \\__/ \n";

    int start_col = (COLS - title_col_count) / 2;
    row++;
    mvprintw(row++, start_col, row0);
    mvprintw(row++, start_col, row1);
    mvprintw(row++, start_col, row2);
    mvprintw(row++, start_col, row3);
    mvprintw(row++, start_col, row4);
}

void print_instructions() {
    char *row0 = "[F1]         Exit Program";
    char *row1 = "[Up Arrow]   Scroll Up";
    char *row2 = "[Down Arrow] Scroll Down";
    char *row3 = "[Enter]      Select Setting";
    int start_col = 4;
    row++;
    mvprintw(row++, start_col, row0);
    mvprintw(row++, start_col, row1);
    mvprintw(row++, start_col, row2);
    mvprintw(row++, start_col, row3);
}

void setup() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
}

void init_menu(ITEM **items, MENU **menu, config_t *lib_config) {
    config_init(lib_config);
    if (!config_read_file(lib_config, CONFIG_PATH)) {
        printf("%s:%d - %s\n", config_error_file(lib_config), config_error_line(lib_config),
               config_error_text(lib_config));
        config_destroy(lib_config);
    }
    int port;
    const char *root_dir = NULL;
    const char *index_page = NULL;
    const char *not_found_page = NULL;
    const char *mode = NULL;

    int port_lookup_status = config_lookup_int(lib_config, "port", &port);
    config_lookup_string(lib_config, "mode", &mode);
    config_lookup_string(lib_config, "directories.root", &root_dir);
    config_lookup_string(lib_config, "pages.index", &index_page);
    config_lookup_string(lib_config, "pages.not_found", &not_found_page);

    // Conversion from int to char*
    char *port_s = NULL;
    if (port_lookup_status != CONFIG_FALSE) {
        int length = snprintf(NULL, 0, "%d", port);
        port_s = calloc(length + 1, sizeof(char));
        sprintf(port_s, "%d", port);
    }

    ITEM *item1 = new_item("Mode:", mode);
    ITEM *item2 = new_item("Port:", port_s);
    ITEM *item3 = new_item("Root Dir:", root_dir);
    ITEM *item4 = new_item("Index Page:", index_page);
    ITEM *item5 = new_item("Not Found Page:", not_found_page);
    items = calloc(6, sizeof(ITEM *));
    items[0] = item1;
    items[1] = item2;
    items[2] = item3;
    items[3] = item4;
    items[4] = item5;
    items[5] = NULL;

    *menu = new_menu(items);
}

void set_menu_properties(MENU *menu, WINDOW *sub) {
    set_menu_mark(menu, " > ");
    set_menu_win(menu, stdscr);
    row++;
    sub = derwin(stdscr, LINES - row, COLS - 2, row, 1);
    set_menu_sub(menu, sub);
}

void process_user_input(MENU *menu) {
    int c;
    while ((c = getch()) != KEY_F(1)) {
        switch (c) {
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_ENTER:
                // TODO: Handle selection
                break;
            default:
                break;
        }
    }
}
