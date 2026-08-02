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
#include <stdbool.h>

bool save_game(Game_state *current_state);

#endif
