#include "../include/gui.h"
#include <string.h>

int get_patience_file(char files[][64], int max_files, char *folder_name, char *extension) {
	DIR *d;
	struct dirent *dir;
	int count = 0;
	d = opendir(folder_name);
	if (d) {
		while ((dir = readdir(d)) != NULL && count < max_files) {
			if (strstr(dir->d_name, extension) != NULL) {
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
					 char patience_files[][64], int num_files, int yMax, int xMax, char saved_files[][64],
					 int num_saved_files) {
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
				mvwprintw(win, 3 + i, split_x + 2, " %d.%s ", i + 1, patience_files[i]);
				if (active_pane == 1 && right_option == i)
					wattroff(win, A_REVERSE);
			}
			if (num_files == 0)
				mvwprintw(win, 3, split_x + 2, "  (No files found)  ");
		} else if (active_option == 1) {
			mvwprintw(win, 1, split_x + 2, "  Saved games:  ");
			if (num_saved_files == 0)
				mvwprintw(win, 3, split_x + 2, "  (No saved games)  ");
			else {
				for (int i = 0; i < num_saved_files; i++) {
					if (active_pane == 1 && right_option == i)
						wattron(win, A_REVERSE);
					mvwprintw(win, 3 + i, split_x + 2, " %d.%s ", i + 1, saved_files[i]);
					if (active_pane == 1 && right_option == i)
						wattroff(win, A_REVERSE);
				}
			}
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
	int num_files = get_patience_file(patience_files, 20, "paciencias", ".paciencia");

	char saved_files[20][64];
	int num_saved_files = get_patience_file(saved_files, 20, "saves", ".txt");

	int active_pane = 0;
	int right_option = 0;

	menu_bar_draw(&mb);
	draw_main_panel(win, mb.selected, active_option, active_pane, right_option, patience_files, num_files, yMax, xMax,
					saved_files, num_saved_files);
	wrefresh(win);

	int ch;
	bool running = true;

	while (running && (ch = wgetch(win))) {
		int current_limit = (active_option == 0) ? num_files : num_saved_files;
		switch (ch) {
		case 'l':
		case KEY_RIGHT:
			if (mb.selected == 0 && active_pane == 0 && current_limit > 0)
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
				right_option = 0;
			} else if (active_pane == 1) {
				if (current_limit > 0)
					right_option = (right_option + 1) % current_limit;
			}
			break;
		case 'h':
		case KEY_LEFT:
			if (active_pane == 1)
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
				right_option = 0;
			} else if (active_pane == 1) {
				if (num_files > 0)
					right_option = (right_option - 1 + current_limit) % current_limit;
			}
			break;

		case '\n':
			if (mb.selected == 0) {
				if (active_pane == 0) {
					if (active_option == 0 && num_files > 0)
						active_pane = 1;
				} else if (active_pane == 1) {
					choice = (active_option == 0) ? MENU_NEW_GAME : MENU_CONTINUE;
					if (active_option == 0)
						strcpy(chosen_file_out, patience_files[right_option]);
					else
						strcpy(chosen_file_out, saved_files[right_option]);
					running = false;
				}
			}
			break;
		case 'q':
			choice = MENU_QUIT;
			running = false;
			break;
		}

		menu_bar_draw(&mb);
		draw_main_panel(win, mb.selected, active_option, active_pane, right_option, patience_files, num_files, yMax,
						xMax, saved_files, num_saved_files);
		wrefresh(win);
	}
	delwin(win);
	endwin();
	return choice;
}

const char *get_unicode_suit(Suit suit) {
	switch (suit) {
	case SUIT_HEART:
		return "\u2665";
	case SUIT_SPADE:
		return "\u2660";
	case SUIT_CLUB:
		return "\u2663";
	case SUIT_DIAMOND:
		return "\u2666";
	default:
		return "?";
	}
}

int get_suit_color(Suit suit) {
	if (suit == SUIT_HEART || suit == SUIT_DIAMOND)
		return 1;
	return 0;
}

