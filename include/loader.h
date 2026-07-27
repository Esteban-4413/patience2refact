#ifndef LOADER_H
#define LOADER_H
#include "game_def.h"
#include "game_state.h"
#include "parser.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

GameDefinition *choose_patience(char *folder);

GameDefinition *load_option(char *folder, char files[][256], int patience);

int list_options(DIR *d, char files[][256]);

int input_patience(int count);

Game_state *build_game_state(GameDefinition *def);
#endif
