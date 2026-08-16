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

void draw_game_description(WINDOW *win, const char *filename, int y, int x, int max_w, int max_h) {
	wattron(win, A_BOLD | COLOR_PAIR(3));


	if (strstr(filename, "klondike") != NULL || strstr(filename, "Klondike") != NULL) {
		mvwprintw(win, y++, x, "=== KLONDIKE ===");
		y++;
		wattroff(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, y++, x, "The classic Solitaire.");
		mvwprintw(win, y++, x, "Goal: Move all cards to the 4 foundations.");
		y++;
		wattron(win, A_UNDERLINE);
		mvwprintw(win, y++, x, "Rules:");
		wattroff(win, A_UNDERLINE);
		y++;
		mvwprintw(win, y++, x, "1. Tableaus build down by alternate color.");
		mvwprintw(win, y++, x, "2. Foundations build up by suit.");
		mvwprintw(win, y++, x, "3. Only kings can be placed on empty columns.");

	} else if (strstr(filename, "freeCell") != NULL || strstr(filename, "FreeCell") != NULL) {
		mvwprintw(win, y++, x, "=== FREECELL ===");
		y++;
		wattroff(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, y++, x, "A strategic, open-card game.");
		mvwprintw(win, y++, x, "Goal: Move all cards to the foundations.");
		y++;
		wattron(win, A_UNDERLINE);
		mvwprintw(win, y++, x, "Rules:");
		wattroff(win, A_UNDERLINE);
		y++;
		mvwprintw(win, y++, x, "1. Tableaus build down by alternate color.");
		mvwprintw(win, y++, x, "2. You have 4 'Free Cells' to hold any 1 card.");
		mvwprintw(win, y++, x, "3. Moving sequences depends on empty cells available.");

	} else if (strstr(filename, "golf") != NULL || strstr(filename, "Golf") != NULL) {
		mvwprintw(win, y++, x, "=== GOLF ===");
		y++;
		wattroff(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, y++, x, "Fast-paced sequencing.");
		mvwprintw(win, y++, x, "Goal: Clear the tableau using the waste pile.");
		y++;
		wattron(win, A_UNDERLINE);
		mvwprintw(win, y++, x, "Rules:");
		wattroff(win, A_UNDERLINE);
		y++;
		mvwprintw(win, y++, x, "1. Play cards 1 rank higher or lower than the waste.");
		mvwprintw(win, y++, x, "2. Suits and colors do not matter.");
		mvwprintw(win, y++, x, "3. Kings cannot wrap to Aces.");

	} else if (strstr(filename, "simpleSimon") != NULL || strstr(filename, "SimpleSimon") != NULL) {
		mvwprintw(win, y++, x, "=== SIMPLE SIMON ===");
		y++;
		wattroff(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, y++, x, "A spider-like variant.");
		mvwprintw(win, y++, x, "Goal: Build 4 full descending sequences in suit.");
		y++;
		wattron(win, A_UNDERLINE);
		mvwprintw(win, y++, x, "Rules:");
		wattroff(win, A_UNDERLINE);
		y++;
		mvwprintw(win, y++, x, "1. Tableaus build down regardless of suit.");
		mvwprintw(win, y++, x, "2. You can only move sequences of the same suit.");

	} else {
		mvwprintw(win, y++, x, "=== %s ===", filename);
		y++;
		wattroff(win, A_BOLD | COLOR_PAIR(3));
		mvwprintw(win, y++, x, "Select this game to read the auto-generated rules!");
	}
}

