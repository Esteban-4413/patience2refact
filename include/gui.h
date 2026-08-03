#ifndef GUI_H
#define GUI_H
#include "game_state.h"
#include <ncurses.h>
// #include <ncurses_dll.h>

typedef enum { MENU_NEW_GAME, MENU_CONTINUE, MENU_QUIT, MENU_UNKNOWN } MenuChoice;

typedef struct {
	int start_x;
	char trigger;
	char *text;
} Menu;

typedef struct {
	WINDOW *win;
	Menu *menus;
	int num_menus;
	int selected;
} MenuBar;

MenuBar *menu_bar_create(WINDOW *win, Menu *menus, int num_menus);
void menu_bar_draw(MenuBar *mb);
void menu_bar_destroy(MenuBar *mb);
MenuChoice start_menu();
void run_ncurses_gui(Game_state *state);
#endif
