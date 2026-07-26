#ifndef PARSER_H
#define PARSER_H

#include "../include/game_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t parse_move_flags(char *flags_str);

GameDefinition *incializa_estado(void);

void cmd_mov(char *buffer, GameDefinition *def, bool is_auto);

void cmd_tipo(char *buffer, GameDefinition *def);
#endif