void draw_main_panel(WINDOW *win, int selected_menu, int active_option, int active_pane, int right_option,
					 char patience_files[][64], int num_files, int yMax, int xMax, char saved_files[][64],
					 int num_saved_files) {

	int h = yMax;
	int w = xMax;

	switch (selected_menu) {
	case 0:
		int split_x = w / 3;
		mvwvline(win, 1, split_x, ACS_VLINE, h - 2);

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
			if (num_files == 0)
				mvwprintw(win, 3, split_x + 2, "   (No files found)   ");
			else {
				for (int i = 0; i < num_files && (3 + i) < (h - 1); i++) {
					if (active_pane == 1 && right_option == i)
						wattron(win, A_REVERSE);
					mvwprintw(win, 3 + i, split_x + 2, " %d.%s ", i + 1, patience_files[i]);
					if (active_pane == 1 && right_option == i)
						wattroff(win, A_REVERSE);
				}
			}
			if (num_files == 0)
				mvwprintw(win, 3, split_x + 2, "  (No files found)  ");
		} else if (active_option == 1) {
			mvwprintw(win, 1, split_x + 2, "  Saved games:  ");
			if (num_saved_files == 0)
				mvwprintw(win, 3, split_x + 2, "  (No saved games)  ");
			else {
				for (int i = 0; i < num_saved_files && (3 + i) < (h - 1); i++) {
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

#define MAX_DROPS 80

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

typedef struct {
	float x, y, speed;
	Suit suit;
} RainDrop;

void init_rain(RainDrop drops[], int max_x, int max_y) {
	int spacing = max_x / (MAX_DROPS > 0 ? MAX_DROPS : 1);
	if (spacing < 1)
		spacing = 1;
	for (int i = 0; i < MAX_DROPS; i++) {
		drops[i].x = (i * spacing) + (rand() % spacing);
		if (drops[i].x >= max_x)
			drops[i].x = max_x - 1;
		drops[i].y = (float)(rand() % max_y);
		drops[i].speed = 0.40f + ((float)rand() / (float)RAND_MAX) * 0.30f;
		drops[i].suit = rand() % 4;
	}
}

void draw_rain(WINDOW *win, RainDrop drops[], int max_x, int max_y) {
	for (int i = 0; i < MAX_DROPS; i++) {
		drops[i].y += drops[i].speed;
		if (drops[i].y >= max_y) {
			drops[i].y = 0.0f;
			drops[i].x = rand() % max_x;
			drops[i].suit = rand() % 4;
			drops[i].speed = 0.40f + ((float)rand() / (float)RAND_MAX) * 0.30f;
		}
		int color = (drops[i].suit == SUIT_HEART || drops[i].suit == SUIT_DIAMOND) ? 1 : 3;
		wattron(win, COLOR_PAIR(color) | A_BOLD);
		mvwprintw(win, (int)drops[i].y, drops[i].x, "%s", get_unicode_suit(drops[i].suit));
		wattroff(win, COLOR_PAIR(color) | A_BOLD);
	}
}

typedef enum { STATE_MAIN, STATE_PLAY, STATE_HELP } MenuState;

MenuChoice start_menu(char *chosen_file_out) {
	MenuChoice choice;
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	timeout(50);

	if (has_colors()) {
		start_color();
		use_default_colors();
		init_pair(1, COLOR_RED, -1);
		init_pair(2, COLOR_YELLOW, -1);
		init_pair(3, COLOR_CYAN, -1);
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

	char patience_files[20][64];
	int num_files = get_patience_file(patience_files, 20, "paciencias", ".paciencia");
	char saved_files[20][64];
	int num_saved_files = get_patience_file(saved_files, 20, "saves", ".txt");

	RainDrop drops[MAX_DROPS];
	init_rain(drops, xMax, yMax);

	MenuState current_state = STATE_MAIN;
	int main_sel = 0;
	int play_tab = 0;
	int list_sel = 0;

	int ch;
	bool running = true;

	while (running) {
		werase(stdscr);
		draw_rain(stdscr, drops, xMax, yMax);

		int start_y = yMax * 0.15;
		int start_x = (xMax - art_width) / 2;
		if (start_x < 0)
			start_x = 0;

		attron(COLOR_PAIR(3) | A_BOLD);
		for (int i = 0; i < title_height; i++)
			mvprintw(start_y + i, start_x, "%s", ascii_title[i]);
		attroff(COLOR_PAIR(3) | A_BOLD);

		WINDOW *popup_win = NULL;


		if (current_state == STATE_MAIN) {
			const char *options[] = {" PLAY ", " HELP ", " QUIT "};
			int opt_y = start_y + title_height + 4;
			for (int i = 0; i < 3; i++) {
				if (i == main_sel)
					attron(A_REVERSE | COLOR_PAIR(3) | A_BOLD);
				mvprintw(opt_y + (i * 2), (xMax - 6) / 2, "%s", options[i]);
				if (i == main_sel)
					attroff(A_REVERSE | COLOR_PAIR(3) | A_BOLD);
			}
			mvprintw(yMax - 2, 2, "Use [j/k] to navigate, [Enter] to select");

		} else if (current_state == STATE_PLAY || current_state == STATE_HELP) {
			int box_w = xMax * 0.8;
			if (box_w < 76)
				box_w = 76;
			int box_h = yMax * 0.6;
			if (box_h < 15)
				box_h = 15;
			int box_y = start_y + title_height + 2;
			int box_x = (xMax - box_w) / 2;

			popup_win = newwin(box_h, box_w, box_y, box_x);
			box(popup_win, 0, 0);

			if (current_state == STATE_PLAY) {
				mvwprintw(popup_win, 0, 2, "┌ tab ┐");
				wattron(popup_win, A_BOLD);
				if (play_tab == 0)
					wattron(popup_win, COLOR_PAIR(3));
				mvwprintw(popup_win, 0, 10, " 1.New Game ");
				if (play_tab == 0)
					wattroff(popup_win, COLOR_PAIR(3));

				if (play_tab == 1)
					wattron(popup_win, COLOR_PAIR(3));
				mvwprintw(popup_win, 0, 23, " 2.Continue ");
				if (play_tab == 1)
					wattroff(popup_win, COLOR_PAIR(3));
				wattroff(popup_win, A_BOLD);

				int split_x = box_w / 2;
				mvwvline(popup_win, 1, split_x, ACS_VLINE, box_h - 2);
				mvwprintw(popup_win, 0, split_x, "┬");
				mvwprintw(popup_win, box_h - 1, split_x, "┴");

				int limit = (play_tab == 0) ? num_files : num_saved_files;
				char (*current_list)[64] = (play_tab == 0) ? patience_files : saved_files;

				for (int i = 0; i < limit && i < box_h - 2; i++) {
					if (i == list_sel)
						wattron(popup_win, A_REVERSE | A_BOLD | COLOR_PAIR(3));
					mvwprintw(popup_win, 2 + i, 2, " %s ", current_list[i]);
					if (i == list_sel)
						wattroff(popup_win, A_REVERSE | A_BOLD | COLOR_PAIR(3));
				}
				if (limit == 0)
					mvwprintw(popup_win, 2, 2, " (No files found) ");

				if (limit > 0) {
					draw_game_description(popup_win, current_list[list_sel], 2, split_x + 3, box_w - split_x - 4,
										  box_h);
				}

				mvwprintw(popup_win, box_h - 1, 2, " [q] Back  [1/2] Tabs  [Enter] Start ");
			} else if (current_state == STATE_HELP) {
				mvwprintw(popup_win, 0, 2, "┌ Help ┐");
				wattron(popup_win, A_BOLD | COLOR_PAIR(3));
				mvwprintw(popup_win, 2, 4, "=== How to move ===");
				wattroff(popup_win, A_BOLD | COLOR_PAIR(3));

				mvwprintw(popup_win, 4, 4, "mv 1 2   : Move TOP card from pile 1 to pile 2");
				mvwprintw(popup_win, 5, 4, "mv 1|3 2 : Move SEQUENCE starting at index 3 from pile 1 to 2");

				wattron(popup_win, A_BOLD | COLOR_PAIR(3));
				mvwprintw(popup_win, 8, 4, "=== Rules glossary ===");
				wattroff(popup_win, A_BOLD | COLOR_PAIR(3));

				wattron(popup_win, A_UNDERLINE);
				mvwprintw(popup_win, 10, 4, "Moving to a Destination Pile:");
				wattroff(popup_win, A_UNDERLINE);
				mvwprintw(popup_win, 11, 4, "-1 Rnk.    : Source card must be 1 rank LOWER (e.g. 7 onto 8)");
				mvwprintw(popup_win, 12, 4, "+1 Rnk.    : Source card must be 1 rank HIGHER (e.g. 3 onto 2)");
				mvwprintw(popup_win, 13, 4, "Adj.       : Source card must be EXACTLY 1 rank lower or higher");
				mvwprintw(popup_win, 14, 4, "Alt color. : Source card must be opposite color of Destination");
				mvwprintw(popup_win, 15, 4, "Same suit. : Source card must be same suit as Destination");
				mvwprintw(popup_win, 16, 4, "To empty.  : Destination must be an empty space");
				mvwprintw(popup_win, 17, 4, "Kings to empty : Only Kings can be moved to an empty space");

				wattron(popup_win, A_UNDERLINE);
				mvwprintw(popup_win, 19, 4, "Moving a Sequence of Cards:");
				wattroff(popup_win, A_UNDERLINE);
				mvwprintw(popup_win, 20, 4, "If the game allows it, the sequence you select (e.g. mv 1|3 2)");
				mvwprintw(popup_win, 21, 4, "must be properly sorted BEFORE moving (e.g., same suit, -1 rnk).");
			}
		}

		wnoutrefresh(stdscr);
		if (popup_win != NULL) {
			wnoutrefresh(popup_win);
		}
		doupdate();

		if (popup_win != NULL) {
			keypad(popup_win, TRUE);
			wtimeout(popup_win, 50);
			ch = wgetch(popup_win);
			delwin(popup_win);
		} else {
			timeout(50);
			ch = wgetch(stdscr);
		}

		if (ch != ERR) {
			if (ch == 'q' || ch == 27) {
				if (current_state == STATE_PLAY || current_state == STATE_HELP) {
					current_state = STATE_MAIN;
				} else if (current_state == STATE_MAIN) {
					choice = MENU_QUIT;
					running = false;
				}
			} else if (ch == '\n' || ch == '\r') {
				if (current_state == STATE_MAIN) {
					if (main_sel == 0) {
						current_state = STATE_PLAY;
						list_sel = 0;
					} else if (main_sel == 1)
						current_state = STATE_HELP;
					else if (main_sel == 2) {
						choice = MENU_QUIT;
						running = false;
					}
				} else if (current_state == STATE_PLAY) {
					int limit = (play_tab == 0) ? num_files : num_saved_files;
					if (limit > 0) {
						choice = (play_tab == 0) ? MENU_NEW_GAME : MENU_CONTINUE;
						strcpy(chosen_file_out, (play_tab == 0) ? patience_files[list_sel] : saved_files[list_sel]);
						running = false;
					}
				}
			} else if (ch == '1' && current_state == STATE_PLAY) {
				play_tab = 0;
				list_sel = 0;
			} else if (ch == '2' && current_state == STATE_PLAY) {
				play_tab = 1;
				list_sel = 0;
			} else if (ch == 'j' || ch == KEY_DOWN) {
				if (current_state == STATE_MAIN)
					main_sel = (main_sel + 1) % 3;
				else if (current_state == STATE_PLAY) {
					int limit = (play_tab == 0) ? num_files : num_saved_files;
					if (limit > 0)
						list_sel = (list_sel + 1) % limit;
				}
			} else if (ch == 'k' || ch == KEY_UP) {
				if (current_state == STATE_MAIN)
					main_sel = (main_sel + 2) % 3;
				else if (current_state == STATE_PLAY) {
					int limit = (play_tab == 0) ? num_files : num_saved_files;
					if (limit > 0)
						list_sel = (list_sel + limit - 1) % limit;
				}
			}
		}
	}

	timeout(-1);
	endwin();
	return choice;
}

int get_suit_color(Suit suit) {
	if (suit == SUIT_HEART || suit == SUIT_DIAMOND)
		return 1;
	return 0;
}

void draw_empty_pile(WINDOW *win, int y, int x, bool is_higlighted) {
	if (is_higlighted)
		wattron(win, A_REVERSE | COLOR_PAIR(2));
	mvwprintw(win, y, x, "┌─────┐");
	mvwprintw(win, y + 1, x, "│%5s│", " ");
	mvwprintw(win, y + 2, x, "│ ─── │");
	mvwprintw(win, y + 3, x, "│%5s│", " ");
	mvwprintw(win, y + 4, x, "└─────┘");
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
		mvwprintw(win, y, x, "┌─────┐");
		mvwprintw(win, y + 1, x, "│%5s│", " ");
		mvwprintw(win, y + 2, x, "│░░░░░│");
		mvwprintw(win, y + 3, x, "│%5s│", " ");
		mvwprintw(win, y + 4, x, "└─────┘");
	} else {
		int color_pair = get_suit_color(c->suit);
		if (color_pair == 1)
			wattron(win, COLOR_PAIR(color_pair));

		char rank_str[4];
		translate_rank(rank_str, c->rank);

		mvwprintw(win, y, x, "┌─────┐");
		mvwprintw(win, y + 1, x, "│%-2s   │", rank_str);
		mvwprintw(win, y + 2, x, "│  %s  │", get_unicode_suit(c->suit));
		mvwprintw(win, y + 3, x, "│   %2s│", rank_str);
		mvwprintw(win, y + 4, x, "└─────┘");

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
	return current_y + 3;
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
	if (spacing_x > 11)
		spacing_x = 11;
	if (spacing_x < 8)
		spacing_x = 8;

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
		mvwprintw(win, 0, xMax - 52, " Time: %02d:%02d | Moves: %3d | Score: %4d | Hints: %d/3", mins, secs,
				  state->stats->moves_count, state->stats->score, 3 - state->stats->hints_used);
		wattroff(win, A_BOLD);

		wmove(win, yMax - 2, 5 + pos);
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
				input_str[0] = '\0';
				wmove(win, yMax - 2, 5 + pos);
				wclrtoeol(win);

				wmove(win, yMax - 2, 5 + pos);
			}
		} else if (pos < max_len - 1 && ch >= 32 && ch <= 126) {
			input_str[pos++] = ch;
			// input_str[0] = '\0';
			// wprintw(win, "%c", ch);
		}
	}
	wtimeout(win, -1);
}
void draw_victory_screen(WINDOW *win, Game_state *state) {
	int yMax, xMax;
	getmaxyx(win, yMax, xMax);

	RainDrop drops[MAX_DROPS];
	init_rain(drops, xMax, yMax);

	const char *ascii_win[] = {" __   __  _______  __   __    _     _  ___   __    _  __ ",
							   "|  | |  ||       ||  | |  |  | | _ | ||   | |  |  | ||  |",
							   "|  |_|  ||   _   ||  | |  |  | || || ||   | |   |_| ||  |",
							   "|       ||  | |  ||  |_|  |  |       ||   | |       ||  |",
							   "|_     _||  |_|  ||       |  |       ||   | |  _    ||__|",
							   "  |   |  |       ||       |  |   _   ||   | | | |   | __ ",
							   "  |___|  |_______||_______|  |__| |__||___| |_|  |__||__|"};
	int art_height = 7;
	int art_width = 59;
	int start_y = (yMax / 2) - (art_height / 2) - 3;
	int start_x = (xMax - art_width) / 2;

	time_t current = time(NULL);
	int elapsed = (int)difftime(current, state->stats->start_time);
	int mins = elapsed / 60;
	int secs = elapsed % 60;

	wtimeout(win, 50);

	while (1) {
		int ch = wgetch(win);
		if (ch != ERR)
			break;
		werase(win);
		draw_rain(win, drops, xMax, yMax);
		box(win, 0, 0);
		wattron(win, COLOR_PAIR(2) | A_BOLD);
		for (int i = 0; i < art_height; i++) {
			mvwprintw(win, start_y + i, start_x, "%s", ascii_win[i]);
		}
		wattroff(win, COLOR_PAIR(2) | A_BOLD);

		wattron(win, COLOR_PAIR(3) | A_BOLD);
		mvwprintw(win, start_y + art_height + 2, (xMax - 30) / 2, "Final Score: %4d", state->stats->score);
		mvwprintw(win, start_y + art_height + 3, (xMax - 30) / 2, "Time: %02d:%02d | Moves: %3d", mins, secs,
				  state->stats->moves_count);
		wattroff(win, COLOR_PAIR(3) | A_BOLD);

		wattron(win, A_BLINK);
		mvwprintw(win, yMax - 3, (xMax - 36) / 2, "Press ANY KEY to return to Menu...");
		wattroff(win, A_BLINK);

		wrefresh(win);
	}
	wtimeout(win, -1);
}

void run_ncurses_gui(Game_state *state) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	srand(time(NULL));

	if (has_colors()) {
		start_color();
		use_default_colors();
		init_pair(1, COLOR_RED, -1);
		init_pair(2, COLOR_YELLOW, -1);
		init_pair(3, COLOR_CYAN, -1);
	}
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW *game_win = newwin(yMax, xMax, 0, 0);
	keypad(game_win, TRUE);
	bool playing = true;
	char input_str[80];
	int ch;

	if (state->stats == NULL) {
		state->stats = malloc(sizeof(GameStats));
		state->stats->start_time = time(NULL);
		state->stats->moves_count = 0;
		state->stats->score = 0;
	}
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
			mvwprintw(game_win, yMax - 4, 2, "  %s", log_old);
			wattroff(game_win, A_DIM);
		}

		wattron(game_win, COLOR_PAIR(2) | A_BOLD);
		mvwprintw(game_win, yMax - 3, 2, "| %s", log_new);
		wattroff(game_win, COLOR_PAIR(2) | A_BOLD);

		// wmove(game_win, yMax - 2, 2);
		mvwprintw(game_win, yMax - 2, 2, ">> ");
		wrefresh(game_win);

		echo();
		curs_set(1);

		read_command_timer(game_win, input_str, 79, state);

		curs_set(0);

		if (strlen(input_str) == 0)
			continue;

		GameCommand cmd = parse_command(input_str);
		feedback_msg[0] = '\0';
		playing = execute_command(state, cmd, feedback_msg);

		if (strlen(feedback_msg) > 0) {
			strcpy(log_old, log_new);
			strcpy(log_new, feedback_msg);
		}

		if (win_codition(state)) {
			draw_victory_screen(game_win, state);
			playing = false;
		}
	}
	delwin(game_win);
	endwin();
}
