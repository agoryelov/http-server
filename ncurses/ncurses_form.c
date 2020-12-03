#include "ncurses_form.h"
#include <libconfig.h>
#include <menu.h>
#include <form.h>
#include <curses.h>
#include "ncurses_shared.h"
#include "ncurses_menu.h"
#include <stdlib.h>

void set_keyboard_form() {
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
}

void create_form(const ITEM *item, FIELD **field, FORM **form, WINDOW **header, WINDOW **sub) {
    int height = 12;
    int width = 40;
    int start_y = INSTRUCTIONS_HEIGHT + ASCII_TITLE_HEIGHT + MARGIN * 2;
    int start_x = (COLS - width) / 2;
    *header = derwin(stdscr, height, width, start_y, start_x);
    *sub = derwin(*header, height, width, 0, 0);
    field[0] = new_field(1, width - 12, 6, 6, 0, 0);
    field[1] = NULL;
    wclear(*header);
    wrefresh(*header);
    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_STATIC);
    if (((config_item_t*)item_userptr(item))->field_type == TYPE_ENUM) {
        char *list[3] = {"Processes", "Threads", NULL};
        set_field_type(field[0], TYPE_ENUM, list, 0, 1);
    }
    else if (((config_item_t*)item_userptr(item))->field_type == TYPE_INTEGER) {
        set_field_type(field[0], TYPE_INTEGER, 0, 0, MAX_PORT);
    }

    *form = new_form(field);
}

void display_form(FORM *form, WINDOW *header, WINDOW *sub, config_item_t *config_item) {
    set_keyboard_form();
    set_form_win(form, header);
    set_form_sub(form, sub);
    post_form(form);
    mvwprintw_center_justify(header, 2, config_item->name);
    mvwprintw_center_justify(header, getmaxy(header) - 3, "[F1] Return     [Enter] Save");
    box(header, 0, 0);
}

void save_to_lib_config(MENU *menu, ITEM *item, char *value, config_t *lib_config) {
    config_item_t *config_item = item_userptr(item);
    config_setting_t *root = config_root_setting(lib_config);
    config_setting_t *setting = config_setting_get_member(root, config_item->path);
    if (setting && config_setting_type(setting) != config_item->config_type) {
        config_setting_remove(root, config_item->path);
        setting = NULL;
    }
    if (!setting) {
        setting = config_setting_add(root, config_item->path, config_item->config_type);
    }

    trim_trailing_whitespace(value);
    if (value[0] == '\0') value = EMPTY_DESCRIPTION;
    switch(config_item->config_type) {
        case CONFIG_TYPE_STRING:
            config_setting_set_string(setting, value);
            update_main_menu(menu, item, value);
            break;
        case CONFIG_TYPE_INT: {
            int val = (int) strtoul(value, NULL, 0);
            if (val < 0) return;
            config_setting_set_int(setting, val);
            update_main_menu(menu, item, val == 0 ? "0" : value);
            break;
        }
        default:
            // Unsupported
            break;
    }

    if (!config_write_file(lib_config, CONFIG_PATH)) {
        printf("%s\n", "Could not write to file.");
    }
}

void process_form_input(FORM *form, FIELD *field, ITEM *item, MENU *menu, config_t *lib_config) {
    int c;
    while((c = getch()) != KEY_F(1)) {
        switch(c) {
            case KEY_LEFT:
                form_driver(form, REQ_LEFT_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_RIGHT_CHAR);
                break;
            case 127:   // Terminals may have different backspace keys
            case KEY_BACKSPACE:
            case 330:
                form_driver(form, REQ_DEL_PREV);
                break;
            case 10:    // ENTER KEY
                if (form_driver(form, REQ_VALIDATION) != E_INVALID_FIELD) {
                    save_to_lib_config(menu, item, field_buffer(field, 0), lib_config);
                    return;
                }
                break;
            default:
                form_driver(form, c);
                break;
        }
    }
}

void init_item_form(MENU *menu, ITEM *item, config_t *lib_config)
{
    // Declaration
    FIELD *field[2];
    FORM *form;
    WINDOW *header, *sub;

    // Setup
    create_form(item, field, &form, &header, &sub);
    display_form(form, header, sub, item_userptr(item));

    // Process
    process_form_input(form, field[0], item, menu, lib_config);

    // Cleanup
    unpost_form(form);
    free_form(form);
    free_field(field[0]);
    free_field(field[1]);
    wclear(header);
    wrefresh(header);
    delwin(sub);
    delwin(header);
}
