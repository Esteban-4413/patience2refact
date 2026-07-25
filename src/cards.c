#include "../include/cards.h"

int is_black(card *c) { return c->suit == SUIT_SPADE || c->suit == SUIT_CLUB; }

int is_alternate_color(card *first, card *second) {
	return is_black(first) != is_black(second);
}

int is_in_sequence(card *higher, card *lower) {
	return higher->rank == lower->rank + 1;
}

int is_same_suit(card *first, card *second) {
	return first->suit == second->suit;
}

card *make_card(Suit x, Rank y) {
	card *c = malloc(sizeof(struct card));
	c->rank = y;
	c->suit = x;
	c->next = NULL;
	return c;
}
