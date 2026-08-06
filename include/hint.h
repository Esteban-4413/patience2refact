#ifndef HINT_H
#define HINT_H

#include "game_state.h"
#include "move.h"
#include "loader.h"
#include <string.h>

#define MAX_HINT_DEPTH 3

Move generate_hints(Game_state *current_state);

#endif
