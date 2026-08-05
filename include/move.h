#ifndef MOVE_H
#define MOVE_H
#include "game_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdbool.h>

typedef struct GameCommand GameCommand;

void print_move(Game_state *state);

bool fill_move(Game_state *current_state, GameCommand *cmd);

void do_move(Game_state *current_state);

bool is_move_valid(Game_state *current_state);

void auto_moves(Game_state *current_state, Turn *current_turn);

void undo_move(Game_state *state);
#endif
