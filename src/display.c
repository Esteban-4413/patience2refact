#include "../include/display.h"

void print_board(Game_state *current_state){
    int column = 0;

    // Print the identification of the piles
    for (int i = 0; i <= current_state->pile_count; i++){
        if (i == 0) printf( "      ");
        else printf(" Pilha %2d(%2d): ", i , current_state->table_piles[i-1]->num_cards);
    }
    printf("\n");


    //
    while(column < 10){
        printf(" %2d ", column + 1);

        for(int i = 0; i < current_state->pile_count; i++){
            Pile *current_pile= current_state->table_piles[i];
            if(current_pile != NULL){
                Card *card = peek_card_at(current_pile, column);
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
