#include "../include/gui.h"


MenuChoice start_menu() {
	MenuChoice choice;
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(1);
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW *win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
	box(win, 0, 0);
	mvwprintw(win, 0, 2, "Patience menu");
	mvwprintw(win, 0, 15, "Options");
	char ch;
	while (ch = wgetch(win)) {
		switch (ch) {
		case 'h':
			wattron(win, A_STANDOUT);
			mvwprintw(win, 0, 2, "Patience menu");
			wattroff(win, A_STANDOUT);
		case 'j':
			break;
		case 'k':
			break;
		case 'l':
			wattron(win, A_STANDOUT);
			mvwprintw(win, 0, 15, "Options");
			wattroff(win, A_STANDOUT);
			break;
		default:
			mvwprintw(win, 0, 2, "Patience menu");
			mvwprintw(win, 0, 15, "Options");
			break;
		}

	default:
	}

	wgetch(win);
	endwin();
	return choice;
}
