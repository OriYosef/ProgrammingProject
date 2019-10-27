/*
 * SPCHGame.c
 *
 *  Created on: Jun 4, 2017
 *      Author: Calypso
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SPCHArrayList.h"


//Definitions
#define SP_CH_GAME_N_ROWS 8
#define SP_CH_GAME_N_COLUMNS 8
#define SP_CH_GAME_N_MAX_PREV_MOVES 6
#define SP_CH_GAME_EMPTY_ENTRY '_'

typedef struct sp_CH_game_t{
	int game_mode;       						// 1 = one player. 2 - two players
	int difficulty;								// 1-5
	int user_color;     			   			// in 1 mode. 1 for white. 0 for black.
	int currentPlayer;							// 1 = white, 0 = black
	char gameBoard[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS];
	bool castle[6];									// [white left knight, white king, white right knight, b l knight, b k, b r knight]
	SPArrayList* prevMoves;							// history
	int score;
} SPCHGame;

/**
 * Type used for returning massages from game functions
 */
typedef enum sp_CH_game_message_t {
	SP_CH_GAME_INVALID_MOVE,					//0
	SP_CH_GAME_INVALID_ARGUMENT,
	SP_CH_GAME_SUCCESS,
	SP_CH_GAME_NO_HISTORY,						//3
	SP_CH_GAME_WHITE_WINNER,
	SP_CH_GAME_INVALID_MOVE_SELF_CHECK,
	SP_CH_GAME_TIE,								//6
	SP_CH_GAME_NO_TIE_WINNER,
	SP_CH_GAME_BLACK_WINNER,
	SP_CH_GAME_SUCCESS_AND_PROMOTION,			//9
	SP_CH_GAME_INVALID_MOVE_NOT_PREV_MATE,
	SP_CH_GAME_INVALID_ARGUMENT_NOT_USER_PIECE,
	SP_CH_GAME_CASTLE_NOT_A_ROOK,				//12
	SP_CH_GAME_SUCCESS_AND_WHITE_WINNER,
	SP_CH_GAME_SUCCESS_AND_BLACK_WINNER,
} SP_CH_GAME_MESSAGE;


/**
 * this function initialize the game board by the rules of Chess
 */
void InitializeBoard(SPCHGame* newGame){
	 for (int i = 2; i < SP_CH_GAME_N_ROWS-2; i++){
	 		for (int j = 0; j < SP_CH_GAME_N_COLUMNS; j++){
	 			newGame->gameBoard[i][j] = SP_CH_GAME_EMPTY_ENTRY;
	 		}
	 	}
	 	for (int j = 0; j < SP_CH_GAME_N_COLUMNS; j++){
	 		newGame->gameBoard[1][j] = 'm';
	 		newGame->gameBoard[6][j] = 'M';
	 	}
	 	newGame->gameBoard[0][0] = 'r';
	 	newGame->gameBoard[0][1] = 'n';
	 	newGame->gameBoard[0][2] = 'b';
	 	newGame->gameBoard[0][3] = 'q';
	 	newGame->gameBoard[0][4] = 'k';
	 	newGame->gameBoard[0][5] = 'b';
	 	newGame->gameBoard[0][6] = 'n';
	 	newGame->gameBoard[0][7] = 'r';
	 	newGame->gameBoard[7][0] = 'R';
	 	newGame->gameBoard[7][1] = 'N';
	 	newGame->gameBoard[7][2] = 'B';
	 	newGame->gameBoard[7][3] = 'Q';
	 	newGame->gameBoard[7][4] = 'K';
	 	newGame->gameBoard[7][5] = 'B';
	 	newGame->gameBoard[7][6] = 'N';
	 	newGame->gameBoard[7][7] = 'R';
return;
}



/**
 * Creates a new game by parameters.
 *
 * @return
 * NULL if a memory allocation failure occurs.
 * Otherwise, a new game instant is returned.
 */
SPCHGame* spCHGameCreate(int mode, int diff, int player, int userColor){
	SPCHGame *newGame = (SPCHGame*)malloc(sizeof(SPCHGame)); 						//creating a new game instance
	if(newGame == NULL){
		printf("ERROR: spCHGameCreate memory allocation has failed\n"); fflush (stdout);
		return NULL;
	}
	newGame->prevMoves = spArrayListCreate(SP_CH_GAME_N_MAX_PREV_MOVES);
	while (newGame->prevMoves == NULL)												// protective programming
		newGame->prevMoves = spArrayListCreate(SP_CH_GAME_N_MAX_PREV_MOVES);
	if(newGame->prevMoves == NULL){
		printf("ERROR: spCHGameCreate memory allocation has failed\n"); fflush (stdout);
		free(newGame);
		return NULL;
	}
	newGame->currentPlayer = player;
	newGame->difficulty = diff;
	newGame->game_mode = mode;
	newGame->user_color = userColor;
	newGame->score = 0;
	for (int i=0; i<6; i++){
		newGame->castle[i] = true;
	}
	InitializeBoard(newGame);
	return newGame;
}


/**
 *	Creates a copy of a given game.
 *	All new copy variables has the same parameters as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPCHGame* spCHGameCopy(SPCHGame* src){
	if(src == NULL){
		printf("ERROR: spCHGameCopy null pointer input\n");fflush (stdout);
		return NULL;
	}
	SPCHGame *gameCopy = spCHGameCreate(src->game_mode, src->difficulty, src->currentPlayer ,src->user_color);
	while (gameCopy == NULL)																		// protective programming
		gameCopy = spCHGameCreate(src->game_mode, src->difficulty, src->currentPlayer ,src->user_color);
	for (int i = 0; i < SP_CH_GAME_N_ROWS; i++){
		for(int j = 0; j < SP_CH_GAME_N_COLUMNS; j++)
			gameCopy->gameBoard[i][j] = src->gameBoard[i][j];
	}
	spArrayListDestroy(gameCopy->prevMoves);
	gameCopy->prevMoves = spArrayListCopy(src->prevMoves);
	gameCopy->score = src->score;
	for (int i=0; i<6; i++){
		gameCopy->castle[i] = src->castle[i];
	}
	return gameCopy;
}

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spCHGameDestroy(SPCHGame* src){
	if (src == NULL){
		printf("ERROR: spCHGameDestroy pointer input\n");fflush (stdout);
		return;
	}else{												//adding destrtoy list
		spArrayListDestroy(src->prevMoves);
		free(src);
	}
}

/**
 * return true if in the <row,col> there is a piece of the white player.
 * otherwise return false
 */
