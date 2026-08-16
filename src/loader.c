#include "../include/loader.h"


Game_state *build_game_state(GameDefinition *def) {
	if (def == NULL || def->game_name[0] == '\0')
		return NULL;
	Game_state *state = calloc(1, sizeof(*state));
	state->definition = def;
	state->history = malloc(sizeof(History));
	if (state->history != NULL)
		initialize_history(state->history);
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
		state->table_piles[i]->pile_class = NULL;
		for (int c = 0; c < def->class_count; c++) {
			if (strcmp(def->pile_classes[c].name, def->inits[i].pile_name) == 0) {
				state->table_piles[i]->pile_class = &(def->pile_classes[c]);
				break;
			}
		}
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
		state->table_piles[idx]->pile_class = NULL;
		for (int c = 0; c < def->class_count; c++) {
			if (strcmp(def->pile_classes[c].name, "STOCK") == 0) {
				state->table_piles[idx]->pile_class = &(def->pile_classes[c]);
				break;
			}
		}
		state->pile_count++;
	}
	// Set move
	set_move(state);

	return state;
}

void set_move(Game_state *state) {
	state->move.src_pile = (-1);
	state->move.dest_pile = (-1);
	state->move.is_move_valid = SET;
	state->move.card_count = 0;
}
