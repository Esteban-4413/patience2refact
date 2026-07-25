#include "../include/stack.h"

pile *make_pile(int num, card_node *head) {
	pile *p = malloc(sizeof(struct pile));
	p->num_cards = num;
	p->head = head;
	return p;
}

void push(pile *pile, card *card) {
	if (pile == NULL)
		return;
	else {
		card_node *new_card = malloc(sizeof(struct card_node));
		new_card->card->next_card = pile->head;
		pile->head = card;
	}
}

card *pop(pile *pile);

card *shift(pile *pile);

void unshift(pile *pile, card *card);

card *peek_card_at(pile *pile, int index);

card *peek(pile *pile);

card *peek_last(pile *pile);

void delete1(pile *pile, card *card);