bool spCHWhite(SPCHGame* src, int row, int col){
	if ((src->gameBoard[row][col] == 'm') || (src->gameBoard[row][col] == 'k')||
			(src->gameBoard[row][col] == 'r') || (src->gameBoard[row][col] == 'b')||
			(src->gameBoard[row][col] == 'n')|| (src->gameBoard[row][col] == 'q'))
		return true;
	return false;
}

/**
 * return true if in the <row,col> there is a piece of the black player.
 * otherwise return false
 */
bool spCHBlack(SPCHGame* src, int row, int col){
	if ((src->gameBoard[row][col] == 'M') || (src->gameBoard[row][col] == 'K')||
			(src->gameBoard[row][col] == 'R') || (src->gameBoard[row][col] == 'B')||
			(src->gameBoard[row][col] == 'N')|| (src->gameBoard[row][col] == 'Q'))
		return true;
	return false;
}

/**
 * change the current player of the input game
 */
void spCHChangePlayer(SPCHGame* src){
	if (src->currentPlayer==1)
		src->currentPlayer = 0;
	else
		src->currentPlayer = 1;
}

/**
 * the function return true if in the <row,col> index there is a piece of the current player of the game.
 * otherwise return false
 */
bool spCHGamePlayerPiece(SPCHGame* src, int row, int col){
	if (src == NULL){
		printf("ERROR: spCHGamePlayerPiece null pointer input\n");fflush (stdout);
		return false;
	}
	if ((src->currentPlayer==1) && spCHWhite(src, row, col))
		return true;
	if ((src->currentPlayer==0) && spCHBlack(src, row, col))
			return true;
	return false;
}

/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible pawn move of the current player.
 * otherwise return false
 */
bool spCHGamePawnMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGamePawnMove null pointer input\n");fflush (stdout);
		return false;
	}
	if ((move[0] == move[2])&&(move[1] == move[3]))											// staying in the place is not legal
		return false;
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] == 'm'){
			if (move[0] == 1){																// first move is one or two to infront
				if ( (move[2] == 2) && (move[3] == move[1])&&
						(src->gameBoard[2][move[1]] == SP_CH_GAME_EMPTY_ENTRY))
					return true;
				if ( (move[2] == 3) && (move[3] == move[1])&&
						(src->gameBoard[2][move[1]] == SP_CH_GAME_EMPTY_ENTRY) &&
						(src->gameBoard[3][move[1]] == SP_CH_GAME_EMPTY_ENTRY))
					return true;
			}
			if ( (move[2] == (move[0] +1) ) && (move[3] == move[1]) &&						// moving one to infront
				(src->gameBoard[move[0]+1][move[1]] == SP_CH_GAME_EMPTY_ENTRY))
					return true;
			if ( (move[2] == (move[0] +1) ) && (move[3] == (move[1]+1))&&					// eating a opponent piece to the right
				(spCHBlack(src, move[2], move[3])))
								return true;
			if ( (move[2] == (move[0] +1) ) && (move[3] == (move[1]-1))&&					// eating a opponent piece to the left
				(spCHBlack(src, move[2], move[3])))
								return true;
		}
	}else{
		if (src->gameBoard[move[0]][move[1]] == 'M'){										// the same cases for black player
			if (move[0] == 6){
				if ( (move[2] == 5) && (move[3] == move[1])&&
						(src->gameBoard[5][move[1]] == SP_CH_GAME_EMPTY_ENTRY))
					return true;
				if ( (move[2] == 4) && (move[3] == move[1])&&
						(src->gameBoard[5][move[1]] == SP_CH_GAME_EMPTY_ENTRY) &&
						(src->gameBoard[4][move[1]] == SP_CH_GAME_EMPTY_ENTRY))
					return true;
			}
			if ( (move[2] == (move[0] - 1) ) && (move[3] == move[1]) &&
				(src->gameBoard[move[0]-1][move[1]] == SP_CH_GAME_EMPTY_ENTRY))
					return true;
			if ( (move[2] == (move[0] -1) ) && (move[3] == (move[1]-1))&&
				(spCHWhite(src, move[2], move[3])))
					return true;
			if ( (move[2] == (move[0] - 1) ) && (move[3] == (move[1]+1))&&
				(spCHWhite(src, move[2], move[3])))
					return true;
		}
	}
	return false;
}

/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible bishop move of the current player.
 * otherwise return false
 */
bool spCHGameBishopMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameBishopMove null pointer input\n");fflush (stdout);
		return false;
	}
	int numRow = abs(move[2]-move[0]);
	int numCol = abs(move[3]-move[1]);
	if (numRow != numCol)																				// verify that is a diogonal move
		return false;
	if ((move[0] == move[2])&&(move[1]== move[3]))														// staying in the place is nor legal
		return false;
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] == 'b'){
			if (move[2]>move[0]){																	// going up
				if (move[3] > move[1]){																	// going right
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]+i][move[1]+i] != SP_CH_GAME_EMPTY_ENTRY )			// verify that the way is free
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHBlack(src, move[2], move[3])))
						return true;
				}else{																					//going left
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]+i][move[1]-i] != SP_CH_GAME_EMPTY_ENTRY )			// verify that the way is free
								return false;
							}
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
								(spCHBlack(src, move[2], move[3])))
							return true;
					}
			}else{																					//going down
				if (move[3] > move[1]){																	// going right
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]-i][move[1]+i] != SP_CH_GAME_EMPTY_ENTRY )			// verify that the way is free
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHBlack(src, move[2], move[3])))
						return true;
				}else{																					//going left
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]-i][move[1]-i] != SP_CH_GAME_EMPTY_ENTRY )	 		// verify that the way is free
								return false;
							}
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
								(spCHBlack(src, move[2], move[3])))
							return true;
					}
				}
			}
	}else{
		if (src->gameBoard[move[0]][move[1]] == 'B'){													// same for a black player
			if (move[2]>move[0]){				// going up
				if (move[3] > move[1]){			// going right
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]+i][move[1]+i] != SP_CH_GAME_EMPTY_ENTRY )
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
						return true;
				}else{							//going left
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]+i][move[1]-i] != SP_CH_GAME_EMPTY_ENTRY )
								return false;
							}
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
								(spCHWhite(src, move[2], move[3])))
							return true;
					}
			}else{								//going down
				if (move[3] > move[1]){			// going right
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]-i][move[1]+i] != SP_CH_GAME_EMPTY_ENTRY )
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
						return true;
				}else{							//going left
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]-i][move[1]-i] != SP_CH_GAME_EMPTY_ENTRY )
								return false;
							}
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
								(spCHWhite(src, move[2], move[3])))
							return true;
					}
				}
			}
		}
	return false;
}

