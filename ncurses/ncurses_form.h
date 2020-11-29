#ifndef NCURSES_FORM_H
#define NCURSES_FORM_H

#include <libconfig.h>
#include <menu.h>
#include <form.h>
#include <curses.h>
#include "ncurses_shared.h"
#include "ncurses_panel.h"
#include <stdlib.h>

void set_keyboard_form();
void create_form(const ITEM *item, FIELD **field, FORM **form, WINDOW **header, WINDOW **sub);
void display_form(FORM *form, WINDOW *header, WINDOW *sub, config_entry_t *config);
void update_config_entry(config_entry_t *config_entry, char *value);
void process_form_input(FORM *form, FIELD *field, ITEM *item);
void display_entry(ITEM *item);

#endif //NCURSES_FORM_H
