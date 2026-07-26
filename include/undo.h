#ifndef UNDO_H
#define UNDO_H
#include "game_state.h"

void initialize_history();

int is_history_empty(History *h);

void enqueue_history(History *h, Move *m);

void dequeue_history(History *h);

int peek_history(History *h);

#endif
