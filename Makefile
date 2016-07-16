CC=clang
all:
	$(CC) -o network network.c
run:
	./network
cleanttt:
	touch tictactoe
	rm tictactoe
tictactoe: cleanttt
	$(CC) -o tictactoe tttmain.c
