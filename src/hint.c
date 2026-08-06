#include "../include/hint.h"


Move generate_hints(Game_state *current_state){

    MoveList root_moves = get_valid_moves(current_state, false);

    return (root_moves.moves[0].move);
}
