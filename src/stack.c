#include "../include/stack.h"

Pile *make_pile(int num, Card *head) {
	Pile *p = malloc(sizeof(Pile));
	p->num_cards = num;
	p->head = head;
	return p;
}

void push(Pile *pile, Card *c) {
	if (pile == NULL || c == NULL)
		return;
	c->next = pile->head;
	pile->head = c;
	pile->num_cards++;
}

Card *pop(Pile *pile) {
	if (pile == NULL || pile->head == NULL)
		return NULL;
	Card *r = pile->head;
	pile->head = pile->head->next;
	pile->num_cards--;
	return r;
}

Card *shift(Pile *p) {
	if (p == NULL || p->head == NULL)
		return NULL;
	Card **indirect = &(p->head);
	while ((*indirect)->next != NULL) {
		indirect = (&(*indirect)->next);
	}
	Card *last_card = *indirect;
	*indirect = NULL;
	p->num_cards--;
	return last_card;
}

void unshift(Pile *p, Card *c) {
	if (p == NULL || c == NULL)
		return;
	c->next = NULL;
	Card **indirect = &(p->head);
	while ((*indirect)->next != NULL) {
		indirect = (&(*indirect)->next);
	}
	*indirect = c;
	p->num_cards++;
}

Card *peek_card_at(Pile *pile, int index) {
	if (pile == NULL || index < 0)
		return NULL;
	Card *r = pile->head;
	for (int i = 0; i < index && r != NULL; i++) {
		r = r->next;
	}
	return r;
}

Card *peek_pile(Pile *pile) { return (pile == NULL) ? NULL : (pile->head); }

Card *peek_last_pile(Pile *pile) {
	if (pile == NULL)
		return NULL;
	Card *r = pile->head;
	while (r->next != NULL) {
		r = r->next;
	}
	return r;
}

void remove_card(Pile *p, Card *c) {
	if (p == NULL || p->head == NULL || c == NULL)
		return;
	Card **indirect = &(p->head);
	while (*indirect != NULL && *indirect != c) {
		indirect = (&(*indirect)->next);
	}
	if (*indirect == c) {
		*indirect = c->next;
		c->next = NULL;
		p->num_cards--;
	}
}
