#ifndef GAME_DEF_H
#define GAME_DEF_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	F_NONE = 0, // * Sem restrições

	// ---SEQUENCE ORDER---
	F_SEQUENCE = 1 << 0,   // + Pode mover sequência de cartas
	F_DESCENDING = 1 << 1, // [ Sequência decrescente
	F_ASCENDING = 1 << 2,  // ] Sequência crescente

	// --- VALOR EM RELAÇÃO AO DESTINO (RANK)---
	F_VAL_LOWER = 1 << 3,	 // < Topo a mover é 1 valor inferior ao destino
	F_VAL_HIGHER = 1 << 4,	 // > Topo a mover é 1 valor superior ao destino
	F_VAL_ADJACENT = 1 << 5, // ~ Valor imediatamente superior ou inferior

	// --- (SUIT) ---
	F_SUIT_SAME_SEQ = 1 << 6, // m Cartas a mover do mesmo naipe
	F_SUIT_SAME_DST = 1 << 7, // M Topo a mover do mesmo naipe do destino
	F_SUIT_ALT_SEQ = 1 << 8,  // x Cartas a mover de naipes alternados
	F_SUIT_DIFF_DST = 1 << 9, // X Topo a mover de naipe diferente do destino

	// ---(COLOR) ---
	F_COLOR_SAME_SEQ = 1 << 10, // c Cartas a mover da mesma cor
	F_COLOR_SAME_DST = 1 << 11, // C Topo a mover da mesma cor do destino
	F_COLOR_ALT_SEQ = 1 << 12,	// d Cartas a mover de cores alternadas
	F_COLOR_DIFF_DST = 1 << 13, // D Topo a mover de cor diferente do destino

	// --- CONDIÇÕES DA PILHA E CARTAS ESPECÍFICAS ---
	F_EMPTY_DEST = 1 << 14, // V Pilha de destino deve estar vazia
	F_TOP_ACE = 1 << 15,	// a Topo a mover deve ser Ás
	F_BOTTOM_ACE = 1 << 16, // A Fundo a mover deve ser Ás
	F_TOP_KING = 1 << 17,	// k Topo a mover deve ser Rei
	F_BOTTOM_KING = 1 << 18 // K Fundo a mover deve ser Rei

} PileFlags;

typedef struct {
	char pile_name[20];
	int num_cards;
} InitCommand;

typedef struct {
	char name[20];
	bool visible_all;
	bool visible_none;
	bool visible_top_only;
	bool max_one_card;
} PileClass;

typedef struct {
	char src_pile[20];
	char dest_pile[20];
	uint32_t flags;
	bool is_auto;
} MoveRule;

typedef struct {
	char name_condition[20];
	int target_card_count;
} WinCondition;

typedef struct {
	char game_name[64];
	int num_decks;

	PileClass pile_classes[50];
	int class_count;

	MoveRule rules[50];
	int rule_count;

	WinCondition win_condition[50];
	int win_cond_count;

	InitCommand inits[50];
	int init_count;
} GameDefinition;

#endif
