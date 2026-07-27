#include "../include/main.h"

int main(){

    bool playing = true;
    while(playing){
        int ch = getchar();
        if (ch == 'q') playing = false;
        else if (ch == 't'){
            // testar a inicialização do baralho
            Pile p;
            Card *deck = fill_deck(&p);

            shuffle_pile(deck);
            print_deck(deck);

        }
    }

    return 0;

}

void print_deck(Card *deck){
    for (int i = 0; i < 52; i++){
        printf("|%d %d|", deck[i].suit, deck[i].rank);
        if (i != 0 && i%             13 == 0) printf("\n");
        printf(" ");
    }
    printf("\n Done!");
}
