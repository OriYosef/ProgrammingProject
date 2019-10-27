/*
 * SPCHMiniMax.c
 *
 *  Created on: 15 בספט 2017
 *      Author: ori
 */

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "SPCHGame.h"

typedef enum sp_CH_minimax_message_t {
	SP_CH_MINI_MAX_SUCCESS,
	SP_CH_MINI_MAX_FAIL
} SP_CH_MINI_MAX_MESSAGE;

int RecursiveMiniMax(SPCHGame* src, int depth, int alpha, int beta, int Player);

/**
 * the function implementing a promotion for the AI as one of the option in the minimax algorithm,
 * when the current player in this position in the mini max is white.
 */
int* promotionWhiteCase(SPCHGame *GameCopy, int row, int col, int depth, int alpha, int beta, int Player){
	if (GameCopy == NULL){
		printf("ERROR: promotionWhiteCase null pointer input\n");fflush (stdout);
		return NULL;
	}
	static int result[2] = {0,0};
	result[0] = -1000;
	result[1] = 0;			//result [1] 0-rook, 1-knight, 2-bishop, 3-queen, 4-pawn
	int promote = 0;
	GameCopy->gameBoard[row][col] = 'r';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}
	if (promote > result[0]){
		result[0] = promote;
		result[1] = 0;
	}
	GameCopy->gameBoard[row][col] = 'n';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}if (promote > result[0]){
		result[0] = promote;
		result[1] = 1;
	}
	GameCopy->gameBoard[row][col] = 'b';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}if (promote > result[0]){
		result[0] = promote;
		result[1] = 2;
	}
	GameCopy->gameBoard[row][col] = 'q';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}if (promote > result[0]){
		result[0] = promote;
		result[1] = 3;
	}
	GameCopy->gameBoard[row][col] = 'm';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}if (promote > result[0]){
		result[0] = promote;
		result[1] = 4;
	}
	return result;
}

/**
 * the function implementing a promotion for the AI as one of the option in the minimax algorithm,
 * when the current player in this position in the mini max is black.
 */
int* promotionBlackCase (SPCHGame *GameCopy, int row, int col, int depth, int alpha, int beta, int Player){
	if (GameCopy == NULL){
		printf("ERROR: promotionBlackCase null pointer input\n");fflush (stdout);
		return NULL;
	}
	static int result[2] = {0,0};
	result[0] = 1000;
	result[1] = 0;			//result [1] 0-rook, 1-knight, 2-bishop, 3-queen, 4-pawn
	int promote = 0;
	GameCopy->gameBoard[row][col] = 'R';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}
	if (promote < result[0]){
		result[0] = promote;
		result[1] = 0;
	}
	GameCopy->gameBoard[row][col] = 'N';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}
	if (promote < result[0]){
		result[0] = promote;
		result[1] = 1;
	}
	GameCopy->gameBoard[row][col] = 'B';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}
	if (promote < result[0]){
		result[0] = promote;
		result[1] = 2;
	}
	GameCopy->gameBoard[row][col] = 'Q';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}
	if (promote < result[0]){
		result[0] = promote;
		result[1] = 3;
	}
	GameCopy->gameBoard[row][col] = 'M';
	GameCopy->score = SPCHGameScore(GameCopy);
	switch (spCHCheckWinnerOrTie(GameCopy)){
		case 8:
			promote = -100;
			break;
		case 4:
			promote = 100;
			break;
		default:
			if (GameCopy->difficulty == depth)
				promote = GameCopy->score;
			else
				promote = RecursiveMiniMax(GameCopy, depth+1, alpha, beta,  !Player);
			break;
	}
	if (promote < result[0]){
		result[0] = promote;
		result[1] = 4;
	}
	return result;
}

/**
 * the function implementing a castle for the AI as one of the option in the minimax algorithm,
 * when the current player in this position in the mini max is white.
 */
