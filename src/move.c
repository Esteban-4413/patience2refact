#include "../include/move.h"
#include "../include/cli.h"

void print_move(Game_state *state) {
	Move m = state->move;
	printf("Pile src: %d | Column: %d | Pile dest: %d | num: %d | Flag: %d"
		   "\n",
		   m.src_pile, m.column_out, m.dest_pile, m.card_count, m.is_move_valid);
}

bool fill_move(Game_state *current_state, GameCommand *cmd) {
	int src = cmd->src - 1;
	int dest = cmd->dest - 1;
	Pile *src_pile = current_state->table_piles[src];
	int column_input = (cmd->card_index_input == -1) ? 0 : cmd->card_index_input;

	int cards_to_move = src_pile->num_cards - column_input;

	current_state->move.src_pile = src;
	current_state->move.dest_pile = dest;
	current_state->move.column_out = (column_input == 0) ? column_input : src_pile->num_cards - column_input;
	current_state->move.card_count = (column_input == 0) ? 1 : src_pile->num_cards - column_input + 1;
	if (current_state->move.card_count <= 0 || current_state->move.column_out < 0) {
		return false;
	}
	return true;
}

void do_move(Game_state *current_state) {
	Move move = current_state->move;
	Pile *src_pile = current_state->table_piles[move.src_pile];
	Pile *dest_pile = current_state->table_piles[move.dest_pile];
	if (move.src_pile < 0 || move.src_pile >= current_state->pile_count || move.dest_pile < 0 ||
		move.dest_pile >= current_state->pile_count)
		return;
	if (src_pile == NULL || src_pile->head == NULL)
		return;
	int index = move.column_out;
	// Card *dest_head = dest_pile->head;
	Card *card = peek_card_at(src_pile, index);
	if (card == NULL)
		return;

	Card *src_head = src_pile->head;

	src_pile->head = card->next;
	card->next = dest_pile->head;
	dest_pile->head = src_head;

	src_pile->num_cards -= move.card_count;
	dest_pile->num_cards += move.card_count;
}

bool flag_checker(uint32_t flags, Move mov, Pile *src, Pile *dest, Card *moving_card) {
	int n = mov.card_count;
	if (flags & F_NONE) {
		return true;
	}
	if (n > 1) {
		if (!(flags & F_SEQUENCE))
			return false;
		if (flags & F_DESCENDING) {
			if (!is_seq_ascending(src->head, n))
				return false;
		}
		if (!(flags & F_ASCENDING)) {
			if (!is_seq_descending(src->head, n))
				return false;
		}
		if (flags & F_SUIT_SAME_SEQ) {
			if (!is_seq_same_suit(src->head, n))
				return false;
		}
		if (flags & F_SUIT_ALT_SEQ) {
			if (!is_seq_alt_suit(src->head, n))
				return false;
		}
		if (flags & F_COLOR_SAME_SEQ) {
			if (!is_seq_same_color(src->head, n))
				return false;
		}
		if (flags & F_COLOR_ALT_SEQ) {
			if (!is_seq_alt_color(src->head, n))
				return false;
		}
	}

	// flags que comparam as cartas da src_pile com as cartas do dest_pile
	// se a dest_pile não tiver cartas então não vale a pena entrar ai
	if (dest->head != NULL) { // neste bloque if lowkey não sei se usar moving_card instead of src_head
		if (flags & F_VAL_LOWER) {
			if (moving_card->rank != dest->head->rank - 1)
				return false;
		}
		if (flags & F_VAL_HIGHER) {
			if (moving_card->rank != dest->head->rank + 1)
				return false;
		}
		if (flags & F_VAL_ADJACENT) {
			if (abs((int)moving_card->rank - (int)dest->head->rank) != 1)
				return false;
		}
		if (flags & F_SUIT_SAME_DST) {
			if (moving_card->suit != dest->head->suit)
				return false;
		}
		if (flags & F_SUIT_DIFF_DST) {
			if (moving_card->suit == dest->head->suit)
				return false;
		}
		if (flags & F_COLOR_SAME_DST) {
			if (is_alternate_color(moving_card, dest->head))
				return false;
		}
		if (flags & F_EMPTY_DEST)
			return false;
	}
	if (flags & F_TOP_ACE) {
		if (src->head->rank != RANK_ACE)
			return false;
	}
	if (flags & F_BOTTOM_ACE) {
		if ( moving_card->rank != RANK_ACE)
			return false;
	}
	if (flags & F_BOTTOM_KING) {
		if (moving_card->rank  != RANK_KING)
			return false;
	}
	if (flags & F_TOP_KING) {
		if (src->head->rank != RANK_KING)
			return false;
	}
	return true;
}