/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible rook move of the current player.
 * otherwise return false
 */
bool spCHGameRookMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameRookMove null pointer input\n");fflush (stdout);
		return false;
	}
	int numRow = abs(move[2]-move[0]);
	int numCol = abs(move[3]-move[1]);
	if ((move[0] == move[2])&&(move[1] == move[3]))													// staying in the place is not legal
		return false;
	if ((move[0] != move[2])&&(move[1]!=move[3]))													// verify that is straight move
		return false;
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] == 'r'){
			if (move[2]==move[0]){																	// going ofki
				if (move[3] > move[1]){																	// going right
					for (int i=1; i<numCol; i++){
						if (src->gameBoard[move[0]][move[1]+i] != SP_CH_GAME_EMPTY_ENTRY )				// way is free
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHBlack(src, move[2], move[3])))
						return true;
				}else{																					//going left
					for (int i=1; i<numCol; i++){
						if (src->gameBoard[move[0]][move[1]-i] != SP_CH_GAME_EMPTY_ENTRY )
								return false;
							}
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
								(spCHBlack(src, move[2], move[3])))
							return true;
					}
			}else{																				//going meunach
				if (move[2] > move[0]){																// going up
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]+i][move[1]] != SP_CH_GAME_EMPTY_ENTRY )
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHBlack(src, move[2], move[3])))
						return true;
				}else{																				//going down
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]-i][move[1]] != SP_CH_GAME_EMPTY_ENTRY )
								return false;
							}
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
								(spCHBlack(src, move[2], move[3])))
							return true;
					}
				}
			}
	}else{
		if (src->gameBoard[move[0]][move[1]] == 'R'){												// same logic for black player
			if (move[2]==move[0]){				// going ofki
				if (move[3] > move[1]){			// going right
					for (int i=1; i<numCol; i++){
						if (src->gameBoard[move[0]][move[1]+i] != SP_CH_GAME_EMPTY_ENTRY )
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
					return true;
				}else{							//going left
					for (int i=1; i<numCol; i++){
						if (src->gameBoard[move[0]][move[1]-i] != SP_CH_GAME_EMPTY_ENTRY )
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
						(spCHWhite(src, move[2], move[3])))
					return true;
				}
			}else{								//going meunach
				if (move[2] > move[0]){			// going up
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]+i][move[1]] != SP_CH_GAME_EMPTY_ENTRY )
							return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
					return true;
				}else{							//going down
					for (int i=1; i<numRow; i++){
						if (src->gameBoard[move[0]-i][move[1]] != SP_CH_GAME_EMPTY_ENTRY )
								return false;
						}
					if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
					return true;
				}
			}
		}
	}
	return false;
}

/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible knight move of the current player.
 * otherwise return false
 */
bool spCHGameKnightMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameKnightMove null pointer input\n");fflush (stdout);
		return false;
	}
	if ((move[0] == move[2])&&(move[1] == move[3]))															// staying in the place is not legal
		return false;
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] == 'n'){
			if ((move[2]==move[0]+2) && (move[3] == move[1] +1)){					//1 by clock  (two to infront and one to the right)
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]+1) &&(move[3] == move[1] +2)){					//2 by clock  (two to the right and one to infront)
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-1) && (move[3] == move[1] + 2)){					//4 by clock		(etc)
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-2) &&(move[3] == move[1] + 1)){					//5 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-2) && (move[3] == move[1] -1)){					//7 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-1) &&(move[3] == move[1] -2)){					//8 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]+1) && (move[3] == move[1] - 2)){					//10 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]+2) &&(move[3] == move[1] - 1)){					//11 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
		}
	}else{
		if (src->gameBoard[move[0]][move[1]] == 'N'){												// same logic for black player
			if ((move[2]==move[0]+2) && (move[3] == move[1] +1)){					//1 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]+1) &&(move[3] == move[1] +2)){					//2 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-1) && (move[3] == move[1] + 2)){					//4 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-2) &&(move[3] == move[1] + 1)){					//5 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-2) && (move[3] == move[1] -1)){					//7 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]-1) &&(move[3] == move[1] -2)){					//8 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]+1) && (move[3] == move[1] - 2)){					//10 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
			if ((move[2]==move[0]+2) &&(move[3] == move[1] - 1)){					//11 by clock
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
		}
	}
	return false;
}

/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible queen move of the current player.
 * otherwise return false
 */
bool spCHGameQueenMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameQueenMove null pointer input\n");fflush (stdout);
		return false;
	}
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] == 'q'){
			src->gameBoard[move[0]][move[1]] = 'b';										// bishop move
			if (spCHGameBishopMove(src, move)){
				src->gameBoard[move[0]][move[1]] = 'q';
				return true;
			}
			src->gameBoard[move[0]][move[1]] = 'r';										// or rook move
			if (spCHGameRookMove(src, move)){
				src->gameBoard[move[0]][move[1]] = 'q';
				return true;
			}
			src->gameBoard[move[0]][move[1]] = 'q';
		}
	}else{
		if (src->gameBoard[move[0]][move[1]] == 'Q'){									// same for black player
			src->gameBoard[move[0]][move[1]] = 'B';
			if (spCHGameBishopMove(src, move)){
				src->gameBoard[move[0]][move[1]] = 'Q';
				return true;
			}
			src->gameBoard[move[0]][move[1]] = 'R';
			if (spCHGameRookMove(src, move)){
				src->gameBoard[move[0]][move[1]] = 'Q';
				return true;
			}
			src->gameBoard[move[0]][move[1]] = 'Q';
		}
	}
	return false;
}

/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible king move of the current player.
 * otherwise return false
 */
