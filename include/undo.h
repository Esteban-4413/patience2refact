#ifndef UNDO_H
#define UNDO_H

#include <stdbool.h>
#define MAX_UNDO 5

typedef enum {
	SET = (-2), // Move set by default
	INVALID,	// The move was verified and classified as invalid
	WAIT,		// Contains information about the source pile/exit of the cards
	VALID		// The move was verified and classified as valid
} MoveFlag;

typedef struct {
	int src_pile;	// Pile from which the cards are going to be removed
	int column_out; // Position in the pile of the card selected by the player
	int card_count; // How many cards were selected

	int dest_pile; // Pile to which the card(s) are going to be moved

	MoveFlag is_move_valid; // Indicates the current state of the move
	bool is_auto;
} Move;

typedef struct {
	int count;
	Move sub_moves[15];
} Turn;

typedef struct History {
	int front;
	int length;
	Turn turn_history[MAX_UNDO];
} History;

void initialize_history(History *h);

int is_history_empty(History *h);

void push_history(History *h, Turn *m);

int pop_history(History *h, Turn *out_move);

int peek_history(History *h, Turn *peeked_move);


#endif
