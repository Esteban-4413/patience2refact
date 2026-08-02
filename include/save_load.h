#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "game_state.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool save_game(Game_state *current_state);

void translate_suit(char *char_suit, Suit suit);

void translate_rank(char *char_rank, Rank rank);
#endif
