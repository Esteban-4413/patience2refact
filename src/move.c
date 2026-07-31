#include "../include/move.h"
#include "../include/cli.h"

void print_move(Game_state *state){
    Move m = state->move;
	printf("Pile src: %d | Column: %d | Pile dest: %d | num: %d | Flag: %d"
		   "\n",
		   m.src_pile, m.column_out, m.dest_pile, m.card_count,
		   m.is_move_valid);
}

// fill move podia devolver um bool para sabbermos se todas as informações foram
// bem colocadas
// tirei os cases switch para esta função ficar mais minimalista, depois para a
// interface gráfica criamos uma função que fique encarregada destas flags.
bool fill_move(Game_state *current_state, GameCommand *cmd) {
	int src = cmd->src - 1;
	int dest = cmd->dest - 1;
	Pile *src_pile = current_state->table_piles[src];
	int column_input =
		(cmd->card_index_input == -1) ? 0 : cmd->card_index_input;

	int cards_to_move = src_pile->num_cards - column_input;
	if (cards_to_move <= 0 || column_input < 0) {
		printf("There is no card to move\n Please try again\n");
		return false;
	}
	current_state->move.src_pile = src;
	current_state->move.dest_pile = dest;
	current_state->move.column_out =
		(column_input == 0) ? column_input : src_pile->num_cards - column_input;
	current_state->move.card_count =
		(column_input == 0) ? 1 : src_pile->num_cards - column_input + 1;
	return true;
}

void move(Game_state *current_state){
    Move move = current_state->move;
    Pile *src_pile = current_state->table_piles[move.src_pile];
    Pile *dest_pile = current_state->table_piles[move.dest_pile];

    Card *src_head = src_pile->head;
    int index = move.column_out;
	// Card *dest_head = dest_pile->head;
	Card *card = peek_card_at(src_pile, index);

    src_pile->head = card->next;
    card->next = dest_pile->head;
    dest_pile->head = src_head;

    src_pile->num_cards -= move.card_count;
    dest_pile->num_cards += move.card_count;
}

bool is_move_valid(Game_state *current_state) {
	Move mov = current_state->move;
	Pile *src = current_state->table_piles[mov.src_pile];
	Pile *dest = current_state->table_piles[mov.dest_pile];
	uint32_t flags = current_state->definition->rules->flags;
	// Card *moving_card = peek_card_at(src, mov.column_out);

	if (flags & F_NONE) {
		return true;
	}
	if (mov.card_count > 1) {
		if (!(flags & F_SEQUENCE))
			return false;
		if (flags & F_DESCENDING) {
			if (!is_seq_ascending(src->head, mov.card_count))
				return false;
		}
		if (!(flags & F_ASCENDING)) {
			if (!is_seq_descending(src->head, mov.card_count))
				return false;
		}
		if (flags & F_SUIT_SAME_SEQ) {
			if (!is_seq_same_suit(src->head, mov.card_count))
				return false;
		}
	}
	if (dest->head == NULL) {
		return true;
	}
	if (flags & F_VAL_LOWER) {
		if (src->head->rank != dest->head->rank + 1)
			return false;
	}
	if (flags & F_VAL_HIGHER) {
		if (src->head->rank != dest->head->rank - 1)
			return false;
	}
	if (flags & F_VAL_ADJACENT) {
		if (abs((int)src->head->rank - (int)dest->head->rank) != 1)
			return false;
	}
	return true;
}
