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

void print_move_list(const MoveList *move_list) {
	printf("--- MoveList (Count: %d) ---\n", move_list->count);

	for (int i = 0; i < move_list->count; i++) {
		const Move *m = &move_list->moves[i].move;
		printf("[%d] sp = %d, c = %d, d = %d, card_count = %d, win_score = %d\n", i, m->src_pile, m->column_out,
			   m->dest_pile, m->card_count, move_list->moves[i].win_score);
	}

	printf("----------------------------\n");
}