int* castleWhiteCase(SPCHGame *src, int depth, int alpha, int beta, int Player){
	if (src == NULL){
		printf("ERROR: castleWhiteCase null pointer input\n");fflush (stdout);
		return NULL;
	}
	SPCHGame *GameCopy;// = spCHGameCreate(1,1,0,0);
	static int result[2];
	int result2 = -1000;
	if ( (src->castle[0]== true)&&(src->castle[1]== true) 						// white left rook: king +rook never moved
			&&(src->gameBoard[0][0] == 'r') &&(src->gameBoard[0][4] == 'k')
			&& (src->gameBoard[0][1] == SP_CH_GAME_EMPTY_ENTRY)						// free way
		&& (src->gameBoard[0][2] == SP_CH_GAME_EMPTY_ENTRY)
		&& (src->gameBoard[0][3] == SP_CH_GAME_EMPTY_ENTRY)
		&& (!spCHGameDangerIndex(src, 0,2)) && (!spCHGameDangerIndex(src, 0,3))		// not passing trough danger
		&& (!spCHGameDangerIndex(src, 0,4)) ) {
		GameCopy = spCHGameCopy(src);
		GameCopy->gameBoard[0][0] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->gameBoard[0][2] = 'k';
		GameCopy->gameBoard[0][3] = 'r';
		GameCopy->gameBoard[0][4] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->castle[0] = false;
		GameCopy->castle[1] = false;
		spCHChangePlayer(GameCopy);
		switch (spCHCheckWinnerOrTie(GameCopy)){
			case 8:
				result[0] = -100;
				break;
			case 4:
				result[0] = 100;
				break;
			default:
				if (src->difficulty == depth)
					result[0] = src->score;
				else
					result[0] = RecursiveMiniMax(GameCopy, depth + 1, alpha, beta, !Player );
				break;
		}
		spCHGameDestroy(GameCopy);
	}
	if ( (src->castle[2]== true)&&(src->castle[1]== true) 						//white right rook: king +rook never moved
			&&(src->gameBoard[0][7] == 'k') &&(src->gameBoard[0][4] == 'k')
			&& (src->gameBoard[0][6] == SP_CH_GAME_EMPTY_ENTRY)						// free way
				&& (src->gameBoard[0][5] == SP_CH_GAME_EMPTY_ENTRY)
				&& (!spCHGameDangerIndex(src, 0,5)) && (!spCHGameDangerIndex(src, 0,6))		// not passing trough danger
				&& (!spCHGameDangerIndex(src, 0,4))  ){                                    	// not in check now
		GameCopy = spCHGameCopy(src);
		GameCopy->gameBoard[0][7] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->gameBoard[0][6] = 'k';
		GameCopy->gameBoard[0][5] = 'r';
		GameCopy->gameBoard[0][4] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->castle[1] = false;
		GameCopy->castle[2] = false;
		spCHChangePlayer(GameCopy);
		switch (spCHCheckWinnerOrTie(GameCopy)){
			case 8:
				result2 = -100;
				break;
			case 4:
				result2 = 100;
				break;
			default:
				if (src->difficulty == depth)
					result2 = src->score;
				else
					result2 = RecursiveMiniMax(GameCopy, depth + 1, alpha, beta, !Player );
				break;
		}
		spCHGameDestroy(GameCopy);
	}
	if (result2>result[0]){
		result[0] = result2;
		result[1] = 1;												//right
	}else{
		result [1] = 0;												//left
	}
	return result;
}


/**
 * the function implementing a castle for the AI as one of the option in the minimax algorithm,
 * when the current player in this position in the mini max is black.
 */
