#ifndef NCURSES_MENU_H
#define NCURSES_MENU_H
#include <menu.h>
#include <curses.h>
#include <libconfig.h>
#include "ncurses_shared.h"

#define NUM_ITEMS 5

/**
 * Sets ncurses for menu input.
 */
void set_keyboard_menu();

/**
 * Creates a config item struct which is stored at the index of the config_item struct array.
 * Uses the name, path, config_type, and field_type specified by the arguments.
 * @param config_items - the config_item struct array
 * @param index - the index the new struct will be stored at
 * @param name - the name of the config_item
 * @param path - the path of hte config_item
 * @param config_type - the config_type
 * @param field_type - the field_type
 */
void create_config_item(config_item_t **config_items, int index, char *name, char *path, int config_type, FIELDTYPE *field_type);

/**
 * Sets all the userptrs for the ncurses ITEM types to equal the config_items structs,
 * mapped according to their respective indices.
 * @param items
 * @param config_items
 */
void set_item_userptrs(ITEM **items, config_item_t **config_items);

/**
 * Creates the main menu to display to the user.
 * @param menu - the menu to display
 * @param lib_config - the config struct which will be written to in order to update the config file
 * @param config_items - the config item struct array
 */
void create_main_menu(MENU **menu, config_t *lib_config, config_item_t **config_items);

/**
 * Displays the main menu to the user
 * @param menu - the menu
 * @param sub - the main menu's sub window
 */
void display_main_menu(MENU *menu, WINDOW *sub);

/**
 * Updates the main menu's item's description with the specified value
 * @param menu - the menu with the item
 * @param item - the item to update
 * @param value - the value to change
 */
void update_main_menu(MENU *menu, ITEM *item, char *value);

/**
 * Deletes the main menu, cleaning up and memory.
 * @param menu - the memory to delete
 */
void delete_main_menu(MENU *menu);

/**
 * Processes the main menu's input in a loop, allowing users to
 * scroll up/down, select a setting to modify, and exit the program.
 * @param menu - the main menu
 * @param lib_config - the config struct which will be written to in order to update the config file
 * @param window - the window which contains the main menu
 */
void process_menu_input(MENU *menu, config_t *lib_config, WINDOW *window);
#endif //NCURSES_MENU_H