bool spCHGameKingMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameKingMove null pointer input\n");fflush (stdout);
		return false;
	}
	if ((move[0] == move[2])&&(move[1] == move[3]))																// staying in the place is e legal
		return false;
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] == 'k'){
			if (move[2]==move[0]-1){
				for (int i=-1; i<=1; i++){																// down row (3 options)
					if (move[3] == move[1] + i){
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHBlack(src, move[2], move[3])))
						return true;
					}
				}
			}
			if (move[2]==move[0]+1){
				for (int i= -1; i<=1; i++){																// up row (3 options)
					if (move[3] == move[1] + i){
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHBlack(src, move[2], move[3])))
						return true;
					}
				}
			}
			if ((move[2] == move[0])&& ( (move[3] == move[1]-1) || (move[3] == move[1]+1) )){			// right move or left move
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHBlack(src, move[2], move[3])))
				return true;
			}
		}
	}else{
		if (src->gameBoard[move[0]][move[1]] == 'K'){													// same for black player
			if (move[2]==move[0]-1){
				for (int i=-1; i<=1; i++){							// down row
					if (move[3] == move[1] + i){
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
						return true;
					}
				}
			}
			if (move[2]==move[0]+1){
				for (int i= -1; i<=1; i++){							// up row
					if (move[3] == move[1] + i){
						if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
							(spCHWhite(src, move[2], move[3])))
						return true;
					}
				}
			}
			if ((move[2] == move[0])&& ( (move[3] == move[1]-1) || (move[3] == move[1]+1) )){
				if ((src->gameBoard[move[2]][move[3]] == SP_CH_GAME_EMPTY_ENTRY)||
					(spCHWhite(src, move[2], move[3])))
				return true;
			}
		}
	}
	return false;
}


/**
 * the function return true if a move of <move[0],move[1]>
 * to <move[2],move[3]> is a possible simple move (without special cases of legality) of the current player.
 * otherwise return false
 */
bool spCHGameIsSimpleValidMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHIsSimpleValidMove null pointer input\n");fflush (stdout);
		return false;
	}
	if (src->currentPlayer==1){
		switch (src->gameBoard[move[0]][move[1]]){											// checking by the kind of the piece
			case ('m'):																		// if the move is legal
			if (spCHGamePawnMove(src, move))
				return true;
				break;
			case ('b'):
				if (spCHGameBishopMove(src, move))
					return true;
				break;
			case ('r'):
				if (spCHGameRookMove(src, move))
					return true;
				break;
			case ('n'):
				if (spCHGameKnightMove(src, move))
					return true;
				break;
			case ('q'):
				if (spCHGameQueenMove(src, move))
					return true;
				break;
			case ('k'):
				if (spCHGameKingMove(src, move))
					return true;
				break;
		}
	}else{
		switch (src->gameBoard[move[0]][move[1]]){												// same for black player
			case ('M'):
			if (spCHGamePawnMove(src, move))
				return true;
				break;
			case ('B'):
				if (spCHGameBishopMove(src, move))
					return true;
				break;
			case ('R'):
				if (spCHGameRookMove(src, move))
					return true;
				break;
			case ('N'):
				if (spCHGameKnightMove(src, move))
					return true;
				break;
			case ('Q'):
				if (spCHGameQueenMove(src, move))
					return true;
				break;
			case ('K'):
				if (spCHGameKingMove(src, move))
					return true;
				break;
		}
	}
	return false;
}

/**
 * the function return true if the index <row,col>
 * is threaten by the opponent player of the game.
 * otherwise return false
 * the function doesn't change the input game
 */
bool spCHGameDangerIndex(SPCHGame* src, int row, int col){
	if (src == NULL){
		printf("ERROR: spCHGameDangerIndex null pointer input\n");fflush (stdout);
		return false;
	}
	if (!spCHGamePlayerPiece(src,row,col))
		return false;
	int check[4] = {0,0,0,0};
	check[2] = row;
	check[3] = col;
	spCHChangePlayer(src);
	if (check[2]>0){																		// if it isnt the first row
			if (check[3]>0){																// if it isnt the first column
				check[0] = check[2]-1;
				check[1] = check[3]-1;
				while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&		// find the first not empty index
						 (check[0]>0) && (check[1]>0)){ 		 									 //left down diago
					check[0]--;
					check[1]--;
				}
				if (spCHGameIsSimpleValidMove(src, check)){
					spCHChangePlayer(src);
					return true;
				}
			}
			check[0] = check[2]-1;
			check[1] = check[3];
			while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&( check[0]>0)){				// down
				check[0]--;
			}
			if (spCHGameIsSimpleValidMove(src, check)){
				spCHChangePlayer(src);
				return true;
			}
			if (check[3]<7){																	// if it isnt the last column
				check[0] = check[2]-1;
				check[1] = check[3]+1;
				while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&
						(check[0]>0) && (check[1]<7)){													// right down diago
					check[0]--;
					check[1]++;
				}
				if (spCHGameIsSimpleValidMove(src, check)){
					spCHChangePlayer(src);
					return true;
				}
			}
		}
		if (check[3]<7){
			check[0] = check[2];
			check[1] = check[3]+1;
			while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&
					(check[1]<7)){																		// right
				check[1]++;
			}
			if (spCHGameIsSimpleValidMove(src, check)){
				spCHChangePlayer(src);
				return true;
			}
		}
		if (check[2]<7){																			// if it isnt the last row
			if (check[3]<7){																		// if it isnt the last column
				check[0] = check[2]+1;
				check[1] = check[3]+1;
				while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&
						(check[0]<7) && (check[1]<7)){													// right up diago
					check[0]++;
					check[1]++;
				}
				if (spCHGameIsSimpleValidMove(src, check)){
					spCHChangePlayer(src);
					return true;
				}
			}
			check[0] = check[2]+1;
			check[1] = check[3];
			while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&
					(check[0]<7)){																		// up
				check[0]++;
			}
			if (spCHGameIsSimpleValidMove(src, check)){
				spCHChangePlayer(src);
				return true;
			}
			if (check[3]>0){
				check[0] = check[2]+1;
				check[1] = check[3]-1;
				while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&
						(check[0]<7) && (check[1]>0)){													// left up diago
					check[0]++;
					check[1]--;
				}
				if (spCHGameIsSimpleValidMove(src, check)){
					spCHChangePlayer(src);
					return true;
				}
			}
		}
		if (check[3]>0){																			// if it isnt the first column
			check[0] = check[2];
			check[1] = check[3]-1;
			while ((src->gameBoard[check[0]][check[1]] == SP_CH_GAME_EMPTY_ENTRY)&&
					(check[1]>0)){															// left
				check[1]--;
			}
			if (spCHGameIsSimpleValidMove(src, check)){
				spCHChangePlayer(src);
				return true;
			}
		}
		spCHChangePlayer(src);
		if (src->currentPlayer == 1){															// checking all the knight possible moves
			if ((check[2] > 0) && (check[3] > 1) && (src->gameBoard[check[2]-1][check[3]-2] == 'N') ){				//knight
				return true;
			}
			if ((check[2] > 0) && (check[3] < 6) && (src->gameBoard[check[2]-1][check[3]+2] == 'N') ){
				return true;
			}
			if ((check[2] > 1) && (check[3] > 0) && (src->gameBoard[check[2]-2][check[3]-1] == 'N') ){
				return true;
			}
			if ((check[2] > 1) && (check[3] < 7) && (src->gameBoard[check[2]-2][check[3]+1] == 'N') ){
				return true;
			}
			if ((check[2] < 7) && (check[3] > 1) && (src->gameBoard[check[2]+1][check[3]-2] == 'N') ){
				return true;
			}
			if ((check[2] < 7) && (check[3] < 6) && (src->gameBoard[check[2]+1][check[3]+2] == 'N') ){
				return true;
			}
			if ((check[2] < 6) && (check[3] > 0) && (src->gameBoard[check[2]+2][check[3]-1] == 'N') ){
				return true;
			}
			if ((check[2] < 6) && (check[3] < 7) && (src->gameBoard[check[2]+2][check[3]+1] == 'N') ){
				return true;
			}
		}else{
			if ((check[2] > 0) && (check[3] > 1) && (src->gameBoard[check[2]-1][check[3]-2] == 'n') ){				//knight
				return true;
			}
			if ((check[2] > 0) && (check[3] < 6) && (src->gameBoard[check[2]-1][check[3]+2] == 'n') ){
				return true;
			}
			if ((check[2] > 1) && (check[3] > 0) && (src->gameBoard[check[2]-2][check[3]-1] == 'n') ){
				return true;
			}
			if ((check[2] > 1) && (check[3] < 7) && (src->gameBoard[check[2]-2][check[3]+1] == 'n') ){
				return true;
			}
			if ((check[2] < 7) && (check[3] > 1) && (src->gameBoard[check[2]+1][check[3]-2] == 'n') ){
				return true;
			}
			if ((check[2] < 7) && (check[3] < 6) && (src->gameBoard[check[2]+1][check[3]+2] == 'n') ){
				return true;
			}
			if ((check[2] < 6) && (check[3] > 0) && (src->gameBoard[check[2]+2][check[3]-1] == 'n') ){
				return true;
			}
			if ((check[2] < 6) && (check[3] < 7) && (src->gameBoard[check[2]+2][check[3]+1] == 'n') ){
				return true;
			}
		}
	return false;
}

