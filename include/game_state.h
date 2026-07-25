#include "stack.h"

#define MAX_UNDO 5

typedef enum {
    SET = (-2), // Move set by default
    INVALID,    // The move was verified and classified as invalid
    WAIT,       // Contains information about the source pile/exit of the cards
    VALID       // The move was verified and classified as valid
} Flag;

typedef struct move {
    int pile_out;    // Pile from which the cards are going to be removed
    int column_out;  // Position in the pile of the card selected by the player
    int card_count;  // How many cards were selected

    int pile_in;     // Pile to which the card(s) are going to be moved

    int flag;        // Indicates the current state of the move (enum Flag)
} move;

typedef struct history{
    int front;
    int length;
    move move_history[MAX_UNDO];
} history;



typedef struct {
	pile **piles;
	int pile_count;

	move Move;
	history History;
	//game_def *Game_def;

} game_state;




void fill_deck(pile *pile);
void shuffle_pile(pile *pile);
