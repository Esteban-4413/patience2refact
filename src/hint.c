#include "../include/hint.h"

MoveList get_valid_moves(Game_state *current_state){ // set the moveList with all possible moves
    MoveList move_list;
    move_list.count = 0;

    moveRule *rules = current_state->definition->rules;
    int rules_count = current_state->definition->rule_count;

    for (int i = 0; i < rules_count; i++){
        moveRule current_rule = rules[i];
       if (!current_rule.is_auto){
           for (int j = 0; j < current_state->pile_count; j++){
               Pile *src = current_state->table_piles[j];
               if ( src->num_cards > 0 && (strcmp(src->pile_class->name, current_rule.src_pile) == 0)){
                   current_state->move.src_pile = j;
                   for (int k = 0; k < src->num_cards; k++){
                       current_state->move.column_out = k;
                       for (int l = 0; l < current_state->pile_count; l++){
                           if(strcmp(current_state->table_piles[l]->pile_class->name, current_rule.dest_pile) == 0){
                               current_state->move.dest_pile = l;
                               if(is_move_valid(current_state)){
                                   move_list.moves[move_list.count].move = current_state->move;
                                   move_list.moves[move_list.count].best_score = (-1);
                                   move_list.count++;
                               }
                           }
                       }
                   }
               }
           }
        }
    }

    set_move(current_state);

    return move_list;
}


void push_hint (HintStack *stack, StackNode node){
    stack->nodes[stack->top] = node;
    stack->top++;
}

StackNode pop_hint(HintStack *stack){
    stack->top --;
    return (stack->nodes[stack->top]);
}

MoveList generate_hints(Game_state *current_state){
    HintStack hint_stack;
    hint_stack.top = 0;

    MoveList root_moves = get_valid_moves(current_state);

    // Inicializa a stack com a root
    Game_state root_state = *current_state;
    StackNode root;
    root.state = &root_state;

    push_hint(&hint_stack, root);

    for(int i = 0; i < root_moves.count; i++){
        Game_state copy = *(hint_stack.nodes[hint_stack.top].state);
        copy.move = root_moves.moves[i].move;
        do_move(&copy);

        StackNode temp;
        temp.state = &copy;
        temp.current_move_idx = i;
        temp.valid_moves = get_valid_moves(temp.state);
        temp.best_score = win_codition(temp.state);
    }

    //


    return root_moves;
}
