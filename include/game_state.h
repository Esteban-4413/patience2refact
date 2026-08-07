#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "cards.h"
#include "game_def.h"
#include "stack.h"
#include "undo.h"
#include <string.h>
#include <time.h>

typedef struct GameStats {
	int score;
	int moves_count;
	time_t start_time;

	int hint_src_pile;
	int hint_dest_pile;
	int hint_card_count;
} GameStats;

typedef struct {
	GameDefinition *definition;

	Pile **table_piles;
	int pile_count;

	History *history;

	Move move;
	GameStats *stats;

} Game_state;


void fill_deck(Pile *pile);
void shuffle_pile(Pile *p);
void swap(Card *deck_arr, int a, int b);
Pile *arr2llist(Card **deck, int num_cards);

bool win_codition(Game_state *current_state);
void update_score(Game_state *state, int src_idx, int dest_idx, int card_count);
#endif
