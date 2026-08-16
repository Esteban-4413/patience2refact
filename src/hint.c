#include "../include/hint.h"

int evaluate_score(Game_state *state, int src_idx, int dest_idx, int card_count) {
	if (src_idx < 0 || dest_idx < 0)
		return 0;

	Pile *src = state->table_piles[src_idx];
	Pile *dest = state->table_piles[dest_idx];

	if (!src || !dest || !src->pile_class || !dest->pile_class)
		return 0;

	PileClass *src_class = src->pile_class;
	PileClass *dest_class = dest->pile_class;

	bool dest_is_fill_goal = false;
	bool dest_is_empty_goal = false;
	bool src_is_fill_goal = false;
	bool src_is_empty_goal = false;

	for (int i = 0; i < state->definition->win_cond_count; i++) {
		WinCondition wc = state->definition->win_condition[i];

		if (strcmp(dest_class->name, wc.name_condition) == 0) {
			if (wc.target_card_count > 0)
				dest_is_fill_goal = true;
			if (wc.target_card_count == 0)
				dest_is_empty_goal = true;
		}
		if (strcmp(src_class->name, wc.name_condition) == 0) {
			if (wc.target_card_count > 0)
				src_is_fill_goal = true;
			if (wc.target_card_count == 0)
				src_is_empty_goal = true;
		}
	}

	int base_score = 0;

	if (dest_is_fill_goal) {
		base_score = 10;
	} else if (src_is_fill_goal) {
		base_score = -15;
	} else if (src_is_empty_goal && !dest_is_empty_goal) {
		base_score = 10;
	} else if (!src_is_empty_goal && dest_is_empty_goal) {
		base_score = -15;
	} else {
		if (src_class->max_one_card && !dest_class->max_one_card) {
			base_score = 3;
		} else if (!src_class->max_one_card && dest_class->max_one_card) {
			base_score = 0;
		} else if (!src_class->visible_all && dest_class->visible_all) {
			base_score = 5;
		} else if (src_class->visible_all && dest_class->visible_all) {
			base_score = 1;
		}
	}

	return base_score * card_count;
}

int update_score(Game_state *state, int src_idx, int dest_idx, int card_count) {
	if (state->stats == NULL)
		return 0;
	int points_earned = evaluate_score(state, src_idx, dest_idx, card_count);

	state->stats->score += points_earned;
	if (state->stats->score < 0)
		state->stats->score = 0;
	return points_earned;
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
