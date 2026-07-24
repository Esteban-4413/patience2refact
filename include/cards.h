#include <stdio.h>

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
	int rank;
	int suit;
	struct card *next_card;
} card;
