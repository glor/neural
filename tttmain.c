#include "tictactoe.c"
#include <time.h>


int main(void) {
	srand(time(NULL));
	int r = rand();
	board_t* board = make_board();
	int numPossibles;
	int possibles[9];
	
	int counter = 0;
	for(int i=0; i<1000; i++) {
		counter = 0;
		init_board(board);
		while(!isOver(board)) {
			numPossibles = getPossibles(board, possibles);
			doMove(board, (counter&1), possibles[abs(rand())%numPossibles]);
			//printBoard(board);
			//putchar('\n');
			counter++;
		}
		printf("%d,", counter);
	} 
}