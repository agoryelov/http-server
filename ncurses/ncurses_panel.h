#ifndef NCURSES_PANEL_H
#define NCURSES_PANEL_H
#include <panel.h>
#include <curses.h>
#include "ncurses_shared.h"

void display_existing_panel(PANEL *panel);

PANEL *display_new_panel(WINDOW *window);

#endif //NCURSES_PANEL_H
