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
                char p[3];
                int j = 0;
                int i;
                for(i = 0; (cmd->arg[i] != '\0') && (cmd->arg[i] != '|'); i++){
                    p[j] = cmd->arg[i];
                    j++;
                }
                p[j] = '\n';
                int pile = atoi(p) - 1;
                // o input é numerado sempre de 1...
                // mas na lógica  do jogo é numerado de 0...
                char c[3];
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
                if (current_state->move.card_count <= 0){
                    printf("There is no card in this position\n");
                    printf("Try again...\n");
                    set_move(current_state);
                } else {
                    current_state->move.is_valid = WAIT;
                }
                print_move(current_state);
            }
            break;
        case WAIT:
            if (strlen(cmd->arg) < 2){
                printf("Your command was inclomplete \n");
                printf("The command has to be 'move pile|' \n");
                printf("Try again... \n");
            } else {
                char p_dest[3];
                int i;
                int j = 0;
                for(i = 0; (cmd->arg[i] != '\0') && (cmd->arg[i] != '|'); i++){
                    p_dest[j] = cmd->arg[i];
                    j++;
                }
                p_dest[i] = '\0';

                current_state->move.dest_pile = atoi(p_dest) - 1;
                current_state->move.is_valid = VALID; // Para fins de teste apenas

                //validate_move(); (TODO)
            }
            print_move(current_state);

            break;
        case VALID:
            move(current_state);
            printf("MOVE doneee! \n");
            print_board(current_state);
            // add_history(); (TODO)
            // set_move(current_state);
            break;
        case INVALID:
            // Reinicia o move
            set_move(current_state);
            print_move(current_state);

            break;
    }
}

void move(Game_state *current_state){
    Move move = current_state->move;
    Pile *src_pile = current_state->table_piles[move.src_pile];
    Pile *dest_pile = current_state->table_piles[move.dest_pile];

    Card *ant = NULL;
    Card *head = peek_card_at(src_pile, 0); // Não tem como a pilha está fazia (já foi verificado)
    if (src_pile == NULL || src_pile->head == NULL) return; // não acontece!!
    if (move.column_out > 0) {
        ant = peek_card_at(src_pile, move.column_out - 1);
        head = peek_card_at(src_pile, move.column_out);
    }

    if (ant == NULL) { src_pile->head = NULL; }
    else { ant->next = NULL; }
    src_pile->num_cards -= move.card_count;

    if (ant != NULL) printf("ant [%d %d] \n", ant->rank, ant->suit);
    if (head != NULL) printf("head [%d %d] \n", head->rank, head->suit);

    if (dest_pile->head == NULL) { dest_pile->head = head; }
    else {
          Card *last_card = peek_card_at(dest_pile, dest_pile->num_cards - 1);
          if (last_card == NULL) return;
          else {printf("last [%d %d] \n", last_card->rank, last_card->suit);}
          printf("HEreee!\n");
          if (last_card->next == NULL) printf("rigth!\n");
          last_card->next = head;

    }
    dest_pile->num_cards += move.card_count;


}
