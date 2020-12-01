#ifndef NCURSES_FORM_H
#define NCURSES_FORM_H

#include <libconfig.h>
#include <menu.h>
#include <form.h>
#include <curses.h>
#include "ncurses_shared.h"
#include "ncurses_panel.h"
#include <stdlib.h>

/**
 * Sets keyboard to "form" mode.
 */
void set_keyboard_form();
void create_form(const ITEM *item, FIELD **field, FORM **form, WINDOW **header, WINDOW **sub);
void display_form(FORM *form, WINDOW *header, WINDOW *sub, config_item_t *config);
void update_config_item(MENU *menu, ITEM *item, char *value, config_t *lib_config);
void process_form_input(FORM *form, FIELD *field, ITEM *item, MENU *menu, config_t *lib_config);
void display_item_form(MENU *menu, ITEM *item, config_t *lib_config);

#endif //NCURSES_FORM_H
