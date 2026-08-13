#include "../include/cards.h"

int is_black(Card *c) { return c->suit == SUIT_SPADE || c->suit == SUIT_CLUB; }

int is_red(Card *c) { return c->suit == SUIT_DIAMOND || c->suit == SUIT_HEART; }

int is_alternate_color(Card *first, Card *second) { return is_black(first) != is_black(second); }

int is_in_sequence(Card *higher, Card *lower) { return higher->rank == lower->rank + 1; }

int can_be_placed_bottom(Card *parent, Card *child) {
	return is_alternate_color(parent, child) && is_in_sequence(parent, child);
}

int is_same_suit(Card *first, Card *second) { return first->suit == second->suit; }

int can_be_placed_on_foundation(Card *parent, Card *child) {
	return is_same_suit(parent, child) && is_in_sequence(parent, child);
}

Card *make_card(Suit x, Rank y) {
	Card *c = malloc(sizeof(Card));
	c->rank = y;
	c->suit = x;
	c->next = NULL;
	return c;
}

bool is_seq_ascending(Card *parent, int n) {
	for (int i = 0; i < n - 1; i++) {
		Card *child = parent->next;
		if (child == NULL)
			return false;
		if (parent->rank != child->rank + 1)
			return false;
		parent = child;
	}
	return true;
}

bool is_seq_descending(Card *parent, int n) {
	for (int i = 0; i < n - 1; i++) {
		Card *child = parent->next;
		if (child == NULL)
			return false;
		if (parent->rank != child->rank - 1)
			return false;
		parent = child;
	}
	return true;
}
bool is_seq_same_suit(Card *parent, int n) {
	for (int i = 0; i < n - 1; i++) {
		Card *child = parent->next;
		if (child == NULL)
			return false;
		if (parent->suit != child->suit)
			return false;
		parent = child;
	}
	return true;
}

bool is_seq_alt_suit(Card *parent, int n) {
	for (int i = 0; i < n - 1; i++) {
		Card *child = parent->next;
		if (child == NULL)
			return false;
		if (parent->suit == child->suit)
			return false;
		parent = child;
	}
	return true;
}

bool is_seq_same_color(Card *parent, int n) {
	for (int i = 0; i < n - 1; i++) {
		Card *child = parent->next;
		if (child == NULL)
			return false;
		if (is_alternate_color(parent, child))
			return false;
		parent = child;
	}
	return true;
}

bool is_seq_alt_color(Card *parent, int n) {
	for (int i = 0; i < n - 1; i++) {
		Card *child = parent->next;
		if (child == NULL)
			return false;
		if (!is_alternate_color(parent, child))
			return false;
		parent = child;
	}
	return true;
}
