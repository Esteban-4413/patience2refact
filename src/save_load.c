#include "../include/save_load.h"

bool save_game(Game_state *current_state){
    #ifdef _WIN32
        mkdir("saves");
    #else
        mkdir("saves", S_IRWXU | S_IRWXG | S_IRWXO);
    #endif
    // S_IRWXU = Read, Write, Execute for User
    return true;
}
