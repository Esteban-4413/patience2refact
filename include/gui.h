#ifndef GUI_H
#define GUI_H
#include "game_state.h"
#include <ncurses.h>
#include <ncurses_dll.h>

typedef enum { MENU_NEW_GAME, MENU_CONTINUE, MENU_QUIT, MENU_UNKNOWN } MenuChoice;

MenuChoice start_menu();
void run_ncurses_gui(Game_state *state);
#endif
