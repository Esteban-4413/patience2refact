#include "../include/gui.h"
#include <string.h>

int get_patience_file(char files[][64], int max_files) {
	DIR *d;
	struct dirent *dir;
	int count = 0;
	d = opendir("paciencias");
	if (d) {
		while ((dir = readdir(d)) != NULL && count < max_files) {
			if (strstr(dir->d_name, ".paciencia") != NULL) {
				strncpy(files[count], dir->d_name, 63);
				files[count][63] = '\0';
				count++;
			}
		}
		closedir(d);
	}
	return count;
}

void menu_bar_draw(MenuBar *mb) {
	for (int i = 0; i < mb->num_menus; i++) {
		if (i == mb->selected)
			wattron(mb->win, A_STANDOUT);

		mvwprintw(mb->win, 0, mb->menus[i].start_x, "%s", mb->menus[i].text);
		if (i == mb->selected)
			wattroff(mb->win, A_STANDOUT);
	}
	wrefresh(mb->win);
}

void draw_main_panel(WINDOW *win, int selected_menu, int active_option, int active_pane, int right_option,
					 char patience_files[][64], int num_files, int yMax, int xMax) {
	for (int y = 1; y < yMax / 2 - 1; y++) {
		for (int x = 1; x < xMax / 2 - 1; x++) {
			mvwprintw(win, y, x, " ");
		}
	}
	switch (selected_menu) {
	case 0:
		int split_x = (xMax / 2) / 3;
		mvwvline(win, 1, split_x, ACS_VLINE, yMax / 2 - 2);
		if (active_pane == 0 && active_option == 0)
			wattron(win, A_REVERSE);
		mvwprintw(win, 2, 2, "  New game  ");
		if (active_pane == 0 && active_option == 0)
			wattroff(win, A_REVERSE);
		if (active_pane == 0 && active_option == 1)
			wattron(win, A_REVERSE);
		mvwprintw(win, 3, 2, "  Continue  ");
		if (active_pane == 0 && active_option == 1)
			wattroff(win, A_REVERSE);
		if (active_option == 0) {
			mvwprintw(win, 1, split_x + 2, "Avaiable games:");
			for (int i = 0; i < num_files; i++) {
				if (active_pane == 1 && right_option == i)
					wattron(win, A_REVERSE);
				mvwprintw(win, 3 + i, split_x + 2, " 1.%s ", patience_files[i]);
				if (active_pane == 1 && right_option == i)
					wattroff(win, A_REVERSE);
			}
			if (num_files == 0)
				mvwprintw(win, 3, split_x + 2, "  (No files found)  ");
		} else if (active_option == 1) {
			mvwprintw(win, 1, split_x + 2, "  Saved games:  ");

			mvwprintw(win, 3, split_x + 2, "  (Not saved games for now)  ");
		}
		break;
	case 1:
		if (active_option == 0)
			wattron(win, A_REVERSE);
		mvwprintw(win, 2, 2, "  Option 1  ");
		if (active_option == 0)
			wattroff(win, A_REVERSE);
		if (active_option == 1)
			wattron(win, A_REVERSE);
		mvwprintw(win, 3, 2, "  Option 2  ");
		if (active_option == 1)
			wattroff(win, A_REVERSE);
		break;
	case 2:
		mvwprintw(win, 2, 2, "How to play");
		mvwprintw(win, 3, 2, "move cards using...");
		mvwprintw(win, 4, 2, "Press 'q' to exit.");
		break;
	default:
		mvwprintw(win, 3, 5, "Welcome! Select a tab above");
		break;
	}
}

MenuChoice start_menu(char *chosen_file_out) {
	MenuChoice choice;
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW *win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
	box(win, 0, 0);

	keypad(win, TRUE);

	Menu menus[] = {{2, 'h', "Patience menu"}, {18, 'l', "Options"}, {28, '?', "Help"}};
	int total_menus = sizeof(menus) / sizeof(menus[0]);
	MenuBar mb;
	mb.win = win;
	mb.menus = menus;
	mb.num_menus = total_menus;

	mb.selected = 0;
	int active_option = 0;
	int num_options_per_tab[] = {2, 2, 0};

	char patience_files[20][64];
	int num_files = get_patience_file(patience_files, 20);

	int active_pane = 0;
	int right_option = 0;

	menu_bar_draw(&mb);
	draw_main_panel(win, mb.selected, active_option, active_pane, right_option, patience_files, num_files, yMax, xMax);
	wrefresh(win);

	int ch;
	bool running = true;

	while (running && (ch = wgetch(win))) {
		switch (ch) {
		case 'l':
		case KEY_RIGHT:
			if (mb.selected == 0 && active_pane == 0 && num_files > 0)
				active_pane = 1;
			else {
				mb.selected = (mb.selected + 1) % mb.num_menus;
				active_pane = 0;
				active_option = 0;
			}
			break;
		case 'j':
		case KEY_DOWN:
			if (active_pane == 0) {
				if (num_options_per_tab[mb.selected] > 0)
					active_option = (active_option + 1) % num_options_per_tab[mb.selected];
			} else if (active_pane == 1) {
				if (num_files > 0)
					right_option = (right_option + 1) % num_files;
			}
			break;
		case 'h':
		case KEY_LEFT:
			if (mb.selected == 0 && active_pane == 0 && num_files > 0)
				active_pane = 0;
			else {
				mb.selected = (mb.selected - 1 + mb.num_menus) % mb.num_menus;
				active_pane = 0;
				active_option = 0;
			}
			break;
		case 'k':
		case KEY_UP:
			if (active_pane == 0) {
				if (num_options_per_tab[mb.selected] > 0)
					active_option =
						(active_option - 1 + num_options_per_tab[mb.selected]) % num_options_per_tab[mb.selected];
			} else if (active_pane == 1) {
				if (num_files > 0)
					right_option = (right_option - 1 + num_files) % num_files;
			}
			break;

		case '\n':
			if (mb.selected == 0) {
				if (active_pane == 0) {
					if (active_option == 0 && num_files > 0)
						active_pane = 1;
				} else if (active_pane == 1) {
					choice = MENU_NEW_GAME;
					strcpy(chosen_file_out, patience_files[right_option]);
					running = false;
				}
			} else if (mb.selected == 1)
				; // TODO
			break;
		case 'q':
			choice = MENU_QUIT;
			running = false;
			break;
		}

		menu_bar_draw(&mb);
		draw_main_panel(win, mb.selected, active_option, active_pane, right_option, patience_files, num_files, yMax,
						xMax);
		wrefresh(win);
	}
	delwin(win);
	endwin();
	return choice;
}

void run_ncurses_gui(Game_state *state) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_WHITE, COLOR_BLACK);
	}
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW *game_win = newwin(yMax, xMax, 0, 0);
	keypad(game_win, TRUE);
	bool playing = true;
	int ch;
	while (playing) {
		werase(game_win);
		box(game_win, 0, 0);
		mvwprintw(game_win, 0, 2, "Playing: %s", state->definition->game_name);
		// TODO
		mvwprintw(game_win, yMax - 1, 2, "Use the keys to move and press q to quit");

		wrefresh(game_win);
		ch = wgetch(game_win);
		switch (ch) {
		case 'q':
			playing = false;
			break;
		}
	}
	delwin(game_win);
	endwin();
}
