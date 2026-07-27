#include "../include/game_state.h"

void fill_deck(Pile *pile) {
	if (pile == NULL)
		return;
	for (int suit = 0; suit < SUIT_COUNT - 1; suit++) {
		for (int rank = 1; rank <= RANK_COUNT; rank++) {
			Card *new_card = make_card(rank, suit);
			push(pile, new_card);
		}
	}
}

void shuffle_pile(Pile *p) {
	if (p == NULL)
		return;
	int n = p->num_cards;
	Card **arr = malloc(n * sizeof(Card *));
	Card *curr = p->head;
	for (int i = 0; i < n; i++) {
		arr[i] = curr;
		curr = curr->next;
	}
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		Card *temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
	p->head = arr[0];
	curr = p->head;
	for (int i = 1; i < n; i++) {
		curr->next = arr[i];
		curr = curr->next;
	}
	curr->next = NULL;
	free(arr);
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

void print_deck_arr(Card *deck_arr) {
	for (int i = 0; i < 52; i++) {
		printf(" |%d %d| ", deck_arr[i].suit, deck_arr[i].rank);
		if (i != 0 && i % 13 == 0)
			printf("\n");
	}
	printf("\nDone!\n");
}
