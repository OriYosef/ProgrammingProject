/*
 * SPCHMainAux.c
 *
 *  Created on: Sep 30, 2017
 *      Author: Calypso
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "SPCHMiniMax.h"
#include "SPCHParser.h"
#include "SPCHGame.h"
#include "limits.h"

#define SP_MAX_LINE_LENGTH 1024



// auxiliary functions and constants used by the main function here.



/*
 * in case of receiving a game_mode command in setting state
 * this function returns the mode value if a valid argument is received
 * and prints the chosen game mode to the player
 * if invalid returns the default value and notify the player via printed message
 */
int gameMode(SPCommand* cmd){
	if (cmd == NULL){
		printf("ERROR: gameMode null pointer input\n");fflush (stdout);
		return 0;
	}
	if (cmd->validArg){
		switch(cmd->arg){																// massage by the validaty of the command
			case 1:
				printf("Game mode is set to 1 player\n"); fflush (stdout);
				break;
			case 2:
				printf("Game mode is set to 2 players\n"); fflush (stdout);
				break;
		}
		return cmd->arg;
	}
	printf("Wrong game mode\n"); fflush (stdout);
	return 1;
}

/*
 * this function returns the difficulty value if a valid argument is received
 * if invalid returns the default value and notify the player via printed message
 */
void difficultyCase(SPCHGame* game, SPCommand* cmd){
	if (cmd == NULL || game == NULL){
		printf("ERROR: difficultyCase null pointer input\n");fflush (stdout);
		return;
	}
	if (cmd->validArg){																				// massage by the validaty of the command
		if (cmd->arg == 5){
			printf("Expert level not supported, please choose a value between 1 to 4:\n");fflush (stdout);
			return;
		}
		game->difficulty = cmd->arg;
		return;
	}
	printf("Wrong difficulty level. The value should be between 1 to 5\n"); fflush (stdout);
	return;
}

/*
 * this function returns the color value if a valid argument is received
 * if invalid returns the default value and notify the player via printed message
 */
int colorCase(SPCommand* cmd){
	if (cmd == NULL){
		printf("ERROR: colorCase null pointer input\n");fflush (stdout);
		return 0;
	}
	if(cmd->validArg)																		// massage by the validaty of the command
		return cmd->arg;
	printf("Invalid command\n");  fflush (stdout);
	return 1;
}

/*
 *this function restart a given game and prints the relevant massages to the consul
 */
void resetCase(SPCHGame* game){
	if (game == NULL){
		printf("ERROR: resetCase null pointer input\n");fflush (stdout);
		return;
	}
	printf("Restarting...\n");fflush (stdout);
	spCHRestartGame(game);
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");fflush (stdout);
}

/*
 *this function receives a path to a file which represents a
 *saved game situation, and alternates the received game according to
 *the file's description, returns 0 on success; -1 on failure
 */
int loadGame(char* address, SPCHGame* game){
	if (game == NULL){
		printf("ERROR: loadGame null pointer input\n");fflush (stdout);
		return -1;
	}
	int row;
	FILE* fp = fopen(address, "r");
	if(!fp){
		printf("Error: File doesn't exist or cannot be opened\n"); fflush (stdout);
		return -1;
	}
	char* buf = malloc(sizeof(char)*365);
	if (!buf){
		printf("ERROR: loadGame memory allocation has failed\n"); fflush (stdout);
		fclose(fp);
		return -1;
	}
	while(fgets(buf,364,fp) != NULL){   								//NULL will indicate the end of the file and end the loop
		const char del[8] = " \t\r\n";
		char copy[SP_MAX_LINE_LENGTH];
		strcpy(copy,buf);
		char* token = strtok(copy,del);
		switch(token[1]){
			case 'c':														// <current_player>
				game->currentPlayer = token[14] - '0';  				// convert char to int
				break;
			case 'g':														// <game>
				if(token[5] == '_')  					///asserting that the tag is "<game_mode>" and not "<game>"/"<general>"
					game->game_mode = token[11] - '0';
				break;
			case 'd':														// <difficulty>
				if(game->game_mode == 1){
					if ((token[12] - '0') == 5){
						printf("Expert level not supported\n"); fflush (stdout);
						fclose(fp);
						free(buf);
						return -1;
					}
					game->difficulty = token[12] - '0';
				}
				break;
			case 'u':														// <user_color>
				if(game->game_mode == 1){
					game->user_color = token[12] - '0';
				}
				break;
			case 'r':														// <row_..>
				row = token[5]-49;
				for(int col = 0; col<8; col++){
					if (token[col + 7] == '_')
						game->gameBoard[row][col] = SP_CH_GAME_EMPTY_ENTRY;
					else
						game->gameBoard[row][col] = token[col+7];
				}
				break;
		}
	}
	game->score = SPCHGameScore(game);
	game->prevMoves->actualSize = 0;
	fclose(fp);
	free(buf);
	return 0;
}


