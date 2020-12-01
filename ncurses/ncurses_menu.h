#ifndef NCURSES_MENU_H
#define NCURSES_MENU_H
#include <menu.h>
#include <curses.h>
#include <libconfig.h>
#include "ncurses_panel.h"
#include "ncurses_shared.h"

/**
 * Sets ncurses to "keyboard" mode.
 */
void set_keyboard_menu();
void create_main_menu(MENU **menu, config_t *lib_config);
void display_main_menu(MENU *menu, WINDOW *sub);
void process_menu_input(MENU *menu, config_t *lib_config, WINDOW *window_body);
config_item_t **create_config_items();
void set_menu_item_userptrs(ITEM **items, config_item_t **config_items);
void update_main_menu(MENU *menu, ITEM *item, char *value);
#endif //NCURSES_MENU_H
