#ifndef UNDO_H
#define UNDO_H


#define MAX_UNDO

typedef struct {
	int src_pile;	// Pile from which the cards are going to be removed
	int column_out; // Position in the pile of the card selected by the player
	int card_count; // How many cards were selected

	int dest_pile; // Pile to which the card(s) are going to be moved

	int flag; // Indicates the current state of the move (enum Flag)
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