/**
 * the function saves the current game in xml format in the file which the adress
 * direct to.
 * validates the SPCHGame pointer received
 */
int saveGame(char* address, SPCHGame* sg){
	if( sg == NULL){
		printf("ERROR: saveGame null pointer input\n");fflush (stdout);
		return 0; 																		//0 will indicate failure
	}
	FILE* fp = fopen(address, "w");
	if(!fp){
		printf("File cannot be created or modified\n"); fflush (stdout);
		return 0;
	}
	fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n\t"
			"<current_turn>%d</current_turn>\n\t<game_mode>%d</game_mode>\n",sg->currentPlayer,sg->game_mode);
	if (sg->game_mode == 1)
		fprintf(fp, "\t<difficulty>%d</difficulty>\n\t<user_color>%d</user_color>\n",sg->difficulty,sg->user_color);
	fprintf(fp, "\t<board>\n");
	for(int row = SP_CH_GAME_N_ROWS; row > 0; row--){
		fprintf(fp,"\t\t<row_%d>",row);
		for(int col = 0; col < SP_CH_GAME_N_COLUMNS; col++){
			if(sg->gameBoard[row-1][col] == SP_CH_GAME_EMPTY_ENTRY)
				fprintf(fp,"_");
			else
				fprintf(fp,"%c",sg->gameBoard[row-1][col]);
		}
		fprintf(fp,"</row_%d>\n",row);
	}
	fprintf(fp, "\t</board>\n</game>\n");
	fclose(fp);
	return 1;																					//1 will indicate success
}


/*
 *the function recieves a game pointer, and prints the game board + enter move request
 *at non-computer turns. in case it receives a NULL pointer - does nothing
 */
void printEM(SPCHGame* game){   															 //EM = Enter Move
	if (game == NULL){
		printf("ERROR: printEM null pointer input\n");fflush (stdout);
		return;
	}
	if(game->game_mode == 1){
		if(game->currentPlayer == game->user_color){
			if(game->user_color == 1){   														 ///white
				printf("white player - enter your move:\n");fflush (stdout);
			}else{
				printf("black player - enter your move:\n");fflush (stdout);
			}
		}
	}else{   																					 ///game_mode == 2
		if(game->currentPlayer == 1){    																///white
			printf("white player - enter your move:\n");fflush (stdout);
		}else{
			printf("black player - enter your move:\n");fflush (stdout);
		}
	}
}

/*
 *the function recieves a game pointer, and changing the game to the exactly game it was before the last move.
 *the function prints the relevant massages to the consul.
 */
