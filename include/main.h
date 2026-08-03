#include "cli.h"
#include "game_state.h"
#include "gui.h"
#include <stdbool.h>

typedef enum { CMD_MODE_CLI, CMD_MODE_GUI, CMD_MODE_UNKNOWN } UImode;
void print_deck_arr(Card *deck_arr);
void print(Pile *deck_llist);
