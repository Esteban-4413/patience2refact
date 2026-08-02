#include "../include/move.h"
#include "../include/cli.h"

void print_move(Game_state *state){
    Move m = state->move;
	printf("Pile src: %d | Column: %d | Pile dest: %d | num: %d | Flag: %d"
		   "\n",
		   m.src_pile, m.column_out, m.dest_pile, m.card_count,
		   m.is_move_valid);
}

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
	int n = mov.card_count;
	Card *moving_card = peek_card_at(src, mov.column_out);

	if (flags & F_NONE) {
		return true;
	}
	if (mov.card_count > 1) {
		if (!(flags & F_SEQUENCE))
			return false;
		if (flags & F_DESCENDING) {
			if (!is_seq_descending(src->head, n))
				return false;
		}
		if (!(flags & F_ASCENDING)) {
			if (!is_seq_ascending(src->head, n))
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
		if (moving_card->rank != RANK_ACE)
			return false;
	}
	if (flags & F_BOTTOM_ACE) {
		if (src->head->rank != RANK_ACE)
			return false;
	}
	if (flags & F_BOTTOM_KING) {
		if (src->head->rank != RANK_KING)
			return false;
	}
	if (flags & F_TOP_KING) {
		if (moving_card->rank != RANK_KING)
			return false;
	}
	return true;
}

void auto_moves(Game_state *current_state){
    GameDefinition *game_def = current_state->definition;
    int moves_count = game_def->rule_count;
    GameCommand cmd;
    bool moved = true;

   while (moved){
       moved = false;
       for (int i = 0; i < moves_count && !moved; i++){ // Para cada MoveAUTO
           if(game_def->rules[i].is_auto){
               MoveRule move_rule = game_def->rules[i];

               for(int j = 0; (strcmp(current_state->table_piles[j]->pile_class->name, move_rule.src_pile) == 0) &&
                        (j < current_state->pile_count) && !moved; j++){
                   cmd.src = j + 1;
                   for(int k = 0; (k < current_state->table_piles[i]->num_cards) && !moved; k++){
                       cmd.card_index_input = k + 1;
                       for(int l = 0; (strcmp(current_state->table_piles[l]->pile_class->name, move_rule.dest_pile) == 0) &&
                                (l < current_state->pile_count) && !moved; l++){
                           cmd.dest = l + 1;
                           fill_move(current_state, &cmd);
                           if(is_move_valid(current_state)){
                               move(current_state);
                               // Isso conta como uma jogada para entrar no historial?
                               moved = true;
                           }
                       }
                   }

               }

           }
       }

   }
   set_move(current_state);
}
