#ifndef HINT_H
#define HINT_H

#include "game_state.h"
#include "loader.h"
#include "move.h"
#include <string.h>

#define MAX_HINT_DEPTH 3

int evaluate_score(Game_state *state, int src_idx, int dest_idx, int card_count);

int update_score(Game_state *state, int src_idx, int dest_idx, int card_count);

MoveList generate_hints(Game_state *current_state);

void print_move_list(const MoveList *move_list);
#endif
