#include "../include/hint.h"

int evaluate_score(Game_state *state, int src_idx, int dest_idx, int card_count) {
	if (src_idx < 0 || dest_idx < 0)
		return 0;
	Pile *src = state->table_piles[src_idx];
	Pile *dest = state->table_piles[dest_idx];

	if (!src->pile_class || !dest->pile_class || !src || !dest)
		return 0;

	char *src_name = src->pile_class->name;
	char *dest_name = dest->pile_class->name;

	if (strstr(dest_name, "FUND") != NULL)
		return 10 * card_count;
	else if (strstr(src_name, "TAB") != NULL && strstr(dest_name, "DESCARTE") != NULL)
		return 10 * card_count;
	else if ((strstr(src_name, "STOCK") != NULL || strstr(src_name, "DESCARTE") != NULL) &&
			 strstr(dest_name, "TAB") != NULL)
		return 5;
	else if (strstr(src_name, "CELL") != NULL && strstr(dest_name, "TAB") != NULL)
		return 3;

	else if (strstr(src_name, "TAB") != NULL && strstr(dest_name, "TAB") != NULL)
		return 1;
	else if (strstr(src_name, "STOCK") != NULL && strstr(dest_name, "DESCARTE") != NULL)
		return 0;

	else if (strstr(src_name, "FUND") != NULL)
		return -15 * card_count;
	return 0;
}

void update_score(Game_state *state, int src_idx, int dest_idx, int card_count) {
	if (state->stats == NULL)
		return;
	int points_earned = evaluate_score(state, src_idx, dest_idx, card_count);

	state->stats->score += points_earned;
	if (state->stats->score < 0)
		state->stats->score = 0;
}

MoveList generate_hints(Game_state *current_state) {
	MoveList move_list = get_valid_moves(current_state, false);
	Game_state copy;
	if (move_list.count <= 0)
		return move_list;
	int best_score = -999999;
	int best_index = 0;

	for (int i = 0; i < move_list.count; i++) {
		MoveEvaluation *current_move = &move_list.moves[i];
		current_move->win_score = evaluate_score(current_state, current_move->move.src_pile,
												 current_move->move.dest_pile, current_move->move.card_count);
		if (current_move->win_score > best_score) {
			best_score = current_move->win_score;
			best_index = i;
		}
	}
	if (best_index != 0) {
		MoveEvaluation temp = move_list.moves[0];
		move_list.moves[0] = move_list.moves[best_index];
		move_list.moves[best_index] = temp;
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

	copy.stats = NULL;
	copy.history = NULL;

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

		Card *curr = state->table_piles[i]->head;
		while (curr != NULL) {
			Card *temp = curr;
			curr = curr->next;
			free(temp);
		}
		// Card *c = pop(state->table_piles[i]);
		// free(c);
		free(state->table_piles[i]);
	}
	free(state->table_piles);
}


void print_move_list(const MoveList *move_list) {
	printf("--- MoveList (Count: %d) ---\n", move_list->count);

	for (int i = 0; i < move_list->count; i++) {
		const Move *m = &move_list->moves[i].move;
		printf("[%d] sp = %d, c = %d, d = %d, card_count = %d, win_score = %d\n", i, m->src_pile, m->column_out,
			   m->dest_pile, m->card_count, move_list->moves[i].win_score);
	}

	printf("----------------------------\n");
}
