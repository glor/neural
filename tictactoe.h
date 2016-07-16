typedef struct {
	int field[3][3];
} board_t; 

int getBoard(board_t* board, int x, int y, int player);
void init_board(board_t* board);
board_t* make_board();
int checkMove(board_t* board, int x, int y);
int getPossibles(board_t* board, int* moves);
int doMove(board_t* board, int player, int move);
int won(board_t* board);
int isOver(board_t* board);
void printBoard(board_t* board);