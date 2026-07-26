#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "stack.h"
#include <stdint.h>

#define MAX_UNDO 5

typedef enum {
	SET = (-2), // Move set by default
	INVALID,	// The move was verified and classified as invalid
	WAIT,		// Contains information about the source pile/exit of the cards
	VALID		// The move was verified and classified as valid
} Flag;

typedef enum {
	F_NONE = 0, // * Sem restrições

	// ---SEQUENCE ORDER---
	F_SEQUENCE = 1 << 0,   // + Pode mover sequência de cartas
	F_DESCENDING = 1 << 1, // [ Sequência decrescente
	F_ASCENDING = 1 << 2,  // ] Sequência crescente

	// --- VALOR EM RELAÇÃO AO DESTINO ---
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
	int pile_out;	// Pile from which the cards are going to be removed
	int column_out; // Position in the pile of the card selected by the player
	int card_count; // How many cards were selected

	int pile_in; // Pile to which the card(s) are going to be moved

	int flag; // Indicates the current state of the move (enum Flag)
} Move;

typedef struct History {
	int front;
	int length;
	Move move_history[MAX_UNDO];
} History;


typedef struct {
	Pile **table_piles;
	int pile_count;

	Move move;
	History history;
	// game_def *Game_def;

} Game_state;


void fill_deck(Pile *pile);
void shuffle_pile(Pile *pile);

#endif
