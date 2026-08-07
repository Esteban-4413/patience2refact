#ifndef HINT_H
#define HINT_H

#include "game_state.h"
#include "move.h"
#include "loader.h"
#include <string.h>

#define MAX_HINT_DEPTH 3

MoveList generate_hints(Game_state *current_state);

Game_state clone_state(Game_state *current_state);

void free_state(Game_state *state);

void swap_hint(MoveEvaluation *moves, int a, int b);

int partition(MoveEvaluation *moves, int N);

void quickSort(MoveEvaluation *moves, int N);
void print_move_list(const MoveList *move_list);
#endif