/**
 * the function returns true if the move <move[0],move[1]>
 * to <move[2],move[3]> is a simple valid move to the current player
 * and after execute the move the current player will be in a check.
 * otherwise returns false.
 * the function doesn't change the input game
 */
bool spCHGameSelfCheck(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameSelfCheck null pointer input\n");fflush (stdout);
		return false;
	}
	if (!spCHGameIsSimpleValidMove(src,move)){												// verify first that the move is simple valid move
		return false;
	}
	int check[4] = {0,0,0,0};
	SPCHGame *GameCopy = spCHGameCopy(src);													// playing the move
	GameCopy->gameBoard[move[2]][move[3]] = GameCopy->gameBoard[move[0]][move[1]];
	GameCopy->gameBoard[move[0]][move[1]] = SP_CH_GAME_EMPTY_ENTRY;
	if (GameCopy->currentPlayer==1){														// white turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (GameCopy->gameBoard[row][col] == 'k'){											// finding the white king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}else{																					// black turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (GameCopy->gameBoard[row][col] == 'K'){											// finding the black king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}
	if (spCHGameDangerIndex(GameCopy, check[0], check[1])){										// after the move check if the king is in danger
		spCHGameDestroy(GameCopy);
		return true;
	}
	spCHGameDestroy(GameCopy);
	return false;
}

/**
 * the function get a game and an index. if the index is a current player rook and
 * castling this rook is legal then the function execute the castling
 * and change the current player. and returns success massage
 * otherwise does nothing and returns invalid move massage.
  */
SP_CH_GAME_MESSAGE spCHcastling(SPCHGame* src, int move[2]){
	if (src == NULL){
		printf("ERROR: spCHCastling null pointer input\n");fflush (stdout);
		return 1;
	}
	if (src->currentPlayer == 1){													// verify that the piece is a rook
		if (src->gameBoard[move[0]][move[1]] != 'r')
			return 12;
	}else{
		if (src->gameBoard[move[0]][move[1]] != 'R')
			return 12;
	}
	if ( (move[0]==0)&&(move[1]==0)													// white left rook
			&&(src->castle[0]== true)&&(src->castle[1]== true) 						// king +rook never moved
			&& (src->gameBoard[0][1] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[0][2] == SP_CH_GAME_EMPTY_ENTRY)
			&& (src->gameBoard[0][3] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 0,2)) && (!spCHGameDangerIndex(src, 0,3))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 0,4))  ){                                    	// not in check now
		spArrayListAddFirst2(src->prevMoves, src->gameBoard);
		src->gameBoard[0][0] = SP_CH_GAME_EMPTY_ENTRY;
		src->gameBoard[0][2] = 'k';
		src->gameBoard[0][3] = 'r';
		src->gameBoard[0][4] = SP_CH_GAME_EMPTY_ENTRY;
		src->castle[0] = false;
		src->castle[1] = false;
		spCHChangePlayer(src);
		return 2;
	}
	if ( (move[0]==0)&&(move[1]==7)													// white right rook
			&&(src->castle[2]== true)&&(src->castle[1]== true) 						// king +rook never moved
			&& (src->gameBoard[0][6] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[0][5] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 0,5)) && (!spCHGameDangerIndex(src, 0,6))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 0,4))  ){                                    	// not in check now
		spArrayListAddFirst2(src->prevMoves, src->gameBoard);
		src->gameBoard[0][7] = SP_CH_GAME_EMPTY_ENTRY;
		src->gameBoard[0][6] = 'k';
		src->gameBoard[0][5] = 'r';
		src->gameBoard[0][4] = SP_CH_GAME_EMPTY_ENTRY;
		src->castle[2] = false;
		src->castle[1] = false;
		spCHChangePlayer(src);
		return 2;
	}
	if ( (move[0]==7)&&(move[1]==0)													// black left rook
			&&(src->castle[3]== true)&&(src->castle[4]== true) 						// king +rook never moved
			&& (src->gameBoard[7][1] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[7][2] == SP_CH_GAME_EMPTY_ENTRY)
			&& (src->gameBoard[7][3] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 7,2)) && (!spCHGameDangerIndex(src, 7,3))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 7,4))  ){                                    	// not in check now
		spArrayListAddFirst2(src->prevMoves, src->gameBoard);
		src->gameBoard[7][0] = SP_CH_GAME_EMPTY_ENTRY;
		src->gameBoard[7][2] = 'K';
		src->gameBoard[7][3] = 'R';
		src->gameBoard[7][4] = SP_CH_GAME_EMPTY_ENTRY;
		src->castle[3] = false;
		src->castle[4] = false;
		spCHChangePlayer(src);
		return 2;
	}
	if ( (move[0]==7)&&(move[1]==7)													// while right rook
			&&(src->castle[5]== true)&&(src->castle[4]== true) 						// king +rook never moved
			&& (src->gameBoard[7][6] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[7][5] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 7,5)) && (!spCHGameDangerIndex(src, 7,6))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 7,4))  ){                                    	// not in check now
		spArrayListAddFirst2(src->prevMoves, src->gameBoard);
		src->gameBoard[7][7] = SP_CH_GAME_EMPTY_ENTRY;
		src->gameBoard[7][6] = 'K';
		src->gameBoard[7][5] = 'R';
		src->gameBoard[7][4] = SP_CH_GAME_EMPTY_ENTRY;
		src->castle[4] = false;
		src->castle[5] = false;
		spCHChangePlayer(src);
		return 2;
	}
	return 0; 																			// "Illegal castling move\n"
}

