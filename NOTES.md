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
- **Stack Representation (`PILHA`):** Move from `CARTAS pilha[52]` (static arrays) to a pointer-based Linked List (`card *head`).
- **Movement Logic (`move.c`):** Replace O(N) array shifting loops with O(1) pointer reassignment.
- **Validation Rules:** Map DSL flags (e.g., `~`, `v`) directly to highly optimized atomic C functions using **Bitwise Flags** (`uint32_t`).

## 4. Development Plan (Step-by-Step)

### Phase 1: Core Data Structures (Foundation) - [COMPLETED]

- [x] Define `Suit` and `Rank` enums in `cards.h`.
- [x] Create the `card` struct as a linked list node (`struct card *next`).
- [x] Refactor stack in `stack.h` (`pile`) to hold a `card *head` pointer, `num_cards`, and `uint32_t rules`.
- [x] Implement atomic list operations in `stack.c`: `push`, `pop`, `shift`, `unshift`, `make_card`.

### Phase 2: Adaptation of the Game State & Parser - [IN PROGRESS]

- [x] Separate concerns into `gamedef.h` (Static Rules/Manual) and `game_state.h` (Dynamic Board).
- [x] Implement Bitwise Flags (`F_SEQUENCE`, `F_DESCENDING`, etc.) to translate the DSL string rules.
- [x] Update `parser.c` to parse `.paciencia` files completely without `realloc` (using static arrays in `GameDefinition`).
- [x] Implement History/Undo as a circular buffer array.
- [ ] Implement `loader.c`: Build the physical table (List of `pile`s) based on `GameDefinition` `INIT` commands.
- [ ] Complete `game_state.c`: Functions to initialize the dynamic board (`game_state`), populate the `STOCK` with 52 cards (`fill_deck`), and shuffle.

### Phase 3: Movement & Engine Logic

- [ ] Refactor `move.c`. Moving a stack of cards should now only involve changing the `next` pointer of the boundary cards.
- [ ] Optimize `verifica_vitoria` to check the `target_card_count` inside `win_conditions`.
- [ ] Map Bitwise Flags constraints to the new atomic validation functions (e.g., `if (rules & F_DESCENDING)`).

#### Phase 3.0: Validate a move
- [ ] Based on the ints given by the `move.src_pile` and the `move.dest_pile` set the pointers to acctual piles `source` and `destiny`, repectively. 
- [ ] With the `move.column_out` we have the exact column where the card that was cliked is. Set a pointer that poinst exactly to the card that was clicked(`peek_card_at`). 
- [ ] Also setting a pointer to the card on top of the pile `destiny` is necessarie.
- [ ] Working with linked lists makes necessary to not just keep a pointer in the card cliked(in column n), but also in the one in the column n-1 (so the next_card can be set as NULL
 if the move is valid).
- [ ] Implement a function that before taking care off the cards to be moved, verificade if the PileClass
- [ ] Having a way to directly access the cards we need, we can then run the flags test on them.  

### Phase 4: Save/Load & Undo Systems

- [ ] Refactor `undo.c` using the circular buffer. Save `src_pile`, `dest_pile`, and `card_count` to easily revert pointer logic.
- [ ] Refactor `save_game` to traverse the linked list from bottom to top and map Enums back to strings (`10S`, `AH`) for the `.txt` output.

### Phase 5: UI Integration & Debugging

- [ ] Update `printPilhas` in `display.c` to traverse the linked list for rendering instead of accessing `pilha[i]`.
- [ ] Implement a debug mode in `main.c` with a fixed seed (`srand(123)`) to easily reproduce edge cases during testing.
- [ ] Run Valgrind or AddressSanitizer to ensure no memory leaks occur with the new linked lists (ensure every `malloc` has a `free`).

## 6. Immediate TO-DOs (Next Session)

- [ ] Create `loader.c` to consume the `GameDefinition` pointer from the parser.
- [ ] Implement `fill_deck` and `shuffle` in `game_state.c` to generate the physical cards (`make_card`) before distributing them.
- [ ] Connect the dynamic `game_state` with the parsed `.paciencia` logic.
