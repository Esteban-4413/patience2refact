#ifndef UNDO_H
#define UNDO_H
#include "game_state.h"

void initialize_history(History *h);

int is_history_empty(History *h);

void enqueue_history(History *h, Move *m);

int dequeue_history(History *h, Move *out_move);

int peek_history(History *h, Move *peeked_move);

#endif
