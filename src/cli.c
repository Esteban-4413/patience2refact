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
	ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
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
	cmd.card_index_input = -1;
	cmd.dest = -1;
	cmd.src = -1;
	char keyword[20];
	if (sscanf(input_buffer->buffer, "%s", keyword) != 1) {
		cmd.type = CMD_UNRECOGNIZED;
		return cmd;
	}
	if (strcmp(keyword, "quit") == 0 || strcmp(keyword, "q") == 0)
		cmd.type = CMD_QUIT;
	else if (strcmp(keyword, "print") == 0 || strcmp(keyword, "p") == 0)
		cmd.type = CMD_PRINT;
	else if (strcmp(keyword, "shuffle") == 0)
		cmd.type = CMD_SHUFFLE;
	else if (strcmp(keyword, "load") == 0) {
		cmd.type = CMD_LOAD;
		sscanf(input_buffer->buffer, "%*s %s", cmd.arg);
	} else if (strcmp(keyword, "mv") == 0) {
		cmd.type = CMD_MOVE;
		if (sscanf(input_buffer->buffer, "%*s %d|%d %d", &cmd.src, &cmd.card_index_input, &cmd.dest) == 3)
			;
		else if (sscanf(input_buffer->buffer, "%*s %d %d", &cmd.src, &cmd.dest))
			cmd.card_index_input = -1;
		else {
			cmd.src = -1;
		}
	} else if (strcmp(keyword, "undo") == 0)
		cmd.type = CMD_UNDO;
	else if (strcmp(keyword, "save") == 0) {
		cmd.type = CMD_SAVE;
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
			} else {
				DIR *d = opendir("saves");
				if (d == NULL) {
					current_def = choose_patience(&current_state, "paciencias");
					if (current_def != NULL && current_def->game_name[0] != '\0') {
						printf("Loaded %s\n", current_def->game_name);
						current_state = build_game_state(current_def);
						if (current_state != NULL) {
							printf("The cards have been shuffled and dealt\n");
							print_board(current_state);
						}
					} else {
						printf("Failed to load the game. Check the filename and try "
							   "again!\n");
					}
				} else {
					printf("Here are the games to continue!\n");
					menu(&current_def, &current_state);
					print_board(current_state);
				}
			}

			break;
		case CMD_PRINT:
			if (current_state == NULL)
				printf("No game loaded yet. Type 'load' first\n");
			else
				print_board(current_state);
			print_board_linked(current_state);
			break;

		case CMD_SHUFFLE:
			printf("Shuffling the cards..\n");
			current_state = build_game_state(current_def);
			printf("Shuffled!\n");
			print_board(current_state);
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
				if (fill_move(current_state, &cmd)) {
					if (cmd.card_index_input != -1) {
						printf("making the move of the sequence %d>>top from the pile %d to the pile %d\n",
							   cmd.card_index_input, cmd.src, cmd.dest);
					} else {
						printf("Moving the card from the top of the pile %d to the pile %d\n", cmd.src, cmd.dest);
					}
					if (!is_move_valid(current_state)) {
						printf("Move is not valid\n Try somthing else!");
					} else {
						Turn current_turn;
						current_turn.count = 0;
						current_state->move.is_auto = false;
						current_turn.sub_moves[current_turn.count] = current_state->move;
						current_turn.count++;
						move(current_state);
						auto_moves(current_state, &current_turn);
						push_history(current_state->history, &current_turn);
					}
					print_board(current_state);
				}
			}
			break;
		case CMD_UNDO:
			if (current_state == NULL)
				printf("No game loaded yet.\n");
			else {
				undo_move(current_state);
				print_board(current_state);
			}
			break;
		case CMD_SAVE:
			if (current_state == NULL) {
				printf("No game loaded yet. Type 'load' first\n");
			} else {
				printf("Saving your game! \n");
				save_game(current_state);
			}
			break;
		case CMD_UNRECOGNIZED:
			printf("Command unrecognized: '%s'\n", input_buffer->buffer);
			break;
		}
	}
}

void menu(GameDefinition **current_def, Game_state **current_state) {
	int r = input_menu();
	if (r == 2) {
		*current_def = choose_patience(current_state, "paciencias");
		if (current_def != NULL && (*current_def)->game_name[0] != '\0') {
			printf("Loaded %s\n", (*current_def)->game_name);
			*current_state = build_game_state(*current_def);
			if (current_state != NULL) {
				printf("The cards have been shuffled and dealt\n");
				print_board(*current_state);
			}
		} else {
			printf("Failed to load the game. Check the filename and try "
				   "again!\n");
		}
	} else {
		*current_def = choose_patience(current_state, "saves");
		if (current_def == NULL && current_state != NULL){
		    printf("choose_patience done!\n");
		}
	}
}

int input_menu() {
	printf("Encontrei um jogo guardado\n1 - Continuar\n2 - Novo Jogo\n");
	int op = 0;
	char buf[64];
	int buul = 0;
	while (!buul) {
		if (fgets(buf, sizeof(buf), stdin) != NULL) {
			if (sscanf(buf, "%d", &op) == 1 && (op == 1 || op == 2))
				buul = 1;
			else
				printf("Número invalido. Tenta outra vez man\n");
		}
	}
	return op;
}
