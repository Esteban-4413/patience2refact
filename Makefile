jogo: card.o game_state.o parser.o stack.o undo.o cli.o src/main.c
	gcc -Wall -g $^ -o $@ -lncursesw

mac: card.o game_state.o parser.o stack.o undo.o cli.o src/main.c
	gcc -Wall -g $^ -o jogo -lncurses

card.o: src/cards.c
	gcc -Wall -g $^ -c -o $@

game_state.o: src/game_state.c
	gcc -Wall -g $^ -c -o $@

parser.o: src/parser.c
	gcc -Wall -g $^ -c -o $@

stack.o: src/stack.c
	gcc -Wall -g $^ -c -o $@

undo.o: src/undo.c
	gcc -Wall -g $^ -c -o $@

cli.o: src/cli.c
	gcc -Wall -g $^ -c -o $@

clean:
	-rm -f card.o game_state.o parser.o stack.o undo.o jogo save.txt cli.o jogo.dSYM