void draw_card(WINDOW *win, int y, int x, Card *c, bool is_hidden) {
	if (c == NULL)
		return;
	if (is_hidden)
		mvwprintw(win, y, x, "[ ** ]");
	else {
		int color_pair = get_suit_color(c->suit);
		if (color_pair == 1)
			wattron(win, COLOR_PAIR(color_pair));
		char rank_str[4];
		translate_rank(rank_str, c->rank);
		mvwprintw(win, y, x, "[%2s%s]", rank_str, get_unicode_suit(c->suit));
		if (color_pair == 1)
			wattroff(win, COLOR_PAIR(color_pair));
	}
}

void draw_pile(WINDOW *win, Pile *p, int start_y, int start_x, bool cascade_down) {
	if (p == NULL || p->num_cards == 0) {
		mvwprintw(win, start_y, start_x, "[ -- ]");
		return;
	}
	Card *current = p->head;
	int current_y = start_y;
	int index = 0;
	while (current != NULL) {
		bool hidden = false;
		if (p->pile_class != NULL && p->pile_class->visible_top_only)
			hidden = (current->next != NULL);
		draw_card(win, current_y, start_x, current, hidden);
		if (cascade_down)
			current_y++;
		current = current->next;
		index++;
	}
}

void draw_game_board(WINDOW *win, Game_state *state) {
	int spacing_x = 11;
	int top_x = 2;
	int bottom_x = 2;
	for (int i = 0; i < state->pile_count; i++) {
		Pile *p = state->table_piles[i];
		if (p == NULL)
			continue;
		int current_x, start_y;
		bool cascade = true;
		if (strstr(p->pile_class->name, "STOCK") != NULL || strstr(p->pile_class->name, "DESCARTE") != NULL ||
			strstr(p->pile_class->name, "FUND") != NULL) {
			current_x = top_x;
			start_y = 3;
			cascade = false;
			top_x += spacing_x;
			if (strstr(p->pile_class->name, "DESCARTE") != NULL)
				top_x += spacing_x;
		} else {
			current_x = bottom_x;
			start_y = 7;
			cascade = true;
			bottom_x += spacing_x;
		}
		mvwprintw(win, start_y - 2, current_x, "%.8s", p->pile_class->name);
		mvwprintw(win, start_y - 1, current_x, "%d(%d)", i + 1, p->num_cards);
		draw_pile(win, p, start_y, current_x, cascade);
	}
}

void run_ncurses_gui(Game_state *state) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (has_colors()) {
		start_color();
		use_default_colors();
		init_pair(1, COLOR_RED, -1);
		init_pair(2, COLOR_YELLOW, -1);
	}
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW *game_win = newwin(yMax, xMax, 0, 0);
	keypad(game_win, TRUE);
	bool playing = true;
	char input_str[80];
	char feedback_msg[128] = "Game started! Type 'hint' if you feel lost.";
	int ch;
	while (playing) {
		werase(game_win);
		box(game_win, 0, 0);
		mvwprintw(game_win, 0, 2, "Playing: %s", state->definition->game_name);
		draw_game_board(game_win, state);

		wattron(game_win, COLOR_PAIR(2));
		mvwprintw(game_win, yMax - 3, 2, "%s", feedback_msg);
		wattroff(game_win, COLOR_PAIR(2));

		mvwprintw(game_win, yMax - 2, 2, "Commands: mv <src> <dest>, undo, save, hint, quit");
		mvwprintw(game_win, yMax - 1, 2, "Command > ");

		wrefresh(game_win);

		echo();
		curs_set(1);
		wmove(game_win, yMax - 1, 12);
		wgetnstr(game_win, input_str, 79);
		noecho();
		curs_set(0);

		GameCommand cmd = parse_command(input_str);
		playing = execute_command(state, cmd, feedback_msg);
	}
	delwin(game_win);
	endwin();
}