void undoCase(SPCHGame* game){
	if (game == NULL){
		printf("ERROR: undoCase null pointer input\n");fflush (stdout);
		return;
	}
	if (game->game_mode == 2){
		printf("Undo command not available in 2 players mode\n");fflush (stdout);
		return;
	}
	char tmp[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS];
	for (int row=0;row<SP_CH_GAME_N_ROWS; row++){
		for (int col=0; col<SP_CH_GAME_N_COLUMNS; col++){
			tmp[row][col] = game->gameBoard[row][col];												// saving the current board
		}
	}
	int msg1,msg2;
	msg1 = spCHGameUndoPrevMove(game);																// update the current move to the last one
	int move[8] = {0,0,0,0,0,0,0,0};
	int count = 0;
	char from = 'A';
	char to = 'A';
	if(msg1 == 3){
		printf("Empty history, move cannot be undone\n");fflush (stdout);
		return;
	}
	if (msg1 == 2){
		for (int row=0;row<SP_CH_GAME_N_ROWS; row++){
			for (int col=0; col<SP_CH_GAME_N_COLUMNS; col++){
				if (game->currentPlayer==1){														// looking for the differences between the boards
					if (tmp[row][col] != game->gameBoard[row][col]  && spCHWhite(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
					if (tmp[row][col] != game->gameBoard[row][col]  && !spCHWhite(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
				}else{
					if (tmp[row][col] != game->gameBoard[row][col]  && spCHBlack(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
					if (tmp[row][col] != game->gameBoard[row][col]  && !spCHBlack(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
				}
			}
		}
		if (count == 4){													// just two different indexes  -> regular move
			from += move[1];
			to += move[3];
			if (game->currentPlayer == 0){
				if (spCHBlack(game,move[0],move[1])){
					printf("Undo move for player black : <%d,%c> -> <%d,%c>\n", move[2]+1, to,move[0]+1,from);fflush (stdout);
				}else{
					printf("Undo move for player black : <%d,%c> -> <%d,%c>\n", move[0]+1,from,move[2]+1, to);fflush (stdout);
				}
			}else{
				if (spCHWhite(game,move[0],move[1])){
					printf("Undo move for player white : <%d,%c> -> <%d,%c>\n", move[2]+1, to,move[0]+1,from);fflush (stdout);
				}else{
					printf("Undo move for player white : <%d,%c> -> <%d,%c>\n", move[0]+1,from,move[2]+1, to);fflush (stdout);
				}
			}
		}else{															// 4 defferences -> castle move
			if (game->currentPlayer ==0){
				printf("Undo castle for player black\n");fflush (stdout);
			}else{
				printf("Undo castle for player white\n");fflush (stdout);
			}
		}
	}
	char tmp2[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS];								// coping the board again
	for (int row=0;row<SP_CH_GAME_N_ROWS; row++){
		for (int col=0; col<SP_CH_GAME_N_COLUMNS; col++){
			tmp2[row][col] = game->gameBoard[row][col];
		}
	}
	msg2 = spCHGameUndoPrevMove(game);												// updating to the last board
	if (msg2 == 3){
		printf("Empty history, move cannot be undone\n");fflush (stdout);
		return;
	}
	from = 'A';
	to = 'A';
	count = 0;
	if (msg2 == 2){
		for (int row=0;row<SP_CH_GAME_N_ROWS; row++){
			for (int col=0; col<SP_CH_GAME_N_COLUMNS; col++){
				if (game->currentPlayer==1){												// looking for the differences
					if (tmp2[row][col] != game->gameBoard[row][col]  && spCHWhite(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
					if (tmp2[row][col] != game->gameBoard[row][col]  && !spCHWhite(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
				}else{
					if (tmp2[row][col] != game->gameBoard[row][col]  && spCHBlack(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
					if (tmp2[row][col] != game->gameBoard[row][col]  && !spCHBlack(game,row,col)){
						move[count] = row;
						move[count+1] = col;
						count = count +2;
					}
				}
			}
		}
		if (count ==4){																// 2 defferences -> regular move
			from += move[1];
			to += move[3];
			if (game->currentPlayer == 0){
				if (spCHBlack(game,move[0],move[1])){
					printf("Undo move for player black : <%d,%c> -> <%d,%c>\n", move[2]+1, to,move[0]+1,from);fflush (stdout);
				}else{
					printf("Undo move for player black : <%d,%c> -> <%d,%c>\n", move[0]+1,from,move[2]+1, to);fflush (stdout);
				}
			}else{
				if (spCHWhite(game,move[0],move[1])){
					printf("Undo move for player white : <%d,%c> -> <%d,%c>\n", move[2]+1, to,move[0]+1,from);fflush (stdout);
				}else{
					printf("Undo move for player white : <%d,%c> -> <%d,%c>\n", move[0]+1,from,move[2]+1, to);fflush (stdout);
				}
			}
		}else{																	// castle move
			if (game->currentPlayer ==0){
				printf("Undo castle for player black\n");fflush (stdout);
			}else{
				printf("Undo castle for player white\n");fflush (stdout);
			}
		}
	}
	game->score = SPCHGameScore(game);
	spCHGamePrintBoard(game);
}

/*
 * this function receives a game pointer and checks for check state
 * on the board. if there is such such a state it informs the relevant player
 * if not does nothing
 */
void checkCase(SPCHGame* game){
	if (game == NULL){
		printf("ERROR: checkCase null pointer input\n");fflush (stdout);
		return;
	}
	if(game->game_mode == 2){
		if(spCHCheckForCheck(game)){
			if(game->currentPlayer == 1){  														//white player
				printf("Check: white King is threatened!\n");fflush (stdout);
			}else{
				printf("Check: black King is threatened!\n");fflush (stdout);
			}
		}
	}else{  																					 //game mode == 1
		if (game->currentPlayer==game->user_color && spCHCheckForCheck(game)){
			printf("Check!\n");fflush (stdout);
		}
	}
}

/**
 * receives a game and checks for a terminal state after player move - winner/tie
 * returns an integer that Indicates the result in correlation to
 * the game messages which are defined in the game module
 */
int terminalCase(SPCHGame* game){
	if (game == NULL){
		printf("ERROR: terminalCase null pointer input\n");fflush (stdout);
		return 0;
	}
	switch(spCHCheckWinnerOrTie(game)){
		case 1:  																	///invalid game argument
			return 1;
			break;
		case 7:  																	 ///no winner nor tie
			return 7;
			break;
		case 8: 																	  ///black won
			printf("Checkmate! black player wins the game\n");fflush (stdout);
			return 8;
			break;
		case 4:  																	///white won
			printf("Checkmate! white player wins the game\n");fflush (stdout);
			return 4;
			break;
		case 6:  																	 //tie
			printf("The game is tied\n");fflush (stdout);
			return 6;
			break;
		default:
			break;
	}
	return 0;
}

/**
 * receives a game and checks for a terminal state after AI move - winner/tie
 * returns an integer that Indicates the result in correlation to
 * the game messages which are defined in the game module
 */
int terminalComCase(SPCHGame* game){
	if (game == NULL){
		printf("ERROR: terminalComCase null pointer input\n");fflush (stdout);
		return 0;
	}
	switch(spCHCheckWinnerOrTie(game)){
		case 1:  														///invalid game argument
			return 1;
			break;
		case 7:  															 ///no winner nor tie
			return 7;
			break;
		case 8:  															 ///black won
			printf("Checkmate! black player wins the game\n");fflush (stdout);
			return 8;
			break;
		case 4: 															 ///white won
			printf("Checkmate! white player wins the game\n");fflush (stdout);
			return 4;
			break;
		case 6: 															  //tie
			printf("The game ends in a tie\n");fflush (stdout);
			return 6;
			break;
		default:
			break;
	}
	return 0;
}

/**
 * the function handle a command in the setting state. prints the relevant massages and execute the command
 */
void settingState(SPCommand* command, SPCHGame* game){
	if (command == NULL || game == NULL){
		printf("ERROR: settingState null pointer input\n");fflush (stdout);
		return;
	}
	switch (command->cmd){														// by the command parser
		case 0:          															   //game_mode case
			game->game_mode = gameMode(command);
			break;
		case 1:  																		 // difficulty case
			if(game->game_mode == 1){
				difficultyCase(game, command);
			}else{
				printf("Invalid command\n");fflush (stdout);
			}
			break;
		case 2:																			//user_color case
			if(game->game_mode == 1){
				game->user_color = colorCase(command);
			}else{
				printf("Invalid command\n");fflush (stdout);
			}
			break;
		case 3:																			//load case;
			loadGame(command->address, game);
			break;
		case 4:																			//default case
			game->game_mode = 1;
			game->difficulty = 2;
			game->currentPlayer = 1;
			game->user_color = 1;
			break;
		case 5:																			//print_setting command case
			if(game->game_mode == 1){
				char* str;
				if (game->user_color==1)
					str = "WHITE";
				else
					str = "BLACK";
				printf("SETTINGS:\nGAME_MODE: 1\nDIFFICULTY_LVL: %d\nUSER_CLR: %s\n",game->difficulty,str);fflush (stdout);
			}else{
				printf("SETTINGS:\nGAME_MODE: 2\n");fflush (stdout);
			}
			break;
		default:  																			 //invalid commands
			printf("Invalid command\n");fflush (stdout);
			break;
	}
	return;
}

/**
 * this function prints and invoking the promotion move that was played
 */

void MoveAndPromition(SPCommand* command, SPCHGame* game){
	char str[SP_MAX_LINE_LENGTH];
	const char s[8] = " \t\r\n";
	char copy[SP_MAX_LINE_LENGTH];
	printf("Pawn promotion- please replace the pawn by queen, rook,"
			" knight, bishop or pawn:\n");fflush (stdout);
	fgets(str, sizeof(str), stdin);
	while ((str[0] != 'q')&&(str[0] != 'k')&&
			(str[0] != 'b')&&(str[0] != 'p')&&(str[0] != 'r')){
		printf("Invalid Type\n");fflush (stdout);
		printf("Pawn promotion- please replace the pawn by queen, rook,"
				" knight, bishop or pawn:\n");fflush (stdout);
		fgets(str, sizeof(str), stdin);
	}
	strcpy(copy, str);
	char* str2 = strtok(copy, s);
	char* str3 = strtok(NULL, s);
	while ( (strcmp(str2, "queen")!=0 && strcmp(str2, "knight")!=0
			&&strcmp(str2, "rook")!=0 && strcmp(str2, "bishop")!=0
			&&strcmp(str2, "pawn")!=0) || (str3 != NULL) || alreadyFull(game,str2) ){
		printf("Invalid Type\n");fflush (stdout);
		printf("Pawn promotion- please replace the pawn by queen, rook,"
				" knight, bishop or pawn:\n");fflush (stdout);
		fgets(str, sizeof(str), stdin);
		while ((str[0] != 'q')&&(str[0] != 'k')&&
				(str[0] != 'b')&&(str[0] != 'p')&&(str[0] != 'r')){
			printf("Invalid Type\n");fflush (stdout);
			printf("Pawn promotion- please replace the pawn by queen, rook,"
					" knight, bishop or pawn:\n");fflush (stdout);
			fgets(str, sizeof(str), stdin);
		}
		strcpy(copy, str);
		str2 = strtok(copy, s);
		str3 = strtok(NULL, s);
	}
	if (game->currentPlayer == 0){												// now black. white makes promotion
		if (str2[0] == 'q')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'q';
		if (str2[0] == 'k')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'n';
		if (str2[0] == 'r')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'r';
		if (str2[0] == 'b')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'b';
	}else{
		if (str2[0] == 'q')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'Q';
		if (str2[0] == 'k')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'N';
		if (str2[0] == 'r')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'R';
		if (str2[0] == 'b')
			game->gameBoard[command->coordinates[2]][command->coordinates[3]] = 'B';
	}
	game->score = SPCHGameScore(game);
	return;
}


/**
 * this function prints and invoking the get_moves command
 */
void GetMovesCase(SPCommand* command, SPCHGame* game){
	if (!command->validCoord){													// invalid command
		printf("Invalid position on the board\n");fflush (stdout);
	}else{
		if (!spCHGamePlayerPiece(game,command->coordinates[0],command->coordinates[1])){		// not user piece
			char* str;
			if (game->currentPlayer==0)
				str = "black";
			else
				str = "white";
			printf("The specified position does not contain %s player piece\n", str);fflush (stdout);
		}else{
			int check[4] = {0,0,0,0};
			check[0] = command->coordinates[0];
			check[1] = command->coordinates[1];
			for (int row=0; row<SP_CH_GAME_N_ROWS; row++){
				for (int col=0; col<SP_CH_GAME_N_COLUMNS; col++){
					char to = 'A';
					check[2]=row;
					check[3]=col;
					if (spCHGameIsValidMove(game,check)==2){				//possible move
						to += col;
						printf("<%d,%c>", row+1,to);fflush (stdout);
						char tmp = game->gameBoard[row][col];
						game->gameBoard[row][col] = game->gameBoard[check[0]][check[1]];
						game->gameBoard[check[0]][check[1]] = SP_CH_GAME_EMPTY_ENTRY;
						if (spCHGameDangerIndex(game,row,col)){
							printf("*");fflush (stdout);
						}
						game->gameBoard[check[0]][check[1]] = game->gameBoard[row][col];
						game->gameBoard[row][col] = tmp;
						if ((game->currentPlayer==1) && (spCHBlack(game,row,col))){
							printf("^");fflush (stdout);
						}
						if ((game->currentPlayer==0) && (spCHWhite(game,row,col))){
							printf("^");fflush (stdout);
						}
						printf("\n");fflush (stdout);
					}
				}
			}
			int checkCas[2] = {0.0};
			checkCas[0] = 0;
			checkCas[1] = 0;
			if (spCHPossibleCastling(game,checkCas))
				printf("castle <1,A>\n");fflush (stdout);
			checkCas[0] = 0;
			checkCas[1] = 7;
			if (spCHPossibleCastling(game,checkCas))
				printf("castle <1,H>\n");fflush (stdout);
			checkCas[0] = 7;
			checkCas[1] = 0;
			if (spCHPossibleCastling(game,checkCas))
				printf("castle <8,A>\n");fflush (stdout);
			checkCas[0] = 7;
			checkCas[1] = 7;
			if (spCHPossibleCastling(game,checkCas))
				printf("castle <8,H>\n");fflush (stdout);
		}
	}
	return;
}

/**
 * this function prints and invoking the castle command
 */
int CastleCase(SPCommand* command, SPCHGame* game){
	if(!command->validCoord){
		printf("Invalid position on the board\n");fflush (stdout);
	}else{
		if (!spCHGamePlayerPiece(game,command->coordinates[0], command->coordinates[1])){
			printf("The specified position does not contain your piece\n");fflush (stdout);
		}else{
			int posCastling[2];
			posCastling[0] = command->coordinates[0];
			posCastling[1] = command->coordinates[1];
			SP_CH_GAME_MESSAGE message = spCHcastling(game,posCastling);
			switch(message){
				case 12:
					printf("Wrong position for a rook\n");fflush (stdout);
					break;
				case 0:
					printf("Illegal castling move\n");fflush (stdout);
					break;
				case 2:
					return 1;					// success case
					break;
				default:
					break;
			}
		}
	}
	return 0;
}

/**
 * the function handle a command in the game state. prints the relevant massages and execute the command
 */
int gameState(SPCommand* command, SPCHGame* game){
	if (command == NULL || game == NULL){
		printf("ERROR: gameState null pointer input\n");fflush (stdout);
		return 0;
	}
	switch(command->cmd){																	// by the command parser
		case 8: 																				//move_to case
			if(!command->validCoord){
				printf("Invalid position on the board\n");fflush (stdout);
			}else{
				SP_CH_GAME_MESSAGE message = spCHGameSetMove(game, command->coordinates);			//set the move and handle the massage returned
				switch(message){
					case 11:  													 //invalid argument coord does not contain user's piece
						printf("The specified position does not contain your piece\n");fflush (stdout);
						break;
					case 2:														 //success
						return 1;
						break;
					case 9:  													//success and promotion
						MoveAndPromition(command, game);
						return 1;
						break;
					case 13:													// winner
					case 14:
						return 1;
						break;
					default:
						printf("Illegal move\n");fflush (stdout);
						break;
				}
			}
			break;
		case 9:  																//get_moves case, get function from Ori
			if (game->game_mode ==2 || game->difficulty >2){
				printf("Invalid command\n");fflush (stdout);
				break;
			}
			GetMovesCase (command, game);
			break;
		case 14:																// castle command case
			if (CastleCase(command, game))
				return 1;
			break;
		case 10: 																 //save case
			saveGame(command->address, game);
			break;
		case 11:
			undoCase(game);
			break;
		default:   																//invalid commands
			printf("Invalid command\n");fflush (stdout);
			break;
		}
	return 0;
}


/**
 * the function gets a game. execute a AI move and prints the relevant massages to the consul
 */
void computerMove(SPCHGame* game){
	if (game == NULL){
		printf("ERROR: computerMove null pointer input\n");fflush (stdout);
		return;
	}
	char* piece = "no move";														// for preventing fail cases
	char from = 'A';
	char to = 'A';
	int *suggestMove = SPCHComputerMove(game);										// getting the best move by the mini max algorithm
	int castle[2] = {0,0};
	while (suggestMove == NULL)														// protective programming
		suggestMove = SPCHComputerMove(game);
	if ((game->currentPlayer==1)&&(suggestMove[4] == 6)){							// [4] > 5 indicates castle
		castle[0] = 0;
		castle[1] = 0;
		spCHcastling(game,castle);
		printf("Computer: castle King at <1,E> and Rook at <1,A>\n");fflush (stdout);
		return;
	}
	if ((game->currentPlayer==1)&&(suggestMove[4] == 7)){
		castle[0] = 0;
		castle[1] = 7;
		spCHcastling(game,castle);
		printf("Computer: castle King at <1,E> and Rook at <1,H>\n");fflush (stdout);
		return;
	}
	if ((game->currentPlayer==0)&&(suggestMove[4] == 8)){
		castle[0] = 7;
		castle[1] = 0;
		spCHcastling(game,castle);
		printf("Computer: castle King at <8,E> and Rook at <8,A>\n");fflush (stdout);
		return;
	}
	if ((game->currentPlayer==0)&&(suggestMove[4] == 9)){
		castle[0] = 7;
		castle[1] = 7;
		spCHcastling(game,castle);
		printf("Computer: castle King at <8,E> and Rook at <8,H>\n");fflush (stdout);
		return;
	}
	if ((game->currentPlayer==1)&&														// regular white pawn move to play
			(game->gameBoard[suggestMove[0]][suggestMove[1]] == 'm')&&(suggestMove[2] == 7)){
		from += suggestMove[1];
		to += suggestMove[3];
		spCHGameSetMove(game,suggestMove);
		switch (suggestMove[4]){												// [4] == 5 indicate a promotion
			case 0:
				piece = "rook";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'r';
				break;
			case 1:
				piece = "knight";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'n';
				break;
			case 2:
				piece = "bishop";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'b';
				break;
			case 3:
				piece = "queen";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'q';
				break;
			case 4:
				piece = "pawn";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'm';
				break;
			default:
				break;
		}
		printf("Computer: move pawn <%d,%c> to <%d,%c> and promote to %s\n",
				suggestMove[0]+1,from,suggestMove[2]+1,to, piece);fflush (stdout);
		game->score = SPCHGameScore(game);
		return;
	}
	if ((game->currentPlayer==0)&&														// regular black pawn move to play
			(game->gameBoard[suggestMove[0]][suggestMove[1]] == 'M')&&(suggestMove[2] == 0)){
		from += suggestMove[1];
		to += suggestMove[3];
		spCHGameSetMove(game,suggestMove);
		switch (suggestMove[4]){														// [4] == 5 indicate a promotion
			case 0:
				piece = "rook";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'R';
				break;
			case 1:
				piece = "knight";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'N';
				break;
			case 2:
				piece = "bishop";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'B';
				break;
			case 3:
				piece = "queen";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'Q';
				break;
			case 4:
				piece = "pawn";
				game->gameBoard[suggestMove[2]][suggestMove[3]] = 'M';
				break;
			default:
				break;
		}
		printf("Computer: move pawn <%d,%c> to <%d,%c> and promote to %s\n",
				suggestMove[0]+1,from,suggestMove[2]+1,to, piece);fflush (stdout);
		game->score = SPCHGameScore(game);
		return;
	}
	switch (game->gameBoard[suggestMove[0]][suggestMove[1]]){									// regular move
		case 'q':
		case 'Q':
			piece = "queen";
			break;
		case 'm':
		case 'M':
			piece = "pawn";
			break;
		case 'n':
		case 'N':
			piece = "knight";
			break;
		case 'b':
		case 'B':
			piece = "bishop";
			break;
		case 'r':
		case 'R':
			piece = "rook";
			break;
		case 'k':
		case 'K':
			piece = "king";
			break;

	}
	from += suggestMove[1];
	to += suggestMove[3];
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n", piece, suggestMove[0]+1,from,suggestMove[2]+1,to);fflush (stdout);
	spCHGameSetMove(game,suggestMove);
	return;
}

/**
 * the function checks for check and printsthe board and the relevant massages
 * for another round.
 */
void spCHAnotherRound(SPCHGame* game){
	checkCase(game);
	spCHGamePrintBoard(game);
	printEM(game);
}

/**
 * the function returns true if a user turn to play
 */
bool spCHPlayerTurn(SPCHGame* game){
	if ( game->game_mode == 2 || (game->game_mode==1 && game->currentPlayer == game->user_color) )
		return true;
	else
		return false;
}

