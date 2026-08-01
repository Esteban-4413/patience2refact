#ifndef CLI_H
#define CLI_H
#include "display.h"
#include "game_state.h"
#include "loader.h"
#include "move.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *buffer;
	size_t buffer_length;
	ssize_t input_length;
} InputBuffer;

typedef enum {
	CMD_LOAD,
	CMD_PRINT,
	CMD_SHUFFLE,
	CMD_QUIT,
	CMD_MOVE,
	CMD_UNDO,
	CMD_UNRECOGNIZED
} CommandType;

typedef struct GameCommand{
	CommandType type;
	char arg[50];
	int src;
	int dest;
	int card_index_input;
} GameCommand;


InputBuffer *new_input_buffer();
void print_deck_arr(Card *deck_arr);
void print_prompt();
void read_input(InputBuffer *input_buffer);
void close_input_buffer(InputBuffer *input_buffer);
GameCommand parse_command(InputBuffer *input_buffer);
void run_cli();

#endif
