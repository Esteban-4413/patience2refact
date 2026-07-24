# Patience Engine - Refactoring & Development Plan

## 1. Architectural Vision

The goal is to evolve the current Solitaire engine into a highly optimized, professional-grade C application. We will transition from static arrays and character-based card representations to **Non-Intrusive Linked Lists** and **Enums**.
Crucially, we will **retain the custom DSL parser** that makes this engine generic and superior to hardcoded implementations.

## 2. What We Keep (The Core)

- **The DSL Parser (`parser.c` & `gamedef.h`):** The ability to read `.paciencia` files dynamically is our strongest asset.
- **Ncurses UI & Mouse Handling (`display.c`, `input.c`):** The visual interface and event loop are already solid.
- **Architecture Modules:** The modular separation of concerns (Parser, GameState, Moves, Save/Load, UI) remains the same.
- **Save/Load & Undo Concepts:** The logic flow remains, but the underlying data serialization will be updated to handle pointers.

## 3. What Needs Refactoring

- **Card Representation:** Move from `char` combinations (`'10'`, `'S'`) to strict `enums` (`Rank`, `Suit`).
- **Stack Representation (`PILHA`):** Move from `CARTAS pilha[52]` (static arrays) to a pointer-based Linked List (`card *topo`).
- **Movement Logic (`move.c`):** Replace $O(N)$ array shifting loops with $O(1)$ pointer reassignment.
- **Validation Rules:** Map DSL flags (e.g., `~`, `v`) directly to highly optimized atomic C functions (`is_alternate_color`, `is_in_sequence`).

## 4. Development Plan (Step-by-Step)

### Phase 1: Core Data Structures (Foundation)

- [ ] Define `Suit` and `Rank` enums in `card.h`.
- [ ] Create the `card` struct as a linked list node (`struct card *next_card`).
- [ ] Refactor `PILHA` in `gamestate.h` to hold a `card *topo` pointer and an integer `tamanho`.
- [ ] Implement atomic list operations in `stack.c`: `push_card`, `pop_card`, `move_sublist(source, dest, count)`.

### Phase 2: Adaptation of the Game State & Parser

- [ ] Update `le_carta` to convert string inputs directly into Enum values.
- [ ] Update `parse_linha_pilha` to build linked lists instead of filling static arrays.
- [ ] Implement the atomic validation functions (`is_black`, `is_alternate_color`, `is_in_sequence`).

### Phase 3: Movement & Engine Logic

- [ ] Refactor `move.c`. Moving a stack of cards should now only involve changing the `next_card` pointer of the boundary cards.
- [ ] Map DSL constraints to the new atomic validation functions.
- [ ] Optimize `verifica_vitoria` to check the `tamanho` variable of the target stacks.

### Phase 4: Save/Load & Undo Systems

- [ ] Refactor `save_game` to traverse the linked list from bottom to top and map Enums back to strings (`10S`, `AH`) for the `.txt` output.
- [ ] Refactor `undo.c`. If using a deep copy for the history, implement a `clone_list` function to safely copy the state pointers.

### Phase 5: UI Integration & Debugging

- [ ] Update `printPilhas` in `display.c` to traverse the linked list for rendering instead of accessing `pilha[i]`.
- [ ] Implement a debug mode in `main.c` with a fixed seed (`srand(123)`) to easily reproduce edge cases during testing.
- [ ] Run Valgrind or AddressSanitizer to ensure no memory leaks occur with the new linked lists (ensure every `malloc` has a `free`).

## 6. Immediate TO-DOs (Next Session)

- [ ] Finish writing the `stack.c` linked list helpers.
- [ ] Test the `push` and `pop` functions in isolation (create a temporary `test_linked_list.c`) before plugging them into the game engine.
