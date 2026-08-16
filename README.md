# Patience's Lab
A collection of classic solitaire games (Patiences) developed in C using the `ncurses` graphics library. The game features a visual console interface, and a command-based control system for an agile gaming experience.

Originally developed as a university project for Laboratórios de Informática II / Laboratórios de Programação II at Universidade do Minho, this repository contains a refactored version of the original project, with a focus on code organization, modularity, and maintainability.

## Installation and requirements
To compile and run the game, you need to have a C compiler (like for example `gcc`) and the `ncurses` library installed.

Clone this repo to your local machine:

```bash
git clone https://github.com/Esteban-4413/patience2refact
cd patience2refact
```

Compile the game using make:

```bash
make
```

And now run the game:

```bash
./patience
```

## How to play
Menu navigation is handled using the arrow keys (or the `j`/`k` keys like the vim motions) and the `Enter` key.

### Main commands
The game is currently controlled through commands typed into the terminal. Mouse controls are not implemented yet.

- Moving a single card: `mv [source pile] [destination]`
  - Example: `mv 1 2` moves the top card from pile 1 to pile 2.
- Move a sequence of cards: `mv [source pile]|[index from where the sequence starts (from up to buttom)] [destination]`
  - Example: `mv 1|3 2` moves a valid sequence starting from the 3rd card in pile 1 to pile 2.
- Ask for a hint: `hint`
  - The system will suggest a valid move (you have a limit of 3 hints per game).
- Undo move: `undo`
  - Reverts the last move you made.
- Save game: `save`
  - Saves your current progress along with your statistics (time, hints, moves) and returns to the main menu.
- Quit: `quit`
  - Leaves the current game

## Included games
The game engine reads the configuration for each solitaire variant from files with the `.paciencia` extension. Currently, it includes the following games:
1. Klondike: The all-time classic solitaire. Build the 4 foundations by suit and move cards on the tableau by alternating colors.

2. FreeCell: An open and highly strategic game. Use your 4 free cells to rearrange cards and solve the board.

3. Golf: A fast-paced sequential matching game. Clear the board by stacking cards that are exactly one rank higher or lower than the waste pile card, regardless of suit.

4. Simple Simon: A Spider-like variant. The goal is to build 4 complete descending sequences on the board of the same suit in order to remove them.

## Architecture
The game engine is designed around a generic representation of solitaire games.

Instead of implementing each game as a separate hardcoded game mode, the rules
for each variant are described through `.paciencia` configuration files. The
engine parses these files and builds the corresponding game state and valid
moves dynamically.

This allows new solitaire variants to be added by creating a new configuration
file rather than rewriting the game logic.

## Adding a new solitaire
Solitaire variants are defined using `.paciencia` files inside the
`paciencias/` directory.

A configuration file describes:

- The number of decks
- The types of piles
- The initial state of each pile
- Valid moves between pile types
- Automatic moves
- Victory conditions

This means that adding a new variant mainly consists of defining its rules in
a new configuration file.
