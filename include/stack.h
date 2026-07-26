#ifndef STACK_H
#define STACK_H

#include "cards.h"

Pile *make_pile(int num, Card *head);

void push(Pile *pile, Card *card);

Card *pop(Pile *pile);

Card *shift(Pile *pile);

void unshift(Pile *pile, Card *card);

Card *peek_card_at(Pile *pile, int index);

Card *peek(Pile *pile);

Card *peek_last(Pile *pile);

void remove_card(Pile *pile, Card *card);

#endif
