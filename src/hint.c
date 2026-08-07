#include "../include/hint.h"


MoveList generate_hints(Game_state *current_state) {
	Move mv;
	MoveList move_list = get_valid_moves(current_state, false);
	Game_state copy;

	if (move_list.count <= 0) {
		mv.src_pile = (-1);
		// não tem movimentos possíveis nesse estado de jogo
	} else {
		for (int i = 0; i < move_list.count; i++) {
			copy = clone_state(current_state);
			MoveEvaluation current_move = move_list.moves[i];
			bool isn_visible = false;
			isn_visible = current_state->table_piles[current_move.move.src_pile]->pile_class->visible_none;

			if (isn_visible) {
				current_move.win_score = win_codition(current_state);
			} else {
				copy.move = current_move.move;
				do_move(&copy);
				current_move.win_score = win_codition(&copy);
			}
			free_state(&copy);
		}

		quickSort(move_list.moves, move_list.count);
		mv = move_list.moves[0].move;
	}


	return move_list;
}

void quickSort(MoveEvaluation *moves, int N) {
	if (N > 1) {
		int p = partition(moves, N);
		quickSort(moves, p);
		quickSort(moves + p + 1, N - p - 1);
	}
}

int partition(MoveEvaluation *moves, int N) {
	int i, p;
	for (i = p = 0; i < N - 1; i++) {
		if (moves[i].win_score >= moves[N - 1].win_score) {
			swap_hint(moves, p, i);
			p++;
		}
	}
	swap_hint(moves, p, N - 1);
	return p;
}

void swap_hint(MoveEvaluation *moves, int a, int b) {
	MoveEvaluation temp = moves[a];
	moves[a] = moves[b];
	moves[b] = temp;
}

Game_state clone_state(Game_state *current_state) {
	Game_state copy;
	copy = *current_state;

	copy.table_piles = malloc((current_state->pile_count + 1) * sizeof(Pile *));

	for (int i = 0; i < copy.pile_count; i++) {
		copy.table_piles[i] = malloc(sizeof(Pile));
		copy.table_piles[i]->head = NULL;
		copy.table_piles[i]->num_cards = 0;
		copy.table_piles[i]->pile_class = NULL;
		GameDefinition *def = current_state->definition;
		// Pile Class
		for (int c = 0; c < def->class_count; c++) {
			if (strcmp(def->pile_classes[c].name, def->inits[i].pile_name) == 0) {
				copy.table_piles[i]->pile_class = &(def->pile_classes[c]);
				break;
			}
		}

		// Copy cards
		for (int j = 0; j < current_state->table_piles[i]->num_cards; j++) {
			Card *card_og = peek_card_at(current_state->table_piles[i], j);
			Suit suit = card_og->suit;
			Rank rank = card_og->rank;
			Card *card_cpy = make_card(suit, rank);
			unshift(copy.table_piles[i], card_cpy);
		}
	}

	return copy;
}

void free_state(Game_state *state) {
	for (int i = 0; i < state->pile_count; i++) {
		for (int j = 0; j < state->table_piles[i]->num_cards; j++) {
			Card *curr = state->table_piles[i]->head;
			while (curr != NULL) {
				Card *temp = curr;
				curr = curr->next;
				free(temp);
			}
			// Card *c = pop(state->table_piles[i]);
			// free(c);
		}
		free(state->table_piles[i]);
	}
	free(state->table_piles);
}
