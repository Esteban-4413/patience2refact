#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "cards.h"
#include "game_def.h"
#include "stack.h"
#include "undo.h"
#include <string.h>


typedef struct {
	GameDefinition *definition;

	Pile **table_piles;
	int pile_count;

	History *history;

	Move move;

} Game_state;


void fill_deck(Pile *pile);
void shuffle_pile(Pile *p);
void swap(Card *deck_arr, int a, int b);
Pile *arr2llist(Card **deck, int num_cards);

int win_codition(Game_state *current_state);
#endif
