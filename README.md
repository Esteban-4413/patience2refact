# Patience's lab

A collection of classic solitaire games (Patiences) developed in C using the `ncurses` graphics library. The game features a visual console interface, and a command-based control system for an agile gaming experience.

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
Mouse is not implemented yet so in order to play you type the commands.

- Moving a single card: `mv [source pile] [destination]`
  - Example: `mv 1 2` moves the top card from pile 1 to pile 2.
- Move a sequence of cards: `mv [source pile]|[index from where the sequence starts (from up to buttom)] [destination]`
  - Example: `mv 1|3 2` moves a valid sequence of cards from pile 1 (starting 3 cards down) to pile 2.
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
