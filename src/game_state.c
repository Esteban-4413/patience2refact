#include "../include/game_state.h"

Card *fill_deck(Pile *pile) {
	if (pile == NULL)
		return NULL;
	Card *deck = malloc(52 * sizeof(*deck));

	int i = 0;
	for (int suit = 0; suit <= SUIT_COUNT; suit++) {
		for (int rank = 0; rank <= RANK_COUNT; rank++) {
			deck[i].rank = rank;
			deck[i].suit = suit;
			i++;
		}
	}
	return deck;
}

void shuffle_pile(Card *deck) {
    if (deck == NULL) return;
    for(int i = 0; i < 52; i++) {
        int idx = rand() % (i+1);
        swap(deck, idx, i);
    }
}


void swap(Card *deck, int a, int b){
    Card temp;
    temp.rank= deck[a].rank;
    temp.suit=deck[a].suit;

    deck[a] = deck[b];
    deck[b] = temp;
}
