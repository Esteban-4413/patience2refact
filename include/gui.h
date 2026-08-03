#ifndef GUI_H
#define GUI_H
#include "game_state.h"
#include <dirent.h>
#include <ncurses.h>
#include <string.h>
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

void draw_main_panel(WINDOW *win, int selected_menu, int active_option, int active_pane, int right_option,
					 char patience_files[][64], int num_files, int yMax, int xMax);
MenuBar *menu_bar_create(WINDOW *win, Menu *menus, int num_menus);
void menu_bar_draw(MenuBar *mb);
void menu_bar_destroy(MenuBar *mb);
MenuChoice start_menu(char *chosen_file_out);
void run_ncurses_gui(Game_state *state);
#endif
