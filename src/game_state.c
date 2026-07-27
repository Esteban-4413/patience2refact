#include "../include/game_state.h"

Card *fill_deck(Pile *pile) {
	if (pile == NULL)
		return NULL;
	Card *deck_arr = malloc(52 * sizeof(*deck_arr));

	int i = 0;
	for (int suit = 0; suit <= SUIT_COUNT; suit++) {
		for (int rank = 0; rank <= RANK_COUNT; rank++) {
			deck_arr[i].rank = rank;
			deck_arr[i].suit = suit;
			i++;
		}
	}
	return deck_arr;
}

void shuffle_pile(Card *deck_arr) {
	if (deck_arr == NULL)
		return;
	for (int i = 0; i < 52; i++) {
		int idx = rand() % (i + 1);
		swap(deck_arr, idx, i);
	}
}


void swap(Card *deck_arr, int a, int b) {
	Card temp;
	temp.rank = deck_arr[a].rank;
	temp.suit = deck_arr[a].suit;

	deck_arr[a] = deck_arr[b];
	deck_arr[b] = temp;
}

Pile *arr2llist(Card **deck_arr, int num_cards) {
	Pile *deck_list = malloc(sizeof(Pile));
	deck_list->head = NULL;
	for (int i = num_cards - 1; i >= 0; i--) {
		push(deck_list, deck_arr[i]);
	}
	return deck_list;
}
