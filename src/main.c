#include "../include/main.h"

int main() {

	bool playing = true;
	while (playing) {
		int ch = getchar();
		Pile p;
		Card *deck = fill_deck(&p);
		if (ch == 'q')
			playing = false;
		else if (ch == 't') {
			print_deck_arr(deck);
			printf("\n");
		} else if (ch == 's') {
			shuffle_pile(deck);
			print_deck_arr(deck);
		}
	}

	return 0;
}

void print_deck_arr(Card *deck_arr) {
	for (int i = 0; i < 52; i++) {
		printf(" |%d %d| ", deck_arr[i].suit, deck_arr[i].rank);
		if (i != 0 && i % 13 == 0)
			printf("\n");
	}
	printf("Done!\n");
}
