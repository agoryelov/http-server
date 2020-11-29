#include "ncurses_panel.h"

void display_existing_panel(PANEL *panel) {
    top_panel(panel);
    update_panels();
    doupdate();
}

PANEL *display_new_panel(WINDOW *window) {
    PANEL *panel = new_panel(window);
    display_existing_panel(panel);
    return panel;
}
