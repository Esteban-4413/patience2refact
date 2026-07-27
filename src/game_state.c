#include "../include/game_state.h"

Card *fill_deck(Pile *pile) {
	if (pile == NULL)
		return NULL;
	Card *deck = malloc(52 * sizeof(*deck));
	for (int i = 0; i <= 52; i++) {
		for (int rank = 0; rank <= RANK_COUNT; rank++) {
			for (int suit = 0; suit <= SUIT_COUNT; suit++) {
				deck[i].rank = rank;
				deck[i].suit = suit;
			}
		}
	}
	return deck;
}

void shuffle_pile(Card *deck) { return; }
