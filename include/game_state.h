#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "game_def.h"
#include "stack.h"
#include "undo.h"

typedef enum {
	SET = (-2), // Move set by default
	INVALID,	// The move was verified and classified as invalid
	WAIT,		// Contains information about the source pile/exit of the cards
	VALID		// The move was verified and classified as valid
} Flag;


typedef struct {
	GameDefinition *definition;

	Pile **table_piles;
	int pile_count;

	History *history;

} Game_state;


void fill_deck(Pile *pile);
void shuffle_pile(Pile *pile);

#endif
