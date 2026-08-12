#include "../include/parser.h"
#include <stdint.h>

uint32_t parse_move_flags(char *flags_str) {
	uint32_t flags = 0;

	for (int i = 0; flags_str[i] != '\0'; i++) {
		switch (flags_str[i]) {
		case '*':
			flags |= F_NONE; // same as writing flags = flags | F_NONE;
			break;
		case '+':
			flags |= F_SEQUENCE;
			break;
		case '[':
			flags |= F_DESCENDING;
			break;
		case ']':
		    flags |= F_ASCENDING;
			break;
		case '<':
			flags |= F_VAL_LOWER;
			break;
		case '>':
			flags |= F_VAL_HIGHER;
			break;
		case '~':
			flags |= F_VAL_ADJACENT;
		case 'm':
			flags |= F_SUIT_SAME_SEQ;
			break;
		case 'M':
			flags |= F_SUIT_SAME_DST;
			break;
		case 'x':
			flags |= F_SUIT_ALT_SEQ;
			break;
		case 'X':
			flags |= F_SUIT_DIFF_DST;
			break;
		case 'c':
			flags |= F_COLOR_SAME_SEQ;
			break;
		case 'C':
			flags |= F_COLOR_SAME_DST;
			break;
		case 'd':
			flags |= F_COLOR_ALT_SEQ;
			break;
		case 'D':
			flags |= F_COLOR_DIFF_DST;
			break;
		case 'V':
			flags |= F_EMPTY_DEST;
			break;
		case 'a':
			flags |= F_TOP_ACE;
			break;
		case 'A':
			flags |= F_BOTTOM_ACE;
			break;
		case 'k':
			flags |= F_TOP_KING;
			break;
		case 'K':
			flags |= F_BOTTOM_KING;
			break;
		}
	}
	return flags;
}

GameDefinition *init_state(void) {
	GameDefinition *def = calloc(1, sizeof(*def));
	def->num_decks = 1;
	return def;
}

void cmd_move(char *buffer, GameDefinition *def, bool is_auto) {
	if (def->rule_count >= 50)
		return;
	char cmd[32], src[20], dest[20], flags_str[32];
	flags_str[0] = '\0';

	sscanf(buffer, "%s %s %s %s", cmd, src, dest, flags_str);

	int idx = def->rule_count;
	strcpy(def->rules[idx].src_pile, src);
	strcpy(def->rules[idx].dest_pile, dest);
	def->rules[idx].flags = parse_move_flags(flags_str);
	def->rules[idx].is_auto = is_auto;
	def->rule_count++;
}

void cmd_class(char *buffer, GameDefinition *def) {
	if (def->class_count >= 20)
		return;

	char cmd[32], name_class[20], flags_str[10];
	flags_str[0] = '\0';
	sscanf(buffer, "%s %s %s", cmd, name_class, flags_str);

	int idx = def->class_count;
	strcpy(def->pile_classes[idx].name, name_class);


	for (int i = 0; flags_str[i] != '\0'; i++) {
		if (flags_str[i] == '=')
			def->pile_classes[idx].visible_all = true;
		if (flags_str[i] == '_')
			def->pile_classes[idx].visible_none = true;
		if (flags_str[i] == '^')
			def->pile_classes[idx].visible_top_only = true;
		if (flags_str[i] == '1')
			def->pile_classes[idx].max_one_card = true;
	}
	def->class_count++;
}

void cmd_init(char *buffer, GameDefinition *def) {
	if (def->init_count >= 50)
		return;

	char cmd[32], name_class[20];
	int cards;

	sscanf(buffer, "%s %s %d", cmd, name_class, &cards);

	int idx = def->init_count;
	strcpy(def->inits[idx].pile_name, name_class);
	def->inits[idx].num_cards = cards;

	def->init_count++;
}

void cmd_win(char *buffer, GameDefinition *def) {
	if (def->win_cond_count >= 10)
		return;

	char cmd[32], name_class[20];
	int cards;

	sscanf(buffer, "%s %s %d", cmd, name_class, &cards);

	int idx = def->win_cond_count;
	strcpy(def->win_condition[idx].name_condition, name_class);
	def->win_condition[idx].target_card_count = cards;

	def->win_cond_count++;
}

void what_command(char *comando, char *buffer, GameDefinition *def) {
	if (strcmp(comando, "JOGO") == 0) {
		sscanf(buffer, "%s %s", comando, def->game_name);
	} else if (strcmp(comando, "BARALHOS") == 0) {
		sscanf(buffer, "%s %d", comando, &def->num_decks);
	} else if (strcmp(comando, "TIPO") == 0) {
		cmd_class(buffer, def);
	} else if (strcmp(comando, "MOV") == 0) {
		cmd_move(buffer, def, false);
	} else if (strcmp(comando, "AUTO") == 0) {
		cmd_move(buffer, def, true);
	} else if (strcmp(comando, "INIT") == 0) {
		cmd_init(buffer, def);
	} else if (strcmp(comando, "WIN") == 0) {
		cmd_win(buffer, def);
	}
}

int process_line(char *buffer, GameDefinition *def) {
	char *comentario = strchr(buffer, '#');
	if (comentario != NULL)
		*comentario = '\0';
	char comando[32];

	if (sscanf(buffer, "%s", comando) == 1)
		what_command(comando, buffer, def);
	return 1;
}

GameDefinition *load_patience(char *ficheiro) {
	GameDefinition *def = init_state();
	FILE *paciencia = fopen(ficheiro, "r");
	char buffer[255];

	if (paciencia == NULL) {
		printf("Erro: Ficheiro '%s' nao existe.\n", ficheiro);
		return def;
	}

	while (fgets(buffer, 255, paciencia) != NULL) {
		process_line(buffer, def);
	}

	fclose(paciencia);
	return def;
}
