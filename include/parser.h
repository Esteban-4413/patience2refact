#ifndef PARSER_H
#define PARSER_H

#include "../include/game_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t parse_move_flags(char *flags_str);

GameDefinition *init_state(void);

void cmd_mov(char *buffer, GameDefinition *def, bool is_auto);

void cmd_class(char *buffer, GameDefinition *def);

void cmd_init(char *buffer, GameDefinition *est);

void cmd_win(char *buffer, GameDefinition *def);

void what_command(char *comando, char *buffer, GameDefinition *def);

int process_line(char *buffer, GameDefinition *def);

GameDefinition *load_patience(char *ficheiro);
#endif