bool is_move_valid(Game_state *current_state) {
	Move mov = current_state->move;
	Pile *src = current_state->table_piles[mov.src_pile];
	Pile *dest = current_state->table_piles[mov.dest_pile];
	// uint32_t flags = current_state->definition->rules->flags;
	// int n = mov.card_count;
	Card *moving_card = peek_card_at(src, mov.column_out);
	if (moving_card == NULL)
		return false;

	for (int i = 0; i < current_state->definition->rule_count; i++) {
		moveRule rule = current_state->definition->rules[i];
		if (strcmp(rule.src_pile, src->pile_class->name) == 0 && strcmp(rule.dest_pile, dest->pile_class->name) == 0) {
			if (flag_checker(rule.flags, mov, src, dest, moving_card))
				return true;
		}
	}
	return false;
}

void auto_moves(Game_state *current_state, Turn *current_turn) {
	bool moved = true;
	while (moved) {
		moved = false;
		MoveList move_list = get_valid_moves(current_state, true);
		if (move_list.count <= 0) {
			moved = false;
		} else {
			Move mv = move_list.moves[move_list.count - 1].move;
			current_state->move = mv;
			current_state->move.is_auto = true;
			if (current_turn != NULL) {
				current_turn->sub_moves[current_turn->count] = current_state->move;
				current_turn->count++;
			}
			do_move(current_state);
			// Isso conta como uma jogada para entrar no historial?
			moved = true;
		}
	}
	set_move(current_state);
}

MoveList get_valid_moves(Game_state *current_state, bool is_auto) { // set the moveList with all possible moves
	MoveList move_list;
	move_list.count = 0;

	moveRule *rules = current_state->definition->rules;
	int rules_count = current_state->definition->rule_count;

	GameCommand cmd;

	bool flag = true;

	for (int i = 0; i < rules_count && flag; i++) {
		moveRule current_rule = rules[i];
		if (is_auto == current_rule.is_auto) {
			for (int j = 0; j < current_state->pile_count && flag; j++) {
				if (current_state->table_piles[j]->num_cards > 0 &&
					(strcmp(current_state->table_piles[j]->pile_class->name, current_rule.src_pile) == 0)) {
					cmd.src = j + 1;
					for (int k = 0; k < current_state->table_piles[j]->num_cards && flag; k++) {
						cmd.card_index_input = k + 1;
						for (int l = 0; l < current_state->pile_count && flag; l++) {
							if (strcmp(current_state->table_piles[l]->pile_class->name, current_rule.dest_pile) == 0) {
								cmd.dest = l + 1;
								fill_move(current_state, &cmd);
								if (is_move_valid(current_state)) {
									move_list.moves[move_list.count].move = current_state->move;
									move_list.moves[move_list.count].win_score = (-1);
									move_list.count++;
									if (is_auto == true && move_list.count == 1) {
										flag = false;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	set_move(current_state);

	return move_list;
}

void undo_move(Game_state *state) {
	Turn last_turn;
	if (is_history_empty(state->history)) {
		return;
	}
	pop_history(state->history, &last_turn);
	for (int i = last_turn.count - 1; i >= 0; i--) {
		Move m = last_turn.sub_moves[i];
		state->move.src_pile = m.dest_pile;
		state->move.dest_pile = m.src_pile;
		state->move.card_count = m.card_count;
		Pile *source = state->table_piles[m.dest_pile];
		state->move.column_out = (m.column_out == 0) ? 0 : source->num_cards - m.card_count;
		state->move.is_auto = true;
		do_move(state);
	}
}
