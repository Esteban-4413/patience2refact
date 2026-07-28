#ifndef MOVE_H
#define MOVE_H
#include "game_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct GameCommand GameCommand;

void print_move(Game_state *state);
void fill_move(Game_state *current_state, GameCommand *cmd);

#endif
