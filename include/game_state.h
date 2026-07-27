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
void shuffle_pile(Card *deck_arr);
void swap(Card *deck_arr, int a, int b);
Pile *arr2llist(Card **deck, int num_cards);


#endif
