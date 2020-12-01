#include "ncurses_menu.h"
#include <menu.h>
#include <curses.h>
#include <libconfig.h>
#include <malloc.h>
#include "ncurses_form.h"
#include "ncurses_shared.h"

#define NUM_ITEMS 5

void set_keyboard_menu(){
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

config_item_t *create_config_item(char *name, char *path, int config_type, FIELDTYPE *field_type) {
    config_item_t *config_item = calloc(1, sizeof(config_item_t));
    config_item->name = name;
    config_item->path = path;
    config_item->config_type = config_type;
    config_item->field_type = field_type;
    return config_item;
}

config_item_t **create_config_items() {
    config_item_t **config_items = calloc(NUM_ITEMS + 1, sizeof(config_item_t*));
    config_items[0] = create_config_item("Mode:", "mode", CONFIG_TYPE_STRING, TYPE_ENUM);
    config_items[1] = create_config_item("Port:", "port", CONFIG_TYPE_INT, TYPE_INTEGER);
    config_items[2] = create_config_item("Root Directory:", "root_dir", CONFIG_TYPE_STRING, NULL);
    config_items[3] = create_config_item("Index Page:", "index_page", CONFIG_TYPE_STRING, NULL);
    config_items[4] = create_config_item("Not Found Page:", "not_found_page", CONFIG_TYPE_STRING, NULL);
    config_items[5] = NULL;
    return config_items;
}

void set_menu_item_userptrs(ITEM **items, config_item_t **config_items) {
    size_t i;
    for (i = 0; config_items[i] != NULL; ++i) {
        set_item_userptr(items[i], config_items[i]);
    }
    set_item_userptr(items[i], config_items[i]);
}

void create_main_menu(MENU **menu, config_t *lib_config) {
    config_init(lib_config);
    if (!config_read_file(lib_config, CONFIG_PATH)) {
        printf("%s:%d - %s\n", config_error_file(lib_config), config_error_line(lib_config), config_error_text(lib_config));
        return;
    }
    int port;
    const char *root_dir = NULL;
    const char *index_page = NULL;
    const char *not_found_page = NULL;
    const char *mode = NULL;

    int port_lookup_status = config_lookup_int(lib_config, "port", &port);
    char *port_s = NULL;
    if (port_lookup_status != CONFIG_FALSE) {
        convert_int_to_string(port, &port_s);
    }
    config_lookup_string(lib_config, "mode", &mode);
    config_lookup_string(lib_config, "root_dir", &root_dir);
    config_lookup_string(lib_config, "index_page", &index_page);
    config_lookup_string(lib_config, "not_found_page", &not_found_page);

    config_item_t **config_items = create_config_items();

    ITEM **items = calloc(6, sizeof(ITEM *));
    items[0] = new_item(config_items[0]->name, mode);
    items[1] = new_item(config_items[1]->name, port_s);
    items[2] = new_item(config_items[2]->name, root_dir);
    items[3] = new_item(config_items[3]->name, index_page);
    items[4] = new_item(config_items[4]->name, not_found_page);
    items[5] = NULL;

    set_menu_item_userptrs(items, config_items);
    *menu = new_menu(items);
}

void display_main_menu(MENU *menu, WINDOW *sub) {
    set_keyboard_menu();
    set_menu_mark(menu, " > ");
    set_menu_win(menu, stdscr);
    sub = sub == NULL ? derwin(stdscr, LINES - 11, COLS - 8, 10, 4) : sub;
    set_menu_sub(menu, sub);
    post_menu(menu);
}

void update_main_menu(MENU *menu, ITEM *item, char *value) {
    ITEM **items = menu_items(menu);
    config_item_t **config_items = create_config_items();
    int index = item_index(item);
    items[index] = new_item(item_name(item), value);
    set_menu_item_userptrs(items, config_items);
    unpost_menu(menu);
    set_menu_items(menu, items);
}

void process_menu_input(MENU *menu, config_t *lib_config, WINDOW *window_body) {
    int c;
    while ((c = getch()) != KEY_F(1)) {
        switch (c) {
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case 10: {   // ENTER KEY
                ITEM *current = current_item(menu);
                display_item_form(menu, current, lib_config);
                display_main_menu(menu, window_body);
                break;
            }
            default:
                break;
        }
    }
    delwin(window_body);
}
