#include "../include/stack.h"

pile *make_pile(int num, card *head) {
	pile *p = malloc(sizeof(struct pile));
	p->num_cards = num;
	p->head = head;
	return p;
}

void push(pile *pile, card *c) {
	if (pile == NULL || c == NULL)
		return;
	c->next = pile->head;
	pile->head = c;
	pile->num_cards++;
}

card *pop(pile *pile) {
	if (pile == NULL)
		return NULL;
	card *r = pile->head;
	pile->head = pile->head->next;
	pile->num_cards--;
	return r;
}

card *shift(pile *p) {
	if (p == NULL || p->head == NULL)
		return NULL;
	card **indirect = &(p->head);
	while ((*indirect)->next != NULL) {
		indirect = (&(*indirect)->next);
	}
	card *last_card = *indirect;
	*indirect = NULL;
	p->num_cards--;
	return last_card;
}

void unshift(pile *p, card *c) {
	if (p == NULL || c == NULL)
		return;
	c->next = NULL;
	card **indirect = &(p->head);
	while ((*indirect)->next != NULL) {
		indirect = (&(*indirect)->next);
	}
	*indirect = c;
	p->num_cards++;
}

card *peek_card_at(pile *pile, int index) {
	if (pile == NULL || index < 0)
		return NULL;
	card *r = pile->head;
	for (int i = 0; i < index && r != NULL; i++) {
		r = r->next;
	}
	return r;
}

card *peek(pile *pile) { return (pile == NULL) ? NULL : (pile->head); }

card *peek_last(pile *pile) {
	if (pile == NULL)
		return NULL;
	card *r = pile->head;
	while (r->next != NULL) {
		r = r->next;
	}
	return r;
}

void remove_card(pile *p, card *c) {
	if (p == NULL || p->head == NULL || c == NULL)
		return;
	card **indirect = &(p->head);
	while (*indirect != NULL && *indirect != c) {
		indirect = (&(*indirect)->next);
	}
	if (*indirect == c) {
		*indirect = c->next;
		c->next = NULL;
		p->num_cards--;
	}
}
