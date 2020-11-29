#include "ncurses_menu.h"
#include <menu.h>
#include <curses.h>
#include <libconfig.h>
#include <malloc.h>
#include "ncurses_form.h"
#include "ncurses_shared.h"

void set_keyboard_menu(){
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void create_main_menu(MENU **menu, config_t *lib_config) {
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
    config_lookup_string(lib_config, "root_dir", &root_dir);
    config_lookup_string(lib_config, "index_page", &index_page);
    config_lookup_string(lib_config, "not_found_page", &not_found_page);

    // Conversion from int to char*
    char *port_s = NULL;
    if (port_lookup_status != CONFIG_FALSE) {
        int length = snprintf(NULL, 0, "%d", port);
        port_s = calloc(length + 1, sizeof(char));
        sprintf(port_s, "%d", port);
    }


    config_entry_t *entry0 = malloc(sizeof(config_entry_t));
    config_entry_t *entry1 = malloc(sizeof(config_entry_t));
    config_entry_t *entry2 = malloc(sizeof(config_entry_t));
    config_entry_t *entry3 = malloc(sizeof(config_entry_t));
    config_entry_t *entry4 = malloc(sizeof(config_entry_t));

    entry0->type = CONFIG_TYPE_STRING;
    entry1->type = CONFIG_TYPE_INT;
    entry2->type = CONFIG_TYPE_STRING;
    entry3->type = CONFIG_TYPE_STRING;
    entry4->type = CONFIG_TYPE_STRING;

    entry0->field_type = TYPE_ENUM;
    entry1->field_type = TYPE_INTEGER;
    entry2->field_type = NULL;
    entry3->field_type = NULL;
    entry4->field_type = NULL;

    entry0->path = "mode";
    entry1->path = "port";
    entry2->path = "root_dir";
    entry3->path = "index_page";
    entry4->path = "not_found_page";

    ITEM **items = calloc(6, sizeof(ITEM *));
    items[0] = new_item("Mode:", mode);
    items[1] = new_item("Port:", port_s);
    items[2] = new_item("Root Dir:", root_dir);
    items[3] = new_item("Index Page:", index_page);
    items[4] = new_item("Not Found Page:", not_found_page);
    items[5] = NULL;

    set_item_userptr(items[0], entry0);
    set_item_userptr(items[1], entry1);
    set_item_userptr(items[2], entry2);
    set_item_userptr(items[3], entry3);
    set_item_userptr(items[4], entry4);

    *menu = new_menu(items);
}

void display_main_menu(MENU *menu, WINDOW *sub) {
    set_menu_mark(menu, " > ");
    set_menu_win(menu, stdscr);
    sub = derwin(stdscr, LINES - 9 - 1, COLS - 2, 9, 1);
    set_menu_sub(menu, sub);
    post_menu(menu);
    box(stdscr, 0, 0);
}

//void update_main_menu(MENU *menu) {
//
//}
//
//void delete_main_menu(MENU *menu) {
//
//}

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
                display_entry(current);
                set_keyboard_menu();
                unpost_menu(menu);

                create_main_menu(&menu, lib_config);
                display_main_menu(menu, window_body);
                break;
            }
            default:
                break;
        }
    }
}
