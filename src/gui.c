#include "../include/gui.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>

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
			mvwprintw(win, 1, split_x + 2, "Available games:");
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
		wattron(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, 2, 4, "=== How to move ===");
		wattroff(win, A_BOLD | COLOR_PAIR(3));

		mvwprintw(win, 4, 4, "mv 1 2   : Move TOP card from pile 1 to pile 2");
		mvwprintw(win, 5, 4, "mv 1|3 2 : Move SEQUENCE starting at index 3 from pile 1 to 2");

		wattron(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, 8, 4, "=== Rules glossary ===");
		wattroff(win, A_BOLD | COLOR_PAIR(3));

		wattron(win, A_UNDERLINE);
		mvwprintw(win, 10, 4, "Moving to a Destination Pile:");
		wattroff(win, A_UNDERLINE);
		mvwprintw(win, 11, 4, "-1 Rnk.    : Source card must be 1 rank LOWER (e.g. 7 onto 8)");
		mvwprintw(win, 12, 4, "+1 Rnk.    : Source card must be 1 rank HIGHER (e.g. 3 onto 2)");
		mvwprintw(win, 13, 4, "Adj.       : Source card must be EXACTLY 1 rank lower or higher");
		mvwprintw(win, 14, 4, "Alt color. : Source card must be opposite color of Destination");
		mvwprintw(win, 15, 4, "Same suit. : Source card must be same suit as Destination");
		mvwprintw(win, 16, 4, "To empty.  : Destination must be an empty space");
		mvwprintw(win, 17, 4, "Kings to empty : Only Kings can be moved to an empty space");

		wattron(win, A_UNDERLINE);
		mvwprintw(win, 19, 4, "Moving a Sequence of Cards:");
		wattroff(win, A_UNDERLINE);
		mvwprintw(win, 20, 4, "If the game allows it, the sequence you select (e.g. mv 1|3 2)");
		mvwprintw(win, 21, 4, "must be properly sorted BEFORE moving (e.g., same suit, -1 rnk).");
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

	if (has_colors()) {
		start_color();
		use_default_colors();
		init_pair(3, COLOR_CYAN, -1);
		init_pair(2, COLOR_YELLOW, -1);
	}

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	clear();

	const char *ascii_title[] = {".--------------------------------------------------------------.",
								 "|                                                              |",
								 "|  ____       _   _                     _       _       _      |",
								 "| |  _ \\ __ _| |_(_) ___ _ __   ___ ___( )___  | | __ _| |__   |",
								 "| | |_) / _` | __| |/ _ \\ '_ \\ / __/ _ \\// __| | |/ _` | '_ \\  |",
								 "| |  __/ (_| | |_| |  __/ | | | (_|  __/ \\__ \\ | | (_| | |_) | |",
								 "| |_|   \\__,_|\\__|_|\\___|_| |_|\\___\\___| |___/ |_|\\__,_|_.__/  |",
								 "|                                                              |",
								 "'--------------------------------------------------------------'"};

	int title_height = 9;
	int art_width = 64;

	int start_y = 1;
	int start_x = (xMax - art_width) / 2;
	if (start_x < 0)
		start_x = 0;

	attron(COLOR_PAIR(3) | A_BOLD);
	for (int i = 0; i < title_height; i++)
		mvprintw(start_y + i, start_x, "%s", ascii_title[i]);
	attroff(COLOR_PAIR(3) | A_BOLD);
	refresh();

	int win_y = start_y + title_height + 1;
	int win_h = yMax - win_y - 2;
	if (win_h < 10)
		win_h = 10;

	WINDOW *win = newwin(win_h, xMax / 2, win_y, xMax / 4);
	box(win, 0, 0);

	keypad(win, TRUE);

	Menu menus[] = {{2, 'h', "Patience menu"}, {28, '?', "Help"}};
	int total_menus = sizeof(menus) / sizeof(menus[0]);
	MenuBar mb;
	mb.win = win;
	mb.menus = menus;
	mb.num_menus = total_menus;

	mb.selected = 0;
	int active_option = 0;
	int num_options_per_tab[] = {2, 0};

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

void draw_empty_pile(WINDOW *win, int y, int x, bool is_higlighted) {
	if (is_higlighted)
		wattron(win, A_REVERSE | COLOR_PAIR(2));
	mvwprintw(win, y, x, "┌────┐");
	mvwprintw(win, y + 1, x, "│ ── │");
	mvwprintw(win, y + 2, x, "└────┘");
	if (is_higlighted)
		wattroff(win, A_REVERSE | COLOR_PAIR(2));
}