int* castleBlackCase(SPCHGame *src, int depth, int alpha, int beta, int Player){
	if (src == NULL){
		printf("ERROR: castleBlackCase null pointer input\n");fflush (stdout);
		return NULL;
	}
	SPCHGame *GameCopy;
	static int result[2];
	int result2 = 1000;
	if ( (src->castle[3]== true)&&(src->castle[4]== true) 						// black left rook: king +rook never moved
		&&(src->gameBoard[7][0] == 'R') &&(src->gameBoard[7][4] == 'K')
			&& (src->gameBoard[7][1] == SP_CH_GAME_EMPTY_ENTRY)						// free way
		&& (src->gameBoard[7][2] == SP_CH_GAME_EMPTY_ENTRY)
		&& (src->gameBoard[7][3] == SP_CH_GAME_EMPTY_ENTRY)
		&& (!spCHGameDangerIndex(src, 7,2)) && (!spCHGameDangerIndex(src, 7,3))		// not passing trough danger
		&& (!spCHGameDangerIndex(src, 7,4)) ) {
		GameCopy = spCHGameCopy(src);
		GameCopy->gameBoard[7][0] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->gameBoard[7][2] = 'K';
		GameCopy->gameBoard[7][3] = 'R';
		GameCopy->gameBoard[7][4] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->castle[3] = false;
		GameCopy->castle[4] = false;
		spCHChangePlayer(GameCopy);
		switch (spCHCheckWinnerOrTie(GameCopy)){
			case 8:
				result[0] = -100;
				break;
			case 4:
				result[0] = 100;
				break;
			default:
				if (src->difficulty == depth)
					result[0] = src->score;
				else
					result[0] = RecursiveMiniMax(GameCopy, depth + 1, alpha, beta, !Player );
				break;
		}
		spCHGameDestroy(GameCopy);
	}
	if ( (src->castle[4]== true)&&(src->castle[5]== true) 						//black right rook: king +rook never moved
			&&(src->gameBoard[7][7] == 'R') &&(src->gameBoard[7][4] == 'K')
			&& (src->gameBoard[7][6] == SP_CH_GAME_EMPTY_ENTRY)						// free way
				&& (src->gameBoard[7][5] == SP_CH_GAME_EMPTY_ENTRY)
				&& (!spCHGameDangerIndex(src, 7,5)) && (!spCHGameDangerIndex(src, 7,6))		// not passing trough danger
				&& (!spCHGameDangerIndex(src, 7,4))  ){                                    	// not in check now
		GameCopy = spCHGameCopy(src);
		GameCopy->gameBoard[7][7] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->gameBoard[7][6] = 'K';
		GameCopy->gameBoard[7][5] = 'R';
		GameCopy->gameBoard[7][4] = SP_CH_GAME_EMPTY_ENTRY;
		GameCopy->castle[4] = false;
		GameCopy->castle[5] = false;
		spCHChangePlayer(GameCopy);
		switch (spCHCheckWinnerOrTie(GameCopy)){
			case 8:
				result2 = -100;
				break;
			case 4:
				result2 = 100;
				break;
			default:
				if (src->difficulty == depth)
					result2 = src->score;
				else
					result2 = RecursiveMiniMax(GameCopy, depth + 1, alpha, beta, !Player );
				break;
		}
		spCHGameDestroy(GameCopy);
	}
	if (result2<result[0]){
		result[0] = result2;
		result[1] = 1;												//right
	}else{
		result [1] = 0;												//left
	}
	return result;
}

/**
 * the function implementing a move for the AI as one of the option in the minimax algorithm,
 */
