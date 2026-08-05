#ifndef HINT_H
#define HINT_H

#include "game_state.h"
#include "move.h"
#include "loader.h"
#include <string.h>

#define MAX_MOVES_PER_STATE 64
#define MAX_HINT_DEPTH 3

typedef struct MoveEvaluation{
    Move move;
    int best_score;
}MoveEvaluation;

typedef struct MoveList{
    MoveEvaluation moves[MAX_MOVES_PER_STATE];
    int count;
}MoveList;

typedef struct StackNode {
    Game_state *state;
    MoveList valid_moves;
    int current_move_idx;
    int best_score;
}StackNode;

typedef struct {
    StackNode nodes[MAX_HINT_DEPTH + 1];
    int top;
}HintStack;

MoveList get_valid_moves(Game_state *current_state);

void push_hint (HintStack *stack, StackNode node);
StackNode pop_hint(HintStack *stack);

#endif
