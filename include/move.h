#ifndef MOVE_H
#define MOVE_H
#include "game_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdbool.h>
//
#define MAX_MOVES_PER_STATE 64

typedef struct MoveEvaluation{
    Move move;
    int win_score;
}MoveEvaluation;

typedef struct MoveList{
    MoveEvaluation moves[MAX_MOVES_PER_STATE];
    int count;
}MoveList;


typedef struct GameCommand GameCommand;

MoveList get_valid_moves(Game_state *current_state, bool is_auto);

bool is_duplicate_move(MoveList *move_list, Move move);

void print_move(Game_state *state);

bool fill_move(Game_state *current_state, GameCommand *cmd);

void do_move(Game_state *current_state);

bool is_move_valid(Game_state *current_state);

void auto_moves(Game_state *current_state, Turn *current_turn);

void undo_move(Game_state *state);
#endif
