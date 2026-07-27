#include "../include/main.h"

int main() {

	// bool playing = true;
	// while (playing) {
	// 	int ch = getchar();
	// 	if (ch == 'q')
	// 		playing = false;
	// 	else if (ch == 't') {
	// 		print_deck_arr(deck);
	// 		printf("\n");
	// 	} else if (ch == 's') {
	// 		shuffle_pile(deck);
	// 		print_deck_arr(deck);
	// 	}
	// }
	Pile p;
	Card *deck = fill_deck(&p);
	run_cli(deck);

	return 0;
}