/**
 * the function get a game and an index. if the index is a current player rook and
 * castling this rook is legal then the function returns true
 * otherwise returns false.
 * the function doesn't change the input game
 */
bool spCHPossibleCastling(SPCHGame* src, int move[2]){									// same function with bool return
	if (src == NULL){
		printf("ERROR: spCHPossibleCastling null pointer input\n");fflush (stdout);
		return false;
	}
	if (src->currentPlayer == 1){
		if (src->gameBoard[move[0]][move[1]] != 'r')
			return false;
	}else{
		if (src->gameBoard[move[0]][move[1]] != 'R')
			return false;
	}
	if ( (move[0]==0)&&(move[1]==0)													// white left rook
			&&(src->castle[0]== true)&&(src->castle[1]== true) 						// king +rook never moved
			&& (src->gameBoard[0][1] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[0][2] == SP_CH_GAME_EMPTY_ENTRY)
			&& (src->gameBoard[0][3] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 0,2)) && (!spCHGameDangerIndex(src, 0,3))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 0,4))  ){                                    	// not in check now
		return true;
	}
	if ( (move[0]==0)&&(move[1]==7)													// white right rook
			&&(src->castle[2]== true)&&(src->castle[1]== true) 						// king +rook never moved
			&& (src->gameBoard[0][6] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[0][5] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 0,5)) && (!spCHGameDangerIndex(src, 0,6))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 0,4))  ){                                    	// not in check now
		return true;
	}
	if ( (move[0]==7)&&(move[1]==0)													// black left rook
			&&(src->castle[3]== true)&&(src->castle[4]== true) 						// king +rook never moved
			&& (src->gameBoard[7][1] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[7][2] == SP_CH_GAME_EMPTY_ENTRY)
			&& (src->gameBoard[7][3] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 7,2)) && (!spCHGameDangerIndex(src, 7,3))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 7,4))  ){                                    	// not in check now
		return true;
	}
	if ( (move[0]==7)&&(move[1]==7)													// while right rook
			&&(src->castle[5]== true)&&(src->castle[4]== true) 						// king +rook never moved
			&& (src->gameBoard[7][6] == SP_CH_GAME_EMPTY_ENTRY)						// free way
			&& (src->gameBoard[7][5] == SP_CH_GAME_EMPTY_ENTRY)
			&& (!spCHGameDangerIndex(src, 7,5)) && (!spCHGameDangerIndex(src, 7,6))		// not passing trough danger
			&& (!spCHGameDangerIndex(src, 7,4))  ){                                    	// not in check now
		return true;
	}
	return false; 																	// "Illegal castling move\n"
}


/**
 * the function returns true if the current player is in check and after execute
 * the move <move[0],move[1]> to <move[2],move[3]> the current player will stay in check.
 * otherwise returns false.
 * the function doesn't change the input game
 */
bool spCHGameNotPreventMate(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameNotPreventMate null pointer input\n");fflush (stdout);
		return false;
	}
	int check[4] = {0,0,0,0};
	SPCHGame *GameCopy = spCHGameCopy(src);
	if (GameCopy->currentPlayer==1){														// white turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (GameCopy->gameBoard[row][col] == 'k'){											// finding the white king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}else{																					// black turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (GameCopy->gameBoard[row][col] == 'K'){											// finding the black king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}
	if (!spCHGameDangerIndex(src, check[0], check[1])){										// verify that the king is in danger
		spCHGameDestroy(GameCopy);
		return false;																		// no ches no problem
	}
	GameCopy->gameBoard[move[2]][move[3]] = GameCopy->gameBoard[move[0]][move[1]];			// playing the move
	GameCopy->gameBoard[move[0]][move[1]] = SP_CH_GAME_EMPTY_ENTRY;
	if (GameCopy->currentPlayer==1){														// white turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (GameCopy->gameBoard[row][col] == 'k'){											// finding again the white king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}else{																					// black turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (GameCopy->gameBoard[row][col] == 'K'){											// finding again the black king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}
	if (spCHGameDangerIndex(GameCopy, check[0], check[1])){										// check if the king in danger
		spCHGameDestroy(GameCopy);
		return true;
	}
	spCHGameDestroy(GameCopy);
	return false;
}

/**
 * the function returns true if the move <move[0],move[1]>
 * to <move[2],move[3]> is a move of current player pawn to the last row
 * otherwise returns false.
 * the function doesn't change the input game. the function doesnt check that the move is legal
 */
bool spCHGamePromotion(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGamePromotion null pointer input\n");fflush (stdout);
		return false;
	}
	if (src->currentPlayer==0){
		if ((src->gameBoard[move[2]][move[3]]=='m') && (move[2]==7))								// a pawn move and last row arrived
			return true;
	}else{
		if ((src->gameBoard[move[2]][move[3]]=='M') && (move[2]==0))
			return true;
	}
	return false;
}

