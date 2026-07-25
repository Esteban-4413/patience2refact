#include "cards.h"

pile *make_pile(int num, card *head);

void push(pile *pile, card *card);

card *pop(pile *pile);

card *shift(pile *pile);

void unshift(pile *pile, card *card);

card *peek_card_at(pile *pile, int index);

card *peek(pile *pile);

card *peek_last(pile *pile);

void remove_card(pile *pile, card *card);
