#include "../include/parser.h"
#include <stdint.h>

uint32_t parse_move_flags(char *flags_str) {
	uint32_t flags = 0;

	for (int i; flags_str[i] != '\0'; i++) {
		switch (flags_str[i]) {
		case '*':
			flags |= F_NONE; // same as writing flags = flags | F_NONE;
			break;
		case '+':
			flags |= F_SEQUENCE;
			break;
		case '[':
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

GameDefinition *incializa_estado(void) {
	GameDefinition *def = malloc(sizeof(*def));
	def->game_name[0] = '\0';
	def->num_decks = 1;
	def->class_count = 0;
	def->rule_count = 0;
	def->win_cond_count = 0;
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

void cmd_tipo(char *buffer, GameDefinition *def) {
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
