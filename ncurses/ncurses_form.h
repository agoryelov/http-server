#ifndef NCURSES_FORM_H
#define NCURSES_FORM_H

#include <libconfig.h>
#include <menu.h>
#include <form.h>
#include <curses.h>
#include "ncurses_shared.h"
#include <stdlib.h>

/**
 * Sets keyboard for form input.
 */
void set_keyboard_form();

/**
 * Creates the form, initializing its window, sub window, and field
 * for that item.
 * @param item - the item of that form
 * @param field - the field to initialize
 * @param form - the form to be created
 * @param header - the main window of the form
 * @param sub - the sub window of the form
 */
void create_form(const ITEM *item, FIELD **field, FORM **form, WINDOW **header, WINDOW **sub);

/**
 * Displays the form to the user.
 * @param form - the form to show
 * @param header - the main window of the form
 * @param sub - the sub window of the form
 * @param config_item - the config item struct which has the name of the item to display
 */
void display_form(FORM *form, WINDOW *header, WINDOW *sub, config_item_t *config_item);

/**
 * Saves the char array value to the config struct.
 * Updates the item in that menu with that value.
 * @param menu - the menu to update
 * @param item - the item's value to update
 * @param value - the char array value
 * @param lib_config - the config struct to write to in order to update the config file
 */
void save_to_lib_config(MENU *menu, ITEM *item, char *value, config_t *lib_config);

/**
 * Processes the form input in a loop, allowing users to scroll left/right,
 * use backspace, save, and return.
 * @param form - the form
 * @param field - the field to process
 * @param item - the item to save
 * @param menu - the menu to update after entering input
 * @param lib_config - the config struct to write to in order to update the config file
 */
void process_form_input(FORM *form, FIELD *field, ITEM *item, MENU *menu, config_t *lib_config);

/**
 * Entry point for the form.
 * Initializes the form with the field for the specified item.
 * Displays the form to the user and accepts inputs.
 * @param menu - the menu to update after entering input
 * @param item - the current item
 * @param lib_config - the config struct to write to in order to update the config file
 */
void init_item_form(MENU *menu, ITEM *item, config_t *lib_config);

#endif //NCURSES_FORM_H
