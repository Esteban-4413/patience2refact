#include "cards.h"

pile *make_pile(int num, card_node *head);

void push(pile *pile, card *card);

card *pop(pile *pile);

card *shift(pile *pile);

void unshift(pile *pile, card *card);

card *peek_card_at(pile *pile, int index);

card *peek(pile *pile);

card *peek_last(pile *pile);

void delete1(pile *pile, card *card);
