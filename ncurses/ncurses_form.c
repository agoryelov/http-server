#include "ncurses_form.h"
#include <libconfig.h>
#include <menu.h>
#include <form.h>
#include <curses.h>
#include "ncurses_shared.h"
#include "ncurses_panel.h"
#include <stdlib.h>

void set_keyboard_form() {
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
}

void create_form(const ITEM *item, FIELD **field, FORM **form, WINDOW **header, WINDOW **sub) {
    int height = 8;
    int width = 40;
    int start_y = 10;
    int start_x = (COLS - width) / 2;
    *header = derwin(stdscr, height, width, start_y, start_x);
    *sub = derwin(*header, height, width, 0, 0);
    field[0] = new_field(1, width - 6, height / 2, 3, 0, 0);
    field[1] = NULL;

    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_STATIC);
    if (((config_entry_t*)item_userptr(item))->field_type == TYPE_ENUM) {
        char *list[3] = {"Processes", "Threads", NULL};
        set_field_type(field[0], TYPE_ENUM, list, 0, 1);
    }
    if (((config_entry_t*)item_userptr(item))->field_type == TYPE_INTEGER) {
        set_field_type(field[0], TYPE_INTEGER, 0, 0, 65535);
    }

    *form = new_form(field);
}

void display_form(FORM *form, WINDOW *header, WINDOW *sub, config_entry_t *config) {
    set_form_win(form, header);
    set_form_sub(form, sub);
    post_form(form);
    mvwprintw_center_justify(header, 2, config->path);
    box(header, 0, 0);
}

void update_config_entry(config_entry_t *config_entry, char *value) {
    config_t lib_config;
    config_init(&lib_config);

    if (!config_read_file(&lib_config, CONFIG_PATH)) {
        printf("%s:%d - %s\n", config_error_file(&lib_config), config_error_line(&lib_config),
               config_error_text(&lib_config));
        config_destroy(&lib_config);
        return;
    }

    config_setting_t *root = config_root_setting(&lib_config);
    config_setting_t *setting = config_setting_get_member(root, config_entry->path);
    if (!setting) {
        setting = config_setting_add(root, config_entry->path, config_entry->type);
    }

    trim_trailing_whitespace(value);
    switch(config_entry->type) {
        case CONFIG_TYPE_STRING:
            config_setting_set_string(setting, value);
            break;
        case CONFIG_TYPE_INT: {
            int val = (int) strtoul(value, NULL, 0);
            if (val < 0) return;
            config_setting_set_int(setting, val);
            break;
        }
        default:
            // Unsupported
            break;
    }

    if (!config_write_file(&lib_config, CONFIG_PATH)) {
        printf("%s\n", "Could not write to file.");
    }

    config_destroy(&lib_config);
}

void process_form_input(FORM *form, FIELD *field, ITEM *item) {
    int c;
    while((c = getch()) != KEY_F(1)) {
        switch(c) {
            case KEY_LEFT:
                form_driver(form, REQ_LEFT_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_RIGHT_CHAR);
                break;
            case 127:   // BACKSPACE
                form_driver(form, REQ_DEL_PREV);
                break;
            case 10:    // ENTER KEY
                if (form_driver(form, REQ_VALIDATION) != E_INVALID_FIELD) {
                    update_config_entry(item_userptr(item), field_buffer(field, 0));
                    return;
                }
                break;
            default:
                form_driver(form, c);
                break;
        }
    }
}

void display_entry(ITEM *item)
{
    // Declaration
    FIELD *field[2];
    FORM *form;
    WINDOW *header, *sub;

    // Setup
    set_keyboard_form();
    create_form(item, field, &form, &header, &sub);
    display_form(form, header, sub, item_userptr(item));

    // Loop
    process_form_input(form, field[0], item);

    // Cleanup
    unpost_form(form);
    free_field(field[0]);
    free_field(field[1]);
    free_form(form);
}
