#include "../include/cli.h"
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

void print_deck_arr(Card *deck_arr) {
	for (int i = 0; i < 52; i++) {
		printf(" |%d %d| ", deck_arr[i].suit, deck_arr[i].rank);
		if (i != 0 && i % 13 == 0)
			printf("\n");
	}
	printf("Done!\n");
}

GameCommand parse_command(InputBuffer *input_buffer) {
	GameCommand cmd;
	char keyword[20];
	if (sscanf(input_buffer->buffer, "%s", keyword) != 1) {
		cmd.type = CMD_UNRECOGNIZED;
		return cmd;
	}
	if (strcmp(keyword, "quit") == 0 || strcmp(keyword, "exit") == 0)
		cmd.type = CMD_QUIT;
	else if (strcmp(keyword, "print") == 0)
		cmd.type = CMD_PRINT;

	else if (strcmp(keyword, "print") == 0)
		cmd.type = CMD_PRINT;
	else if (strcmp(keyword, "shuffle") == 0)
		cmd.type = CMD_SHUFFLE;
	else if (strcmp(keyword, "load") == 0) {
		cmd.type = CMD_LOAD;
		sscanf(input_buffer->buffer, "%*s %s", cmd.arg);
	} else
		cmd.type = CMD_UNRECOGNIZED;
	return cmd;
}

void run_cli(Card *deck_arr) {
	InputBuffer *input_buffer = new_input_buffer();
	GameDefinition *current_game = NULL;
	printf("Welcome to <name>. Type your command ('load' <file>, 'print', "
		   "'shuffle' or 'quit'.\n");
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
			printf("Loading your game: %s\n", cmd.arg);
			current_game = load_patience(cmd.arg);
			if (current_game->game_name[0] != '\0')
				printf("Game loaded hehe\n");
			break;
		case CMD_PRINT:
			if (current_game == NULL)
				printf("First load your game\n");
			else {
				printf("Here is your game");

				print_deck_arr(deck_arr);
			}
			break;

		case CMD_SHUFFLE:
			printf("Shuffling the cards..\n");
			shuffle_pile(deck_arr);
			printf("Shuffled\n");
			print_deck_arr(deck_arr);
			break;
		case CMD_UNRECOGNIZED:
			printf("Command unrecognized: '%s\n", input_buffer->buffer);
			break;
		}
	}
}