void draw_card(WINDOW *win, int y, int x, Card *c, bool is_hidden, bool is_highlighted) {
	if (c == NULL)
		return;
	int color_pair = is_highlighted ? 2 : get_suit_color(c->suit);
	if (is_highlighted)
		wattron(win, A_REVERSE | COLOR_PAIR(color_pair));
	else if (color_pair == 1)
		wattron(win, COLOR_PAIR(color_pair));
	if (is_hidden) {
		mvwprintw(win, y, x, "┌────┐");
		mvwprintw(win, y + 1, x, "│░░░░│");
		mvwprintw(win, y + 2, x, "└────┘");
	} else {
		int color_pair = get_suit_color(c->suit);
		if (color_pair == 1)
			wattron(win, COLOR_PAIR(color_pair));
		char rank_str[4];
		translate_rank(rank_str, c->rank);
		mvwprintw(win, y, x, "┌────┐");
		mvwprintw(win, y + 1, x, "│%2s%s │", rank_str, get_unicode_suit(c->suit));
		mvwprintw(win, y + 2, x, "└────┘");
		if (color_pair == 1)
			wattroff(win, COLOR_PAIR(1));
	}
	if (is_highlighted)
		wattroff(win, A_REVERSE | COLOR_PAIR(color_pair));
	else if (color_pair == 1)
		wattroff(win, COLOR_PAIR(color_pair));
}

int draw_cascade_recursive(WINDOW *win, Card *c, int start_y, int start_x, PileClass *pclass, bool is_head,
						   int current_index, int total_cards, bool is_src_hint, bool is_dest_hint, int hint_count) {
	if (c == NULL)
		return start_y;
	int current_y = draw_cascade_recursive(win, c->next, start_y, start_x, pclass, false, current_index - 1,
										   total_cards, is_src_hint, is_dest_hint, hint_count);
	bool is_hidden = false;
	if (pclass != NULL && pclass->visible_top_only)
		is_hidden = !is_head;
	bool highlight_this = false;
	if (is_dest_hint)
		highlight_this = true;
	else if (is_src_hint && current_index > (total_cards - hint_count))
		highlight_this = true;
	draw_card(win, current_y, start_x, c, is_hidden, highlight_this);
	if (!is_hidden) {
		wattron(win, COLOR_PAIR(2));
		mvwprintw(win, current_y + 1, start_x - 3, "%2d|", current_index);
		wattroff(win, COLOR_PAIR(2));
	}
	return current_y + 2;
}

void draw_pile(WINDOW *win, Pile *p, int start_y, int start_x, bool cascade_down, bool is_src_hint, bool is_dest_hint,
			   int hint_count) {
	if (p == NULL || p->num_cards == 0) {
		draw_empty_pile(win, start_y, start_x, is_dest_hint || is_src_hint);
		return;
	}
	Card *current = p->head;
	int current_y = start_y;
	int index = 0;
	if (!cascade_down) {
		Card *top_card = p->head;
		bool hidden = false;
		if (p->pile_class != NULL && strstr(p->pile_class->name, "STOCK") != NULL)
			hidden = true;
		else if (p->pile_class != NULL && p->pile_class->visible_top_only)
			hidden = false;
		draw_card(win, start_y, start_x, p->head, hidden, is_src_hint || is_dest_hint);
	} else
		draw_cascade_recursive(win, p->head, start_y, start_x, p->pile_class, true, p->num_cards, p->num_cards,
							   is_src_hint, is_dest_hint, hint_count);
}

