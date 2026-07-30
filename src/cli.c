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
	cmd.card_index = -1;
	cmd.dest = -1;
	cmd.src = -1;
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
	} else if (strcmp(keyword, "mv") == 0) {
		cmd.type = CMD_MOVE;
		if (sscanf(input_buffer->buffer, "%*s %d|%d %d", &cmd.src,
				   &cmd.card_index, &cmd.dest) == 3)
			;
		else if (sscanf(input_buffer->buffer, "%*s %d %d", &cmd.src, &cmd.dest))
			cmd.card_index = -1;
		else {
			cmd.src = -1;
		}
	} else
		cmd.type = CMD_UNRECOGNIZED;
	return cmd;
}

void run_cli() {
	InputBuffer *input_buffer = new_input_buffer();
	GameDefinition *current_def = NULL;
	Game_state *current_state = NULL;
	printf("Welcome to <name>. Type your command ('load' <file>, 'print', "
		   "'shuffle', 'quit' or 'mv pile|column' .\n");
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
				char path[80];
				if (strstr(cmd.arg, ".paciencia") != NULL)
					sprintf(path, "paciencias/%s", cmd.arg);
				else
					sprintf(path, "paciencias/%s.paciencia", cmd.arg);
				printf("Loading your game: %s\n", cmd.arg);
				current_def = load_patience(path);
			} else
				current_def = choose_patience("paciencias");
			if (current_def != NULL && current_def->game_name[0] != '\0') {
				printf("Loaded %s\n", current_def->game_name);
				current_state = build_game_state(current_def);
				if (current_state != NULL)
					printf("The cards have been shuffled and dealt\n");
			} else
				printf("Failed to load the game. Check the filename and try "
					   "again!\n");
			break;
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
			if (current_state == NULL) {
				printf("No game loaded yet. Type 'load' first\n");
			} else if (cmd.src == -1 || cmd.dest == -1) {
				printf("Invalid format! Use 'mv <src> <dest>' or 'mv "
					   "<src>|<index> <dest>'\n");
				printf("Examples:\n");
				printf("  mv 1 2   (Moves the top card from pile 1 to pile "
					   "2)\n");
				printf("  mv 1|3 2 (Moves a sequence starting from index 3 "
					   "in pile 1 to pile 2)\n");
			} else {
				fill_move(current_state, &cmd);

				if (cmd.card_index != -1) {
					printf("making the move of the sequence %d>>top from the "
						   "pile %d "
						   "to the pile %d\n",
						   cmd.card_index, cmd.src, cmd.dest);
				} else {
					printf("Moving the card from the top of the pile %d to the "
						   "pile %d\n",
						   cmd.src, cmd.dest);
					move(current_state);
				}
				move(current_state);
				print_board(current_state);
			}
			break;

		case CMD_UNRECOGNIZED:
			printf("Command unrecognized: '%s'\n", input_buffer->buffer);
			break;
		}
	}
}
