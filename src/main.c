// #include "../include/main.h"
// #include "../include/cli.h"
#include "../include/gui.h"
#include <stdio.h>

// void start_cli_mode() { run_cli(); }

void start_ncurses_mode() {
	bool game_running = true;
	while (game_running) {
		char chosen_file[128] = {0};
		MenuChoice choice = start_menu(chosen_file);
		if (choice == MENU_QUIT) {
			game_running = false;
			break;
		} else if (choice == MENU_NEW_GAME) {
			if (chosen_file[0] != '\0') {
				char path[256];
				snprintf(path, sizeof(path), "paciencias/%s", chosen_file);
				GameDefinition *def = load_patience(path);
				if (def != NULL && def->game_name[0] != '\0') {
					Game_state *state = build_game_state(def);
					run_ncurses_gui(state);
					free_state(state);
					free(def);
				} else
					printf("Failed to load the game definition from %s.\n", path);
			} else
				printf("No game was selected.\n");
		} else if (choice == MENU_CONTINUE) {
			if (chosen_file[0] != '\0') {
				char path[256];
				snprintf(path, sizeof(path), "saves/%s", chosen_file);
				Game_state *state = load_game(path);
				if (state != NULL) {
					run_ncurses_gui(state);
					free_state(state);
				} else
					printf("Faile to load the save from %s.\n", path);
			} else
				printf("No save file was selected.\n");
		}
	}
}


int main() {
	setlocale(LC_ALL, "");
	// char choice[10];
	// printf("Select interface mode:\n");
	// printf("  1. Classic Terminal (CLI)\n");
	// printf("  2. Graphical Interface (NCurses)\n");
	// printf("Enter choice [1/2]: ");

	start_ncurses_mode();

	// if (fgets(choice, sizeof(choice), stdin) != NULL) {
	// 	choice[strcspn(choice, "\n")] = 0;
	// 	if (strcmp(choice, "1") == 0 || strcasecmp(choice, "cli") == 0) {
	// 		start_cli_mode();
	// 	} else if (strcmp(choice, "2") == 0 || strcasecmp(choice, "ncurses") == 0) {

	// 	} else {
	// 		printf("Invalid option. Defaulting to CLI mode.\n");
	// 		start_cli_mode();
	// 	}
	// }
	return 0;
}
