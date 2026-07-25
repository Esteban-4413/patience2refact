#include <stdio.h>
#include <stdlib.h>
#define CARD_COUNT 52

typedef enum { SUIT_HEART, SUIT_SPADE, SUIT_CLUB, SUIT_DIAMOND } Suit;

typedef enum {
	RANK_ACE,
	RANK_2,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
	RANK_8,
	RANK_9,
	RANK_10,
	RANK_JACK,
	RANK_QUEEN,
	RANK_KING
} Rank;

typedef struct card {
	Rank rank;
	Suit suit;
	struct card *next;
} card;


typedef struct pile {
	card *head;
	int num_cards;
} pile;

int is_black(card *c);


int is_red(card *c);

int is_alternate_color(card *first, card *second);

int is_in_sequence(card *higher, card *lower);

int can_be_placed_bottom(card *parent, card *child);
int is_same_suit(card *first, card *second);


card *make_card(Suit x, Rank y);
