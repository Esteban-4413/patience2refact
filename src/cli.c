#include "../include/cli.h"
#include <stdio.h>
#include <string.h>


GameCommand parse_command(char *input_text) {
	GameCommand cmd;
	cmd.arg[0] = '\0';
	cmd.card_index_input = -1;
	cmd.dest = -1;
	cmd.src = -1;
	char keyword[20];
	if (sscanf(input_text, "%s", keyword) != 1) {
		cmd.type = CMD_UNRECOGNIZED;
		return cmd;
	}
	if (strcmp(keyword, "quit") == 0 || strcmp(keyword, "q") == 0)
		cmd.type = CMD_QUIT;

	else if (strcmp(keyword, "mv") == 0) {
		cmd.type = CMD_MOVE;
		if (sscanf(input_text, "%*s %d|%d %d", &cmd.src, &cmd.card_index_input, &cmd.dest) == 3) {
		} else if (sscanf(input_text, "%*s %d %d", &cmd.src, &cmd.dest)) {
			cmd.card_index_input = -1;
		} else {
			cmd.src = -1;
			cmd.dest = -1;
		}
	} else if (strcmp(input_text, "undo") == 0)
		cmd.type = CMD_UNDO;
	else if (strcmp(keyword, "save") == 0) {
		cmd.type = CMD_SAVE;
	} else if (strcmp(keyword, "hint") == 0) {
		cmd.type = CMD_HINT;
	} else
		cmd.type = CMD_UNRECOGNIZED;
	return cmd;
}


bool execute_command(Game_state *state, GameCommand cmd, char *feedback_msg) {
	if (state == NULL) {
		strcpy(feedback_msg, "Error: No game was loaded");
		return true;
	}

	bool used_hint = false;
	if (cmd.type != CMD_HINT && state->stats != NULL) {
		if (cmd.type == CMD_MOVE && (cmd.src - 1) == state->stats->hint_src_pile &&
			(cmd.dest - 1) == state->stats->hint_dest_pile)
			used_hint = true;
		state->stats->hint_dest_pile = -1;
		state->stats->hint_src_pile = -1;
		state->stats->hint_card_count = 0;
	}

	switch (cmd.type) {
	case CMD_QUIT:
		// strcpy(feedback_msg, "Bye byeeee...");
		return false;
	case CMD_MOVE:
		if (cmd.src == -1 || cmd.dest == -1 || cmd.src > state->pile_count || cmd.dest > state->pile_count)
			strcpy(feedback_msg, "Error: Invalid move format.");
		else {
			if (fill_move(state, &cmd)) {
				if (!is_move_valid(state))
					strcpy(feedback_msg, "Not a valid move");
				else {
					Turn current_turn;
					current_turn.count = 0;
					state->move.is_auto = false;
					current_turn.sub_moves[current_turn.count] = state->move;
					current_turn.count++;
					int src = state->move.src_pile;
					int dest = state->move.dest_pile;
					int count = state->move.card_count;

					do_move(state);
					state->stats->moves_count++;
					if (!used_hint) {
						int pts = update_score(state, src, dest, count);
						if (pts >= 0)
							sprintf(feedback_msg, "Moved %d card(s) from %d to (%d) (+%d pts)", count, cmd.src,
									cmd.dest, pts);
						else
							sprintf(feedback_msg, "Moved %d card(s) from %d to %d (-%d pts)", count, cmd.src, cmd.dest,
									pts);
					} else
						sprintf(feedback_msg, "Move succesful! (0 points: Hint used)");
					auto_moves(state, &current_turn);
					push_history(state->history, &current_turn);

					sprintf(feedback_msg, "The card was moved from the pile %d to the pile %d", cmd.src, cmd.dest);
				}
			} else
				strcpy(feedback_msg, "Error: We could not process the move.");
		}
		break;
	case CMD_UNDO:
		undo_move(state);
		strcpy(feedback_msg, "Undo");
		break;
	case CMD_SAVE:
		save_game(state);
		strcpy(feedback_msg, "Your game was saved");
		return false;
	case CMD_HINT: {
		MoveList ml = generate_hints(state);

		if (state->stats->hints_used >= 3) {
			snprintf(feedback_msg, 127, "No hints used remaining! (Max used)");
			return true;
		} else if (ml.count > 0) {
			int bag_size = (ml.count >= 3) ? 3 : ml.count;
			int rand_idx = rand() & bag_size;
			Move best = ml.moves[rand_idx].move;
			if (state->stats != NULL) {
				state->stats->hint_src_pile = best.src_pile;
				state->stats->hint_dest_pile = best.dest_pile;
				state->stats->hint_card_count = best.card_count;
				state->stats->hints_used++;
			}
			if (best.card_count > 1)
				snprintf(feedback_msg, 128, "Hint: Move a sequence of %d cards from pile %d to %d", best.card_count,
						 best.src_pile + 1, best.dest_pile + 1);
			else
				snprintf(feedback_msg, 128, "Hint: Move top card cards from pile %d to %d", best.src_pile + 1,
						 best.dest_pile + 1);
		} else
			strcpy(feedback_msg, "No valid moves. You are stuck!");
		break;
	case CMD_UNRECOGNIZED:
		strcpy(feedback_msg, "Command not recognized... Try: mv, undo, save, hint, or quit");
		break;
	}
	default:
		strcpy(feedback_msg, "Command ignored");
		break;
	}
	return true;
}