/**
 * the function checks if the move <move[0],move[1]>
 * to <move[2],move[3]> is valid move of the current player
 * and returns a suitable massage
 * the function doesn't change the input game
 */
SP_CH_GAME_MESSAGE spCHGameIsValidMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameIsValidMove null pointer input\n");fflush (stdout);
		return 1;
	}
	if (!spCHGamePlayerPiece(src, move[0], move[1]))										// verify that the piece is current player piece
		return 11;
	if (!spCHGameIsSimpleValidMove(src, move)) 												// verify that the move is legal by the kind og the piece
		return 0;
	if (spCHGameSelfCheck(src, move))														// not entering self check case
		return 5;
	if (spCHGameNotPreventMate(src, move))													// not preventing mate
		return 10;
	return 2;

}

/**
 * the function gets a game and index and return the value of the piece in the index
 * the function doesn't change the input game
 */
int SPCHPieceScore(SPCHGame* src, int row, int col){
	if (src == NULL){
		printf("ERROR: SPCHPieceScore null pointer input\n");fflush (stdout);
		return false;
	}
	switch (src->gameBoard[row][col]){
		case 'm':
			return 1;
			break;
		case 'n':
			return 3;
			break;
		case 'b':
			return 3;
			break;
		case 'r':
			return 5;
			break;
		case 'q':
			return 9;
			break;
		case 'k':
			return 100;
			break;
		case 'M':
			return -1;
			break;
		case 'N':
			return -3;
			break;
		case 'B':
			return -3;
			break;
		case 'R':
			return -5;
			break;
		case 'Q':
			return -9;
			break;
		case 'K':
			return -100;
			break;
		case ' ':
			return 0;
			break;
		default:
			break;
	}
	return 0;
}


/**
 * the function gets a game and returns the score of the game.
 * total white value minus total black value
 * the function doesn't change the input game
 */
int SPCHGameScore(SPCHGame* src){
	if (src == NULL){
		printf("ERROR: SPCHGameScore null pointer input\n");fflush (stdout);
		return 1;
	}
	int totalScore = 0;
	for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
		for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
			totalScore += SPCHPieceScore(src, row, col);									// summing all the pieces values
		}
	}
	return totalScore;
}

/**
 * the function gets a game and changing it to the exactly game it was before the last move
 * and returns a suitable massage
 */
SP_CH_GAME_MESSAGE spCHGameUndoPrevMove(SPCHGame* src){
	if (src == NULL){
		printf("ERROR: spCHGameUndoPrevMove null pointer input\n");fflush (stdout);
		return 1;
	}
	if(spArrayListIsEmpty(src->prevMoves))
		return 3;
	for (int row=0; row<SP_CH_GAME_N_ROWS; row++){
		for (int col=0; col<SP_CH_GAME_N_COLUMNS; col++){
			src->gameBoard[row][col] = src->prevMoves->elements[row*SP_CH_GAME_N_ROWS+col];						// copying the last board
		}
	}
	spArrayListDeleteLast(src->prevMoves);																		// updating the history
	src->score = SPCHGameScore(src);										// adding print
	spCHChangePlayer (src);
	return 2;
}

/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. Small letters are for white player.
 *   Capital for black player.
 * @param src - the target game
 * @return
 * SP_CH_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_CH_GAME_SUCCESS - otherwise
 *
 */
SP_CH_GAME_MESSAGE spCHGamePrintBoard(SPCHGame* src){
	if(src == NULL){
		printf("ERROR: spCHGamePrintBoard null pointer input\n");fflush (stdout);
		return 1;
	}
	for(int i = SP_CH_GAME_N_ROWS-1; i >= 0; i--){
		printf("%d| ", i+1); fflush(stdout);
		for(int j = 0; j < SP_CH_GAME_N_COLUMNS; j++ ){
			printf("%c ",src->gameBoard[i][j]);fflush(stdout);
		}
		printf("|\n");fflush(stdout);
	}
	printf("  -----------------\n  ");fflush(stdout);
	for(int i = 0 ; i < SP_CH_GAME_N_ROWS ;i++)
		printf(" %c",65+i);fflush(stdout);
	printf("\n");fflush(stdout);
	return 2;
}

/**
 * the function returns true if there is a possible legal move to the current player
 * otherwise returns false
 * the function doesn't change the input game
 */
bool spCHPosibleMoveExist(SPCHGame* src){
	if(src == NULL){
		printf("ERROR: spCHPosibleMoveExist null pointer input\n");fflush (stdout);
		return 1;
	}
	int check[4] = {0,0,0,0};
	for (int CurRow=0; CurRow<SP_CH_GAME_N_ROWS ; CurRow++){									// check all possible regular moves
		for (int CurCol=0; CurCol<SP_CH_GAME_N_COLUMNS ; CurCol++){
			for (int DesRow=0; DesRow<SP_CH_GAME_N_ROWS; DesRow++){
				for (int DesCol=0; DesCol<SP_CH_GAME_N_COLUMNS; DesCol++){
					check[0] = CurRow;
					check[1] = CurCol;
					check[2] = DesRow;
					check[3] = DesCol;
					if (spCHGameIsValidMove(src, check)==2)
						return true;
				}
			}
		}
	}
	int check2[2] = {0,0};												// checking all castling moves
	check2[0] = 0;
	check2[1] = 0;
	if (spCHPossibleCastling(src,check2))
		return true;
	check2[0] = 0;
	check2[1] = 7;
	if (spCHPossibleCastling(src,check2))
		return true;
	check2[0] = 7;
	check2[1] = 0;
	if (spCHPossibleCastling(src,check2))
		return true;
	check2[0] = 7;
	check2[1] = 7;
	if (spCHPossibleCastling(src,check2))
		return true;
	return false;
}


/**
 * the function returns true if the current player is in check
 * otherwise returns false
 * the function doesn't change the input game
 */