void flags_interpreter(uint32_t flags, char *out_desc) {
	out_desc[0] = '\0';

	if (flags == F_NONE) {
		strcpy(out_desc, "Any valid card");
		return;
	}

	if ((flags & F_EMPTY_DEST) && ((flags & F_TOP_KING) || (flags & F_BOTTOM_KING))) {
		strcpy(out_desc, "Kings to empty");
		return;
	}

	if (flags & F_VAL_ADJACENT)
		strcat(out_desc, "Adj. ");
	else if (flags & F_VAL_LOWER)
		strcat(out_desc, "-1 Rnk. ");
	else if (flags & F_VAL_HIGHER)
		strcat(out_desc, "+1 Rnk. ");

	if ((flags & F_COLOR_ALT_SEQ) || (flags & F_COLOR_DIFF_DST))
		strcat(out_desc, "Alt color. ");
	if ((flags & F_SUIT_SAME_SEQ) || (flags & F_SUIT_SAME_DST))
		strcat(out_desc, "Same suit. ");

	if (flags & F_EMPTY_DEST)
		strcat(out_desc, "To empty.");
}
void draw_game_board(WINDOW *win, Game_state *state) {
	int yMax, xMax;
	getmaxyx(win, yMax, xMax);

	int sidebar_width = 35;
	int sidebar_x = xMax - sidebar_width;

	bool show_sidebar = (xMax >= 105);
	if (!show_sidebar)
		sidebar_x = xMax;

	int available_x = sidebar_x - 6;


	int spacing_x = available_x / state->pile_count;
	if (spacing_x > 10)
		spacing_x = 10;
	if (spacing_x < 7)
		spacing_x = 7;

	int top_x = 4;
	int top_y_name = 1, top_y_count = 2, top_y_cards = 3;

	int bottom_x = 4;
	int bottom_y_name = 7, bottom_y_count = 8, bottom_y_cards = 9;

	int h_src = (state->stats != NULL) ? state->stats->hint_src_pile : -1;
	int h_dest = (state->stats != NULL) ? state->stats->hint_dest_pile : -1;
	int h_count = (state->stats != NULL) ? state->stats->hint_card_count : 0;


	for (int i = 0; i < state->pile_count; i++) {
		Pile *p = state->table_piles[i];
		if (!p || !p->pile_class)
			continue;

		bool is_higlighted_src = (i == h_src);
		bool is_highlighted_dest = (i == h_dest);

		if (p->pile_class->visible_all) {
			mvwprintw(win, bottom_y_name, bottom_x, "%.8s", p->pile_class->name);
			mvwprintw(win, bottom_y_count, bottom_x, "%d(%d)", i + 1, p->num_cards);

			draw_pile(win, p, bottom_y_cards, bottom_x, true, is_higlighted_src, is_highlighted_dest, h_count);

			bottom_x += spacing_x;
		} else {
			mvwprintw(win, top_y_name, top_x, "%.8s", p->pile_class->name);
			mvwprintw(win, top_y_count, top_x, "%d(%d)", i + 1, p->num_cards);

			draw_pile(win, p, top_y_cards, top_x, false, is_higlighted_src, is_highlighted_dest, h_count);

			if (p->pile_class->visible_none)
				top_x += spacing_x + 3;
			else
				top_x += spacing_x;
		}
	}

	if (show_sidebar) {
		mvwvline(win, 1, sidebar_x, ACS_VLINE, yMax - 5);

		wattron(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, 2, sidebar_x + 2, " ===   QUICK QUIDE   ===");
		wattroff(win, A_BOLD | COLOR_PAIR(3));

		mvwprintw(win, 4, sidebar_x + 2, "• mv [src] [dest]");
		mvwprintw(win, 5, sidebar_x + 4, "Move the top card.");

		mvwprintw(win, 7, sidebar_x + 2, "• mv [src]|[idx] [dest]");
		mvwprintw(win, 8, sidebar_x + 4, "Move a sequence of cards.");

		mvwprintw(win, 10, sidebar_x + 2, "• hint");
		mvwprintw(win, 11, sidebar_x + 4, "Ask for hint (0 pts)");

		mvwprintw(win, 13, sidebar_x + 2, "• undo");
		mvwprintw(win, 14, sidebar_x + 4, "Travel back in time.");

		mvwprintw(win, 16, sidebar_x + 2, "• save");
		mvwprintw(win, 17, sidebar_x + 4, "Save progress & exit to menu");

		mvwprintw(win, 19, sidebar_x + 2, "• quit");
		mvwprintw(win, 20, sidebar_x + 4, "Return to main menu.");

		wattron(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, 22, sidebar_x + 2, " ===   GAME INFO   ===");
		wattroff(win, A_BOLD | COLOR_PAIR(3));

		int info_y = 22;
		int total_cards = state->definition->num_decks * 52;
		mvwprintw(win, info_y++, sidebar_x + 2, "Cards: %d | Piles: %d", total_cards, state->pile_count);

		info_y++;
		wattron(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, info_y++, sidebar_x + 2, " >>>   GOAL   <<<");
		wattroff(win, A_BOLD | COLOR_PAIR(3));

		info_y++;
		for (int i = 0; i < state->definition->win_cond_count && i < 2; i++) {
			WinCondition wc = state->definition->win_condition[i];
			if (wc.target_card_count == 0)
				mvwprintw(win, info_y++, sidebar_x + 2, "• Empty all %s pile", wc.name_condition);
			else
				mvwprintw(win, info_y++, sidebar_x + 2, "• Fill %s to %d", wc.name_condition, wc.target_card_count);
		}

		info_y++;
		wattron(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, info_y++, sidebar_x + 2, " >>>   RULES   <<<");
		wattroff(win, A_BOLD | COLOR_PAIR(3));

		info_y++;
		int printed_rules = 0;
		for (int i = 0; i < state->definition->rule_count && printed_rules < 6; i++) {
			moveRule r = state->definition->rules[i];
			if (r.is_auto)
				continue;

			char desc[50];
			flags_interpreter(r.flags, desc);

			mvwprintw(win, info_y++, sidebar_x + 2, "%3s -> %3s: %.18s", r.src_pile, r.dest_pile, desc);
			printed_rules++;
		}
	}
}

