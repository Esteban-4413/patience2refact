#include "../include/undo.h"

void initialize_history(History *h) {
	h->front = 0;
	h->length = 0;
}

int is_history_empty(History *h) { return (h->length == 0); }

void push_history(History *h, Turn *m) {
	if (h->length == MAX_UNDO) {
		h->front = (h->front + 1) % MAX_UNDO;
		h->length--;
	}
	int top_index = (h->front + h->length) % MAX_UNDO;
	h->turn_history[top_index] = *m;
	h->length++;
}

int pop_history(History *h, Turn *out_move) {
	if (h->length == 0)
		return 0;
	int top_index = (h->front + h->length - 1) % MAX_UNDO;
	*out_move = h->turn_history[top_index];
	h->length--;
	return 1;
}

int peek_history(History *h, Turn *peeked_move) {
	if (h->length == 0)
		return 0;
	int top_index = (h->front + h->length - 1) % MAX_UNDO;
	*peeked_move = h->turn_history[top_index];
	return 1;
}
