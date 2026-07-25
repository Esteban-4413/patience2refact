#include "stack.h"

typedef struct {
	pile **piles;
	int pile_count;
} game_state;


void fill_deck(pile *pile);
void shuffle_pile(pile *pile);
