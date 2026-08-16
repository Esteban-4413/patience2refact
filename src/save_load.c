#include "../include/save_load.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void mkdir_saves() {
#ifdef _WIN32
	mkdir("saves");
#else
	mkdir("saves", S_IRWXU);
#endif
}

bool save_game(Game_state *current_state) {

	mkdir_saves();
	char name[64];
	strcpy(name, current_state->definition->game_name);

	char just_name[64];
	int i = 0;
	while (name[i] != '\0' && name[i] != '.') {
		just_name[i] = name[i];
		i++;
	}
	just_name[i] = '\0';

	char path[128];
	snprintf(path, 128, "saves/%s.txt", just_name);

	FILE *f = fopen(path, "w");
	if (f == NULL)
		return false;

	fprintf(f, "%s\n", name);

	if (current_state->stats != NULL) {
		time_t current_time = time(NULL);
		long elapsed = (long)difftime(current_time, current_state->stats->start_time);
		fprintf(f, "STATS %ld %d %d %d\n", elapsed, current_state->stats->moves_count, current_state->stats->score,
				current_state->stats->hints_used);

	} else {
		fprintf(f, "STATS 0 0 0 0\n");
	}

	for (int i = 0; i < current_state->pile_count; i++) {
		if (current_state->table_piles[i]->num_cards > 0) {
			Card *current_card = current_state->table_piles[i]->head;
			while (current_card != NULL) {
				char suit[3];
				char rank[4];
				translate_suit(suit, current_card->suit);
				translate_rank(rank, current_card->rank);
				fprintf(f, "%s%s ", rank, suit);
				current_card = current_card->next;
			}
		}
		fprintf(f, "\n");
	}

	fclose(f);
	return true;
}

void translate_suit(char *char_suit, Suit suit) {
	switch (suit) {
	case SUIT_HEART:
		strcpy(char_suit, "H");
		break;
	case SUIT_SPADE:
		strcpy(char_suit, "S");
		break;
	case SUIT_CLUB:
		strcpy(char_suit, "C");
		break;
	case SUIT_DIAMOND:
		strcpy(char_suit, "D");
		break;
	default:
		break;
	}
}

void translate_rank(char *char_rank, Rank rank) {
	switch (rank) {
	case RANK_ACE:
		strcpy(char_rank, "A");
		break;
	case RANK_JACK:
		strcpy(char_rank, "J");
		break;
	case RANK_QUEEN:
		strcpy(char_rank, "Q");
		break;
	case RANK_KING:
		strcpy(char_rank, "K");
		break;
	default:
		char value[3];
		snprintf(value, 3, "%d", rank + 1);
		strcpy(char_rank, value);
		break;
	}
}

Card *read_card(char *str) {
	int len = strlen(str);

	int suit;
	char char_suit = str[len - 1];

	switch (char_suit) {
	case 'H':
		suit = SUIT_HEART;
		break;
	case 'S':
		suit = SUIT_SPADE;
		break;
	case 'C':
		suit = SUIT_CLUB;
		break;
	case 'D':
		suit = SUIT_DIAMOND;
		break;
	}

	int rank;
	char char_rank[3];
	int i;
	for (i = 0; i < len - 1; i++)
		char_rank[i] = str[i];
	char_rank[i] = '\0';
	if (strcmp(char_rank, "A") == 0) {
		rank = RANK_ACE;
	} else if (strcmp(char_rank, "J") == 0) {
		rank = RANK_JACK;
	} else if (strcmp(char_rank, "Q") == 0) {
		rank = RANK_QUEEN;
	} else if (strcmp(char_rank, "K") == 0) {
		rank = RANK_KING;
	} else {
		rank = atoi(char_rank) - 1;
	}
	return make_card(suit, rank);
}

Game_state *load_game(char *save_file) {
	Game_state *state = calloc(1, sizeof(*state));

	state->history = malloc(sizeof(History));
	if (state->history != NULL) {
		initialize_history(state->history);
	}

	FILE *f = fopen(save_file, "r");
	char buffer[128];

	// load_definition
	fgets(buffer, sizeof(buffer), f);
	int len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n')
		buffer[len - 1] = '\0';
	char path[256];
	snprintf(path, sizeof(path), "paciencias/%s.paciencia", buffer);

	GameDefinition *current_def = load_patience(path);
	state->definition = current_def;

	state->stats = malloc(sizeof(*state->stats));
	state->stats->start_time = time(NULL);
	state->stats->moves_count = 0;
	state->stats->score = 0;
	state->stats->hint_src_pile = -1;
	state->stats->hint_dest_pile = -1;
	state->stats->hint_card_count = 0;
	state->stats->hints_used = 0;

	long pos_before_stats = ftell(f);
	if (fgets(buffer, sizeof(buffer), f) != NULL) {
		if (strncmp(buffer, "STATS", 5) == 0) {
			long elapsed = 0;
			int moves = 0, score = 0, hints = 0;
			sscanf(buffer, "STATS %ld %d %d %d", &elapsed, &moves, &score, &hints);

			state->stats->start_time = time(NULL) - elapsed;
			state->stats->moves_count = moves;
			state->stats->score = score;
			state->stats->hints_used = hints;
		} else
			fseek(f, pos_before_stats, SEEK_SET);
	}

	state->pile_count = current_def->init_count;
	state->table_piles = malloc((state->pile_count + 1) * sizeof(Pile *));

	// load_cards
	int i = 0;
	while (i < state->pile_count) {
		state->table_piles[i] = malloc(sizeof(Pile));
		state->table_piles[i]->head = NULL;
		state->table_piles[i]->num_cards = 0;
		state->table_piles[i]->pile_class = NULL;

		for (int c = 0; c < current_def->class_count; c++) {
			if (strcmp(current_def->pile_classes[c].name, current_def->inits[i].pile_name) == 0) {
				state->table_piles[i]->pile_class = &(current_def->pile_classes[c]);
				break;
			}
		}
		if (fgets(buffer, sizeof(buffer), f) != NULL) {
			int len = strlen(buffer);
			if (len > 0 && buffer[len - 1] == '\n')
				buffer[len - 1] = '\0';

			if (buffer[0] != '\0') {
				char token[4];
				int pos = 0;
				int read = 0;
				while (sscanf(buffer + pos, "%s%n", token, &read) == 1) {
					if (read <= 0)
						break;
					Card *c = read_card(token);
					unshift(state->table_piles[i], c);
					pos += read;
					read = 0;
				}
			}
		}
		i++;
	}
	fclose(f);
	return state;
}
