#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "cards.h"
#include "game_def.h"
#include "stack.h"
#include "undo.h"


typedef struct {
	GameDefinition *definition;

	Pile **table_piles;
	int pile_count;

	History *history;

} Game_state;


Card *fill_deck(Pile *pile);
void shuffle_pile(Card *deck);
void swap(Card *deck, int a, int b);


#endif
