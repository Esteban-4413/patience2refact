#include "cards.h"
#include "stack.h"

#define MAX_UNDO 5

typedef enum {
	SET = (-2), // Move set by default
	INVALID,	// The move was verified and classified as invalid
	WAIT,		// Contains information about the source pile/exit of the cards
	VALID		// The move was verified and classified as valid
} Flag;

typedef struct {
	int pile_out;	// Pile from which the cards are going to be removed
	int column_out; // Position in the pile of the card selected by the player
	int card_count; // How many cards were selected

	int pile_in; // Pile to which the card(s) are going to be moved

	int flag; // Indicates the current state of the move (enum Flag)
} Move;

typedef struct History {
	int front;
	int length;
	Move move_history[MAX_UNDO];
} History;


typedef struct {
	Pile **table_piles;
	int pile_count;

	Move move;
	History history;
	// game_def *Game_def;

} Game_state;


void fill_deck(Pile *pile);
void shuffle_pile(Pile *pile);
