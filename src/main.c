#include "../include/main.h"

int main(){

    return 0;

    bool playing;
    while(playing){
        int ch = getchar();
        if (ch == 'q') playing = false;
        else if (ch == 't'){
            // testar a inicialização do baralho
            Pile p;
            Card *deck = fill_deck(&p);

            print_deck(deck);

        }
    }
}

void print_deck(Card *deck){
    for (int i = 0; i < 52; i++){
        printf("|%d %d| ", deck[i].suit, deck[i].rank);
        if(i/13 == 0) printf("\n");
    }
}
