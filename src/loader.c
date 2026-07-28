#include "../include/loader.h"
#include <string.h>

GameDefinition *choose_patience(char *folder) {
	GameDefinition *r;
	DIR *d = opendir(folder);
	if (d == NULL) {
		printf("%s folder not found\n", folder);
		GameDefinition *empty = malloc(sizeof(*empty));
		empty->game_name[0] = '\0';
		return empty;
	}
	char files[50][256];
	int count = list_options(d, files);
	int option = input_patience(count);
	r = load_option(folder, files, option);
	return r;
}

GameDefinition *load_option(char *folder, char files[][256], int option) {
	char path[256];
	sprintf(path, "%s/%s", folder, files[option - 1]);
	GameDefinition *g = load_patience(path);
	printf("loading %s for you <3\n", path);
	strcpy(g->game_name, files[option - 1]);
	return g;
}

int list_options(DIR *d, char (*files)[256]) {
	struct dirent *entry;
	int count = 0;
	while ((entry = readdir(d)) != NULL) {
		if (entry->d_name[0] != '.') {
			strcpy(files[count], entry->d_name);
			printf("%d. %s\n", count + 1, entry->d_name);
			count++;
		}
	}
	return count;
}

int input_patience(int count) {
	int option = 0;
	char buffer[128];
	int bflag = 0;
	printf("What game do you wanna play?\n Choose a number\n");
	while (!bflag) {
		if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
			if (sscanf(buffer, "%d", &option) == 1 && option >= 1 &&
				option <= count)
				bflag = 1;
			else
				printf("Not a valid option. Please try again but this time a "
					   "number between 1 and %d\n",
					   count);
		}
	}
	return option;
}

// BUG: SEGMENTATION FAULT HERE!!!!
Game_state *build_game_state(GameDefinition *def) {
	if (def == NULL || def->game_name[0] == '\0')
		return NULL;
	Game_state *state = calloc(1, sizeof(*state));
	state->definition = def;
	Pile init_deck;
	init_deck.head = NULL;
	init_deck.num_cards = 0;
	for (int d = 0; d < def->num_decks; d++) {
		fill_deck(&init_deck);
	}
	shuffle_pile(&init_deck);
	state->pile_count = def->init_count;
	state->table_piles = malloc((state->pile_count + 1) * sizeof(Pile *));
	for (int i = 0; i < def->init_count; i++) {
		state->table_piles[i] = malloc(sizeof(Pile));
		state->table_piles[i]->head = NULL;
		state->table_piles[i]->num_cards = 0;
		int cards_needed = def->inits[i].num_cards;
		for (int c = 0; c < cards_needed; c++) {
			Card *card_to_move = pop(&init_deck);
			if (card_to_move != NULL) {
				push(state->table_piles[i], card_to_move);
			}
		}
	}
	if (init_deck.num_cards > 0) {
		int idx = state->pile_count;
		state->table_piles[idx] = malloc(sizeof(Pile));
		state->table_piles[idx]->head = init_deck.head;
		state->table_piles[idx]->num_cards = init_deck.num_cards;
		state->pile_count++;
	}
	// Set move
	state->move.src_pile = (-1);
    state->move.dest_pile = (-1);
    state->move.is_valid = SET;
    state->move.card_count = 0;

	return state;
}
