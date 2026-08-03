#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#ifdef _WIN32
    #include <direct.h> // Required for mkdir on Windows
    // There is no problem if it says: 'direct.h' file not found
    // because we are not working in Windows
#else
    #include <sys/stat.h> // Required for mkdir on Linux/macOS
    #include <sys/types.h>
#endif

#include "game_state.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


bool save_game(Game_state *current_state);

void translate_suit(char *char_suit, Suit suit);

void translate_rank(char *char_rank, Rank rank);

Game_state *load_game(char *save_file);
#endif