void read_command_timer(WINDOW *win, char *input_str, int max_len, Game_state *state) {
	int pos = 0;
	input_str[0] = '\0';
	wtimeout(win, 1000);

	int yMax, xMax;
	getmaxyx(win, yMax, xMax);

	while (1) {
		time_t current = time(NULL);
		int elapsed = (int)difftime(current, state->stats->start_time);
		int mins = elapsed / 60;
		int secs = elapsed % 60;

		wattron(win, A_BOLD);
		mvwprintw(win, 0, xMax - 40, " Time: %02d:%02d | Moves: %3d | Score: %4d", mins, secs,
				  state->stats->moves_count, state->stats->score);
		wattroff(win, A_BOLD);

		wmove(win, yMax - 1, 12 + pos);
		wrefresh(win);

		int ch = wgetch(win);

		if (ch == ERR)
			continue;

		if (ch == '\n' || ch == '\r') {
			input_str[pos] = '\0';
			break;
		} else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
			if (pos > 0) {
				pos--;
				mvwprintw(win, yMax - 1, 12 + pos, " ");
				wmove(win, yMax - 1, 12 + pos);
			}
		} else if (pos < max_len - 1 && ch >= 32 && ch <= 126) {
			input_str[pos++] = ch;
			wprintw(win, "%c", ch);
		}
	}
	wtimeout(win, -1);
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
	int ch;

	state->stats = malloc(sizeof(GameStats));
	state->stats->start_time = time(NULL);
	state->stats->moves_count = 0;
	state->stats->score = 0;
	state->stats->hint_src_pile = -1;
	state->stats->hint_dest_pile = -1;
	state->stats->hint_card_count = 0;

	char log_old[128] = "";
	char log_new[128] = "Game started! Type 'hint' if you feel lost.";
	char feedback_msg[128] = "";


	while (playing) {
		werase(game_win);
		box(game_win, 0, 0);
		mvwprintw(game_win, 0, 2, "Playing: %s", state->definition->game_name);
		draw_game_board(game_win, state);

		if (strlen(log_old) > 0) {
			wattron(game_win, A_DIM);
			mvwprintw(game_win, yMax - 3, 2, "  %s", log_old);
			wattroff(game_win, A_DIM);
		}

		wattron(game_win, COLOR_PAIR(2) | A_BOLD);
		mvwprintw(game_win, yMax - 2, 2, ">> %s", log_new);
		wattroff(game_win, COLOR_PAIR(2) | A_BOLD);

		mvwprintw(game_win, yMax - 1, 2, "Command > ");

		wrefresh(game_win);

		echo();
		curs_set(1);

		read_command_timer(game_win, input_str, 79, state);

		curs_set(0);

		if (strlen(input_str) == 0)
			continue;

		GameCommand cmd = parse_command(input_str);
		if (cmd.src > 0 && cmd.dest > 0) {
			state->stats->moves_count++;
			// TODO: ADDING POINTS;
		}
		feedback_msg[0] = '\0';
		playing = execute_command(state, cmd, feedback_msg);

		if (strlen(feedback_msg) > 0) {
			strcpy(log_old, log_new);
			strcpy(log_new, feedback_msg);
		}
	}
	delwin(game_win);
	endwin();
}
