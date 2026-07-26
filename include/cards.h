#include <stdint.h>
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
} Card;


typedef struct pile {
	Card *head;
	int num_cards;
	// uint8_t rules;
} Pile;

int is_black(Card *c);


int is_red(Card *c);

int is_alternate_color(Card *first, Card *second);

int is_in_sequence(Card *higher, Card *lower);

int can_be_placed_bottom(Card *parent, Card *child);
int is_same_suit(Card *first, Card *second);


Card *make_card(Suit x, Rank y);
