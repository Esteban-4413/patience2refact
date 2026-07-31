#ifndef UNDO_H
#define UNDO_H

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
} Move;

typedef struct History {
	int front;
	int length;
	Move move_history[MAX_UNDO];
} History;

void initialize_history(History *h);

int is_history_empty(History *h);

void enqueue_history(History *h, Move *m);

int dequeue_history(History *h, Move *out_move);

int peek_history(History *h, Move *peeked_move);

#endif
