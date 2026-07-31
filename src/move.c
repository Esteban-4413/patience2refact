#include "../include/cli.h"
#include <stdint.h>

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
void fill_move(Game_state *current_state, GameCommand *cmd){
	// Flag f = current_state->move.is_valid;
	int src = cmd->src - 1;
	int dest = cmd->dest - 1;
	Pile *src_pile = current_state->table_piles[src];
	int column0;
	if (cmd->card_index == -1) {
		column0 = src_pile->num_cards - 1;
	} else {
		column0 = cmd->card_index - 1;
	}
	int cards_to_move = src_pile->num_cards - column0;
	if (cards_to_move <= 0 || column0 < 0) {
		printf("There is no card to move\n Please try again\n");
		return;
	}
	current_state->move.src_pile = src;
	current_state->move.dest_pile = dest;
	current_state->move.column_out = column0;
	current_state->move.card_count = cards_to_move;

	// switch (f) {
	// case SET:
	// 	if (strlen(cmd->arg) < 3) {
	// 		printf("Your command was inclomplete \n");
	// 		printf("The command has to be 'move <pile|column> <dest pile>' "
	// 			   "\n");
	// 		printf("Try again... \n");
	// 		cmd->type = CMD_UNRECOGNIZED;

	// 	} else {
	// 		char p[3];
	// 		int j = 0;
	// 		int i;
	// 		for (i = 0; (cmd->arg[i] != '\0') && (cmd->arg[i] != '|'); i++) {
	// 			p[j] = cmd->arg[i];
	// 			j++;
	// 		}
	// 		p[j] = '\n';
	// 		int pile = atoi(p) - 1;
	// 		// o input é numerado sempre de 1...
	// 		// mas na lógica  do jogo é numerado de 0...
	// 		char c[3];
	// 		j = 0;
	// 		for (i = i + 1; cmd->arg[i] != '\0'; i++) {
	// 			c[j] = cmd->arg[i];
	// 			j++;
	// 		}
	// 		c[j] = '\n';

	// 		int column = atoi(c) - 1;

	// 		current_state->move.src_pile = pile;
	// 		current_state->move.column_out = column;

	// 		current_state->move.card_count =
	// 			current_state->table_piles[pile]->num_cards - column;
	// 		// Ou seja, se o card_count <= 0
	// 		// o jogador selecionou uma coluna em que não existem
	// 		// cartas nessa pilha!
	// 		if (current_state->move.card_count <= 0) {
	// 			printf("There is no card in this position\n");
	// 			printf("Try again...\n");
	// 			set_move(current_state);
	// 		} else {
	// 			current_state->move.is_valid = WAIT;
	// 		}


	// 	}
	// 	break;
	// case WAIT:
	// 	if (strlen(cmd->arg) < 2) {
	// 		printf("Your command was inclomplete \n");
	// 		printf("The command has to be 'move pile|' \n");
	// 		printf("Try again... \n");
	// 	} else {
	// 		char p_dest[3];
	// 		int i;
	// 		int j = 0;
	// 		for (i = 0; (cmd->arg[i] != '\0') && (cmd->arg[i] != '|'); i++) {
	// 			p_dest[j] = cmd->arg[i];
	// 			j++;
	// 		}
	// 		p_dest[i] = '\0';

	// 		current_state->move.dest_pile = atoi(p_dest) - 1;
	// 		current_state->move.is_valid = VALID;
	// 		// Para fins de teste estou a considerar todo move como válido

	// 		// validate_move(); (TODO)
	// 	}
	// 	break;
	// case VALID:
	// 	move(current_state);
	// 	// add_history(); (TODO)
	// 	print_board(current_state);
	// 	set_move(current_state);
	// 	break;
	// case INVALID:
	// 	set_move(current_state);
	// 	break;
	// }
}

void move(Game_state *current_state){
    Move move = current_state->move;
    Pile *src_pile = current_state->table_piles[move.src_pile];
    Pile *dest_pile = current_state->table_piles[move.dest_pile];

    Card *ant = NULL;
    Card *head = peek_card_at(src_pile, 0); // Não tem como a pilha está vazia (já foi verificado)
    if (src_pile == NULL || src_pile->head == NULL) return; // não acontece!!
    if (move.column_out > 0) {
        ant = peek_card_at(src_pile, move.column_out - 1);
        head = peek_card_at(src_pile, move.column_out);
    }
    if (ant == NULL) { src_pile->head = NULL; }
    else { ant->next = NULL; }
    src_pile->num_cards -= move.card_count;

    if (dest_pile->head == NULL) { dest_pile->head = head; }
    else {
          Card *last_card = peek_card_at(dest_pile, dest_pile->num_cards - 1);
          if (last_card == NULL) return;
          if (last_card->next == NULL) { last_card->next = head; }
    }
    dest_pile->num_cards += move.card_count;
}

bool is_move_valid(Game_state *current_state) {
	Move mov = current_state->move;
	Pile *src = current_state->table_piles[mov.src_pile];
	Pile *dest = current_state->table_piles[mov.dest_pile];
	uint32_t flags = current_state->definition->rules->flags;
	Card *moving_card = peek_card_at(src, mov.column_out);
	if (flags & F_NONE) {
		return true;
	}
	if (flags & F_SEQUENCE) {
		return (mov.card_count);
	}
	return true;
}