int RecursiveMiniMax(SPCHGame* src, int depth, int alpha, int beta, int Player){ 				//player == 0 is max player, white player. player == 1 is black player, min player
	if (src == NULL){
		printf("ERROR: RecursiveMiniMax null pointer input\n");fflush (stdout);
		return 0;
	}
	if (depth == src->difficulty)
		return src->score;
	switch (spCHCheckWinnerOrTie(src)){
		case 4:
			return 100;
			break;
		case 8:
			return -100;
			break;
		case 6:
			return src->score;
			break;
		default:
			break;
	}
	int *promotion;
	int check[4] = {0,0,0,0};
	int *castling;
	bool pruning = false;
	SPCHGame *GameCopy;// = spCHGameCreate(1,1,0,0);
	if (Player == 1){ 												// white player - max player
		for (int CurRow=0; CurRow<SP_CH_GAME_N_ROWS ; CurRow++){
			if (!pruning){
				for (int CurCol=0; CurCol<SP_CH_GAME_N_COLUMNS ; CurCol++){
					if (!pruning){
						for (int DesRow=0; DesRow<SP_CH_GAME_N_ROWS; DesRow++){
							if (!pruning){
								for (int DesCol=0; DesCol<SP_CH_GAME_N_COLUMNS; DesCol++){
									if (!pruning){
										check[0] = CurRow;
										check[1] = CurCol;
										check[2] = DesRow;
										check[3] = DesCol;
										if (spCHGameIsValidMove(src, check) == 2){
											GameCopy = spCHGameCopy(src);
											int MoveScore;
											switch (spCHGameSetMove(GameCopy,check)){
												case 13:
													MoveScore = 100;
													break;
												case 14:
													MoveScore = -100;
													break;
												case 9 :
													promotion = promotionWhiteCase(GameCopy,DesRow,DesCol,depth,alpha,beta,1);
													MoveScore = promotion[0];
													break;
												default:
													MoveScore = RecursiveMiniMax(GameCopy, depth + 1, alpha, beta, !Player );
													break;
											}
											spCHGameDestroy(GameCopy);
											if (alpha < MoveScore){
												alpha = MoveScore;
											}
											if (beta <= alpha){
												pruning = true;
												break;
											}
										}
									}else
										break;
								}
							}else
								break;
						}
					}else
						break;
				}
			}else
				break;
		}
		GameCopy = spCHGameCopy(src);
		int castle[2] = {7,0};
		int castle2[2] = {0,0};
		if ( (spCHPossibleCastling(GameCopy,castle)) || (spCHPossibleCastling(GameCopy,castle2)) ){
			castling = castleWhiteCase(GameCopy,depth +1, alpha, beta, 1);
			if (castling[0] > alpha)
				alpha = castling[0];
		}
		spCHGameDestroy(GameCopy);
		return alpha;
	}else{																	// black player - min player
		for (int CurRow=0; CurRow<SP_CH_GAME_N_ROWS ; CurRow++){
			if (!pruning){
				for (int CurCol=0; CurCol<SP_CH_GAME_N_COLUMNS ; CurCol++){
					if (!pruning){
						for (int DesRow=0; DesRow<SP_CH_GAME_N_ROWS; DesRow++){
							if (!pruning){
								for (int DesCol=0; DesCol<SP_CH_GAME_N_COLUMNS; DesCol++){
									if (!pruning){
										check[0] = CurRow;
										check[1] = CurCol;
										check[2] = DesRow;
										check[3] = DesCol;
										if (spCHGameIsValidMove(src, check) == 2){
//											if (src->gameBoard[3][7] != 'q')
//												check[0] = CurRow;
											GameCopy = spCHGameCopy(src);
											int MoveScore;
											switch (spCHGameSetMove(GameCopy,check)){
												case 13:
													MoveScore = 100;
													break;
												case 14:
													MoveScore = -100;
													break;
												case 9 :
													promotion = promotionBlackCase(GameCopy,DesRow,DesCol,depth,alpha,beta,0);
													MoveScore = promotion[0];
													break;
												default:
													MoveScore = RecursiveMiniMax(GameCopy, depth + 1, alpha, beta, !Player );
													break;
											}
											spCHGameDestroy(GameCopy);
											if (beta > MoveScore){
												beta = MoveScore;
											}
											if (beta <= alpha){
												pruning = true;
												break;
											}
										}
									}else
										break;
								}
							}else
								break;
						}
					}else
						break;
				}
			}else
				break;
		}
		GameCopy = spCHGameCopy(src);
		int castle[2] = {7,0};
		int castle2[2] = {0,0};
		if ( (spCHPossibleCastling(GameCopy,castle)) || (spCHPossibleCastling(GameCopy,castle2)) ){
			castling = castleBlackCase(GameCopy,depth+1, alpha, beta, 0);
			if (castling[0] < beta)
				beta = castling[0];
		}
		spCHGameDestroy(GameCopy);
		return beta;
	}
}

/**
 * the function returns the best move for the AI to play by the mini max algorithm
 */
