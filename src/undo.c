#include "../include/undo.h"

void initialize_history(History *h) {
	h->front = 0;
	h->length = 0;
}

int is_history_empty(History *h) { return (h->length == 0); }

void enqueue_history(History *h, Move *m) {
	if (h->length == MAX_UNDO) {
		h->front = (h->front + 1) % MAX_UNDO;
		h->length--;
	}
	h->move_history[(h->length + h->front) % MAX_UNDO] = *m;
	h->length++;
}

int dequeue_history(History *h, Move *out_move) {
	if (h->length == 0)
		return 0;
	*out_move = h->move_history[h->front];
	h->length--;
	h->front = (h->front + 1) % MAX_UNDO;
	return 1;
}

int peek_history(History *h, Move *peeked_move) {
	if (h->length == 0)
		return 0;
	*peeked_move = h->move_history[h->front];
	return 1;
}
