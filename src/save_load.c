#include "../include/save_load.h"


bool save_game(Game_state *current_state){
    char name[64];
    strcpy(name, current_state->definition->game_name);

    char just_name[64];
    int i = 0;
    while(name[i] != '\0' && name[i] != '.'){
        just_name[i] = name[i];
        i++;
    }
    just_name[i] = '\0';

    char path[128];
    snprintf(path, 128, "saves/%s.txt", just_name);

    FILE *f = fopen(path, "w");
    if (f == NULL) return false;

    fprintf(f, "%s\n", name);

    for(int i = 0; i < current_state->pile_count; i++){
        if(current_state->table_piles[i]->num_cards > 0){
            Card *current_card = current_state->table_piles[i]->head;
            while (current_card != NULL){
                char suit[3];
                char rank[4];
                translate_suit(suit, current_card->suit);
                translate_rank(rank, current_card->rank);
                fprintf(f,"%s%s ", rank, suit);
                current_card = current_card->next;
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return true;
}

void translate_suit(char *char_suit, Suit suit){
    switch (suit) {
        case SUIT_HEART:
            strcpy(char_suit, "H");
            break;
        case SUIT_SPADE:
            strcpy(char_suit, "S");
            break;
        case SUIT_CLUB:
            strcpy(char_suit, "C");
            break;
        case SUIT_DIAMOND:
            strcpy(char_suit, "D");
            break;
        default:
            break;
    }
}

void translate_rank(char *char_rank, Rank rank){
    switch (rank) {
        case RANK_ACE:
            strcpy(char_rank, "A");
            break;
        case RANK_JACK:
            strcpy(char_rank, "J");
            break;
        case RANK_QUEEN:
            strcpy(char_rank, "Q");
            break;
        case RANK_KING:
            strcpy(char_rank, "K");
            break;
        default:
            char value[3];
            snprintf(value, 3, "%d", rank + 1);
            strcpy(char_rank, value);
            break;
    }
}
