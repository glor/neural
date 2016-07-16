#include <stdio.h>
#include <stdlib.h>

#define makeArray(board,boardvec) int (*board)[3] = (int (*)[3]) boardvec;

typedef struct {
	int field[3][3];
} board_t; 

int getBoard(board_t* board, int x, int y, int player) {
	return player - board->field[x][y];
}

void init_board(board_t* board) {
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			board->field[i][j] = 0;
}

board_t* make_board() {
	board_t* board = malloc(sizeof(board_t));
	init_board(board);
	return board;
}

int checkMove(board_t* board, int x, int y) {
	if(board->field[x][y]!=0) {
		return 0;
	} else {
		return 1;
	}
}

// returns length, writes possibles to array
int getPossibles(board_t* board, int* moves) {
	int pos = 0;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			if(checkMove(board,i,j)) {
				moves[pos++] = i*3 + j;
			}
	return pos;
}

int doMove(board_t* board, int player, int move) {
	player++;
	int x = move/3;
	int y = move%3;
	if(checkMove(board,x,y)) {
		board->field[x][y] = player;
		return 1;
	} else {
		return 0;
	}
}

int won(board_t* board) {
	for(int i=0; i<3; i++) {
		if(board->field[i][0] == board->field[i][1] && board->field[i][1] == board->field[i][2])
			return board->field[i][0];
		if(board->field[0][i] == board->field[1][i] && board->field[1][i] == board->field[2][i])
			return board->field[0][i];
	}
	if(board->field[0][0] == board->field[1][1] && board->field[1][1] == board->field[2][2]) {
		return board->field[1][1];
	}
	if(board->field[0][2] == board->field[1][1] && board->field[1][1] == board->field[2][0]) {
		return board->field[1][1];
	}
	return 0;
}

int isOver(board_t* board) {
	if(won(board))
		return 1;
	int moves[9];
	if(getPossibles(board, moves) == 0)
		return 1;
	return 0;
}

void printBoard(board_t* board) {
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			/*switch(board->field[i][j]) {
				case 1:
					putchar('X');
					break;
				case 2:
					putchar('O');
					break;
				case 0:
					putchar(' ');
					break;
				default:
					exit(1);
			}*/
			printf("%d", board->field[i][j]);
			putchar(' ');
			if(j!=2)
				putchar('|');
		}
		putchar('\n');
		if(i!=2)
			puts("--+--+--");
	}
}
