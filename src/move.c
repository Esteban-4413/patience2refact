#include "../include/cli.h"

void print_move(Game_state *state){
    Move m = state->move;
    printf("Pile src: %d | Column: %d | Pile dest: %d | num: %d | Flag: %d" "\n",
        m.src_pile, m.column_out, m.dest_pile, m.card_count, m.is_valid);
}


void fill_move(Game_state *current_state, GameCommand *cmd){
    Flag f = current_state->move.is_valid;
    switch (f) {
        case SET:
            if (strlen(cmd->arg) < 3){
                printf("Your command was inclomplete \n");
                printf("The command has to be 'move pile|column' \n");
                printf("Try again... \n");
                cmd->type = CMD_UNRECOGNIZED;
            } else {
                char p[25];
                int j = 0;
                int i;
                for(i = 0; cmd->arg[i] != '\0' && (cmd->arg[i] != '|'); i++){
                    p[j] = cmd->arg[i];
                    j++;
                }
                p[j] = '\n';
                int pile = atoi(p) - 1;
                // o input é numerado sempre de 1...
                // mas na lógica  do jogo é numerado de 0...
                char c[25];
                j = 0;
                for(i = i + 1; cmd->arg[i] != '\0'; i++){
                    c[j] = cmd->arg[i];
                   j++;
                }
                c[j]='\n';

                int column = atoi(c) - 1 ;

                current_state->move.src_pile = pile;
                current_state->move.column_out = column;

                current_state->move.card_count = current_state->table_piles[pile]->num_cards - column;
                // Ou seja, se o card_count <= 0
                // o jogador selecionou uma coluna em que não existem
                // cartas nessa pilha!

                current_state->move.is_valid = WAIT;

                print_move(current_state); // Só para debugar - apagar depois

            }
            break;
        case WAIT:
            // TODO
            break;
        case VALID:
            // TODO
            break;
        case INVALID:
            // TODO
            break;
    }
}