int* SPCHComputerMove (SPCHGame* src){
	if (src == NULL){
		printf("ERROR: SPCHComputerMove null pointer input\n");fflush (stdout);
		return NULL;
	}
	int alpha = -10000;
	int beta = 10000;
	int check[4] = {0,0,0,0};
	int *promotion;
	int *castling;
	int MoveScore;
	bool promote = false;
	static int bestMove[5] = {0,0,0,0,0};
	bool pruning = false;
	SPCHGame *GameCopy;// = spCHGameCreate(1,1,0,0);
	if (src->currentPlayer == 1){ 												// the computer is white and now it is his turn
		for (int CurRow=0; CurRow<SP_CH_GAME_N_ROWS ; CurRow++){
			if (!pruning){
				for (int CurCol=0; CurCol<SP_CH_GAME_N_COLUMNS ; CurCol++){
					if (!pruning){
						for (int DesRow=0; DesRow<SP_CH_GAME_N_ROWS; DesRow++){
							if (!pruning){
								for (int DesCol=0; DesCol<SP_CH_GAME_N_COLUMNS; DesCol++){
									if (!pruning){
										check[0] = CurRow;
										check[1] = CurCol;
										check[2] = DesRow;
										check[3] = DesCol;
										if (spCHGameIsValidMove(src, check) == 2){
											GameCopy = spCHGameCopy(src);
											if (check[0] == 3 && check[1] == 7)
												check[0] =3;
											switch (spCHGameSetMove(GameCopy,check)){
												case 13:
													MoveScore = 100;
													break;
												case 14:
													MoveScore = -100;
													break;
												case 9 :
													promotion = promotionWhiteCase(GameCopy,DesRow,DesCol,1,alpha,beta,1);
													MoveScore = promotion[0];
													promote = true;
													break;
												default:
													MoveScore = RecursiveMiniMax(GameCopy, 1, alpha, beta,  0);
													break;
											}
											spCHGameDestroy(GameCopy);
											if (alpha < MoveScore){
												alpha = MoveScore;
												bestMove[0] = check[0];
												bestMove[1] = check[1];
												bestMove[2] = check[2];
												bestMove[3] = check[3];
												if (promote)
													bestMove[4] = promotion[1];
												promote = false;
											}
											if (beta <= alpha){
												pruning = true;
												break;
											}
										}
									}else
										break;
								}
							}else
								break;
						}
					}else
						break;
				}
			}else
				break;
		}
		GameCopy = spCHGameCopy(src);
		int castle[2] = {0,0};
		int castle2[2] = {0,7};
		if ( (spCHPossibleCastling(GameCopy,castle)) || (spCHPossibleCastling(GameCopy,castle2)) ){
			castling = castleWhiteCase(GameCopy,1, alpha, beta, 1);
			if (castling[0] > alpha){
				alpha = castling[0];
				if (castling[1] == 0)
					bestMove[4] = 6;
				else
					bestMove[4] = 7;
			}
		}
		spCHGameDestroy(GameCopy);
		return bestMove;
	}else{																	// black player
		for (int CurRow=0; CurRow<SP_CH_GAME_N_ROWS ; CurRow++){
			if (!pruning){
				for (int CurCol=0; CurCol<SP_CH_GAME_N_COLUMNS ; CurCol++){
					if (!pruning){
						for (int DesRow=0; DesRow<SP_CH_GAME_N_ROWS; DesRow++){
							if (!pruning){
								for (int DesCol=0; DesCol<SP_CH_GAME_N_COLUMNS; DesCol++){
									if (!pruning){
										check[0] = CurRow;
										check[1] = CurCol;
										check[2] = DesRow;
										check[3] = DesCol;
										if (spCHGameIsValidMove(src, check) == 2){
											GameCopy = spCHGameCopy(src);
//											if (check[0] == 6 && check[1] == 3)
//												check[0] = 6;
											switch (spCHGameSetMove(GameCopy,check)){
												case 13:
													MoveScore = 100;
													break;
												case 14:
													MoveScore = -100;
													break;
												case 9 :
													promotion = promotionBlackCase(GameCopy,DesRow,DesCol,1,alpha,beta,0);
													MoveScore = promotion[0];
													promote = true;
													break;
												default:
													MoveScore = RecursiveMiniMax(GameCopy, 1, alpha, beta,  1);
													break;
											}
											spCHGameDestroy(GameCopy);
											if (beta > MoveScore){
												beta = MoveScore;
												bestMove[0] = check[0];
												bestMove[1] = check[1];
												bestMove[2] = check[2];
												bestMove[3] = check[3];
												if (promote)
													bestMove[4] = promotion[1];
												promote = false;
											}
											if (beta <= alpha){
												pruning = true;
												break;
											}
										}
									}else
										break;
								}
							}else
								break;
						}
					}else
						break;
				}
			}else
				break;
		}
		GameCopy = spCHGameCopy(src);
		int castle[2] = {7,0};
		int castle2[2] = {7,7};
		if ( (spCHPossibleCastling(GameCopy,castle)) || (spCHPossibleCastling(GameCopy,castle2)) ){
			castling = castleBlackCase(GameCopy,1, alpha, beta, 0);
			if (castling[0] < beta){
				beta = castling[0];
				if (castling[1] == 0)
					bestMove[4] = 8;
				else
					bestMove[4] = 9;
			}
		}
		spCHGameDestroy(GameCopy);
		return bestMove;
	}
}