bool spCHCheckForCheck(SPCHGame* src){
	if (src == NULL){
		printf("ERROR: spCHCheckForCheck null pointer input\n");fflush (stdout);
		return false;
	}
	int check[2] = {0,0};
	if (src->currentPlayer==1){														// white turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (src->gameBoard[row][col] == 'k'){											// finding the white king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}else{																					// black turn
		for(int row = 0; row < SP_CH_GAME_N_ROWS; row++){
			for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
				if (src->gameBoard[row][col] == 'K'){											// finding the black king
					check[0] = row;
					check[1] = col;
				}
			}
		}
	}
	if (spCHGameDangerIndex(src, check[0], check[1]))									// check if the king in danger
		return true;
	else
		return false;
}

/**
 * the function gets a game and check if there is a winner or a tie
 * and returns a suitable massage
 * the function doesn't change the input game
 */
SP_CH_GAME_MESSAGE spCHCheckWinnerOrTie(SPCHGame* src){
	if (src == NULL){
		printf("ERROR: spCHCheckWinnerOrTie null pointer input\n");fflush (stdout);
		return 1;
	}
	if (spCHCheckForCheck(src)){															// the current player is in check status
		if (spCHPosibleMoveExist(src)){													// check if there is a legal move
			return 7;																		// continue game
		}else{
			if (src->currentPlayer==1)
				return 8;																	// black won
			else
				return 4;																	// white won
		}
	}else{																					// no ches
		if (!spCHPosibleMoveExist(src))													// no moves -> tie
			return 6;
	}
	return 7;																				// continue game
}

/**
 * the function gets a game and restart it to the default settings
 */
void spCHRestartGame(SPCHGame* src){
	if (src == NULL){
		printf("ERROR: spCHRestartGame null pointer input\n");fflush (stdout);
		return;
	}
	src->prevMoves->actualSize=0;																	// all default values
	src->currentPlayer = 1;
	src->difficulty = 2;
	src->game_mode = 1;
	src->user_color = 1;
	src->score = 0;
	for (int i=0; i<6; i++){
		src->castle[i] = true;
	}
	InitializeBoard(src);
	return;
}


/**
 * the function gets a game and a piece name. if the current player has the full possible number of
 * this piece on the board the function returns true
 * otherwise return false
 * the function doesn't change the game
 */
bool alreadyFull(SPCHGame* game, char* str2){
	if (game == NULL){
		printf("ERROR: alreadyFull null pointer input\n");fflush (stdout);
		return false;
	}
	int queen = 0;
	int rook = 0;
	int bishop = 0;
	int knight = 0;
	if (str2[0] == 'p'){
		return false;
	}
	if (game->currentPlayer==0){												// black turn but the white makes promotion
		for (int row=0; row<SP_CH_GAME_N_ROWS; row++){									// scanning all the board
			for (int col = 0; col< SP_CH_GAME_N_COLUMNS; col++){
				switch (game->gameBoard[row][col]){
					case 'q':
						queen++;
						break;
					case 'r':
						rook++;
						break;
					case 'b':
						bishop++;
						break;
					case 'n':
						knight++;
						break;
					default:
						break;
				}
			}
		}
		if (str2[0] == 'q'){										// already has a queen
			if (queen == 1)
				return true;
			else
				return false;
		}
		if (str2[0] == 'r'){										// already has two rooks
			if (rook == 2)
				return true;
			else
				return false;
		}
		if (str2[0] == 'b'){										// already has two bishops
			if (bishop == 2)
				return true;
			else
				return false;
		}
		if (str2[0] == 'k'){										// already has two knights
			if (knight == 2)
				return true;
			else
				return false;
		}
	}else{																// same logic for black player
		for (int row=0; row<SP_CH_GAME_N_ROWS; row++){
			for (int col = 0; col< SP_CH_GAME_N_COLUMNS; col++){
				switch (game->gameBoard[row][col]){
					case 'Q':
						queen++;
						break;
					case 'R':
						rook++;
						break;
					case 'B':
						bishop++;
						break;
					case 'N':
						knight++;
						break;
					default:
						break;
				}
			}
		}
		if (str2[0] == 'q'){
			if (queen == 1)
				return true;
			else
				return false;
		}
		if (str2[0] == 'r'){
			if (rook == 2)
				return true;
			else
				return false;
		}
		if (str2[0] == 'b'){
			if (bishop == 2)
				return true;
			else
				return false;
		}
		if (str2[0] == 'k'){
			if (knight == 2)
				return true;
			else
				return false;
		}
	}
	return true;
}


/**
 * the function gets a game and possible move.
 * if the move is legal then the function execute the move, change the current player and returns a suitable massage
 * otherwise returns a suitable massage without changing the game
 */
SP_CH_GAME_MESSAGE spCHGameSetMove(SPCHGame* src, int move[4]){
	if (src == NULL){
		printf("ERROR: spCHGameSetMove null pointer input\n");fflush (stdout);
		return 1;
	}
	switch (spCHGameIsValidMove(src, move)){										// verify that the move is legal
		case 1:
			return 1;
			break;
		case 11:
			return 11;
			break;
		case 0:
			return 0;
			break;
		case 5:
			return 5;
			break;
		case 10:
			return 10;
			break;
		case 2:
			if ((move[0] == 0) && (move[1] == 0)){												// if rook or king moves update
				src->castle[0] = false;
			};
			if ((move[0] == 0) && (move[1] == 4)){
				src->castle[1] = false;
			};
			if ((move[0] == 0) && (move[1] == 7)){
				src->castle[2] = false;
			};
			if ((move[0] == 7) && (move[1] == 0)){
				src->castle[3] = false;
			};
			if ((move[0] == 7) && (move[1] == 4)){
				src->castle[4] = false;
			};
			if ((move[0] == 7) && (move[1] == 7)){
				src->castle[5] = false;
			};
			spArrayListAddFirst2(src->prevMoves, src->gameBoard);								// update the history
			src->score -= SPCHPieceScore(src, move[2], move[3]);
			src->gameBoard[move[2]][move[3]] = src->gameBoard[move[0]][move[1]];
			src->gameBoard[move[0]][move[1]] = SP_CH_GAME_EMPTY_ENTRY;
			spCHChangePlayer(src);
			if (spCHCheckWinnerOrTie(src)==4){
				return 13;
			}
			if (spCHCheckWinnerOrTie(src)==8){
				return 14;
			}
			if (spCHGamePromotion(src, move))
				return 9;
			else
				return 2;
			break;
		default:
			break;
	}
	return 1;
}
