#include "../include/cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputBuffer *new_input_buffer() {
	InputBuffer *input_buffer = malloc(sizeof(*input_buffer));
	input_buffer->buffer = NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length = 0;
	return input_buffer;
}

void print_prompt() { printf("input > "); }

void read_input(InputBuffer *input_buffer) {
	ssize_t bytes_read =
		getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
	if (bytes_read <= 0) {
		printf("Error reading the input\n");
		exit(EXIT_FAILURE);
	}
	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer) {
	free(input_buffer->buffer);
	free(input_buffer);
}

GameCommand parse_command(InputBuffer *input_buffer) {
	GameCommand cmd;
	cmd.arg[0] = '\0';
	char keyword[20];
	if (sscanf(input_buffer->buffer, "%s", keyword) != 1) {
		cmd.type = CMD_UNRECOGNIZED;
		return cmd;
	}
	if (strcmp(keyword, "quit") == 0 || strcmp(keyword, "exit") == 0)
		cmd.type = CMD_QUIT;
	else if (strcmp(keyword, "print") == 0)
		cmd.type = CMD_PRINT;
	else if (strcmp(keyword, "shuffle") == 0)
		cmd.type = CMD_SHUFFLE;
	else if (strcmp(keyword, "load") == 0) {
		cmd.type = CMD_LOAD;
		sscanf(input_buffer->buffer, "%*s %s", cmd.arg);
	}
	else if (strcmp(keyword, "move") == 0){
	    cmd.type = CMD_MOVE;
		sscanf(input_buffer->buffer, "%*s %s", cmd.arg);
	} else
		cmd.type = CMD_UNRECOGNIZED;
	return cmd;
}

void run_cli() {
	InputBuffer *input_buffer = new_input_buffer();
	GameDefinition *current_def = NULL;
	Game_state *current_state = NULL;
	printf("Welcome to <name>. Type your command ('load' <file>, 'print', "
		   "'shuffle', 'quit' or 'move pile|column' .\n");
	while (true) {
		print_prompt();
		read_input(input_buffer);
		if (input_buffer->input_length == 0)
			continue;
		GameCommand cmd = parse_command(input_buffer);
		switch (cmd.type) {
		case CMD_QUIT:
			printf("Bye bye...\n");
			close_input_buffer(input_buffer);
			exit(EXIT_SUCCESS);
			break;
		case CMD_LOAD:
			if (strlen(cmd.arg) > 0) {
				printf("Loading your game: %s\n", cmd.arg);
				current_def = load_patience("paciencias");
			} else
				current_def = choose_patience("paciencias");
			if (current_def != NULL && current_def->game_name[0] != '\0') {
				printf("Loaded %s\n", current_def->game_name);
				current_state = build_game_state(current_def);
				if (current_state != NULL)
					printf("The cards have been shuffled and dealt\n");
				break;
			}
		case CMD_PRINT:
			if (current_state == NULL)
				printf("No game loaded yet. Type 'load' first\n");
			else
                print_board(current_state);
			break;

		case CMD_SHUFFLE:
			printf("Shuffling the cards..\n");
			printf("Shuffled!\n");
			printf("The deck is actually shuffled when you use 'load'\n");
			break;

		case CMD_MOVE:
            if (current_state == NULL){
                printf("No game loaded yet. Type 'load' first\n");
		    } else {
				if(strlen(cmd.arg) > 0){
				//Função que vai fazer o move e vai fazer print do novo board (TODO);

				// Se a pessoa digitar 'move 17!2' ele ainda retira
                // o valor da source pile, mas não o da column
                // Talvez seja melhor verificar que se não houver nenum '|'
                // no cmd.arg ele é logo classificado como CMD_UNRECOGNIZED
				fill_move(current_state, &cmd);
				} else {
    				printf("Ups, I belive you forgot the argument \n");
    				printf("Try again!\n");
    				print_prompt();
				}
			}
            break;

		case CMD_UNRECOGNIZED:
			printf("Command unrecognized: '%s\n", input_buffer->buffer);
			break;
		}
	}
}
