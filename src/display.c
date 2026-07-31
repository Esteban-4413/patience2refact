#include "../include/display.h"

void print_board(Game_state *current_state){
    int column = 0;

    // Print the identification of the piles
    for (int i = 0; i <= current_state->pile_count; i++){
        if (i == 0) printf( "      ");
        else printf(" Pilha %2d(%2d): ", i , current_state->table_piles[i-1]->num_cards);
    }
    printf("\n");

    // Get the number of columns needed to print all piles in the current game state.
    int max = 0;
    for(int i = 0; i < current_state->pile_count; i++) {
        int num_cards = current_state->table_piles[i]->num_cards;
        if (num_cards > max) max = num_cards;
    }

    // Print the cards of each pile
    while(column < max) // The limit is the num_cards of the bigest pile.
    {
        printf(" %2d ", column + 1);

        for(int i = 0; i < current_state->pile_count; i++){
            Pile *current_pile= current_state->table_piles[i];
            if(current_pile != NULL){
                Card *card = peek_card_at(current_pile, current_pile->num_cards -1 -column);
                if (card != NULL){
                    printf("    [%2d %2d]    ", card->rank, card->suit);
                }
                else if (column + 1 >=current_pile->num_cards){
                    printf("    [%2s %2s]    ", "-", "-");
                    // Só para fins de debugar, porque na prática quando as cartas da pilha acabarem,
                    // vai fazer print de espaços
                } else {
                    printf("    [%2s %2s]    ", " ", " ");
                }
            } else {
                printf("%9s","PILE");
                // Para debugar
            }


        }
        printf("\n");
        column++;
    }
}

void print_linked(Pile *pile) {
	if (pile == NULL) {
		printf("[pile is empty]\n");
		return;
	}
	Card *c = pile->head;
	if (c == NULL) {
		printf(" * \n");
		return;
	}
	while (c != NULL) {
		printf("[%d %d] -> ", c->rank, c->suit);
		c = c->next;
	}
	printf("* \n");
}

void print_board_linked(Game_state *state) {
	printf("---\t---\t---\t---\n");
	for (int i = 0; i < state->pile_count; i++) {
		printf("Pile (%d): ", i + 1);
		print_linked(state->table_piles[i]);
	}
}
