// #include "../include/main.h"
#include "../include/cli.h"
#include "../include/gui.h"

void start_cli_mode() { run_cli(); }

void start_ncurses_mode() {
	MenuChoice choice = start_menu();
	if (choice == MENU_NEW_GAME) {
		GameDefinition *def = choose_patience("paciencias");
		if (def != NULL && def->game_name[0] != '\0') {
			Game_state *state = build_game_state(def);
			run_ncurses_gui(state);
		} else {
			printf("Failed to load the game definition.\n");
		}
	} else if (choice == MENU_CONTINUE) // TODOO!!
		;
	else {
		printf("Bye bye...\n");
	}
}


int main() {
	char choice[10];
	printf("Select interface mode:\n");
	printf("  1. Classic Terminal (CLI)\n");
	printf("  2. Graphical Interface (NCurses)\n");
	printf("Enter choice [1/2]: ");
	if (fgets(choice, sizeof(choice), stdin) != NULL) {
		choice[strcspn(choice, "\n")] = 0;
		if (strcmp(choice, "1") == 0 || strcasecmp(choice, "cli") == 0) {
			start_cli_mode();
		} else if (strcmp(choice, "2") == 0 || strcasecmp(choice, "ncurses") == 0) {
			start_ncurses_mode();
		} else {
			printf("Invalid option. Defaulting to CLI mode.\n");
			start_cli_mode();
		}
	}
	return 0;
}
