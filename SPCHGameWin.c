/*
 * SPCHGameWin.c
 *
 *  Created on: Sep 27, 2017
 *      Author: Calypso
 */

#include "SPCHGameWin.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "SPCHMemoryCard.h"

bool history = false; ///to indicate if undo move is available
int down = 0;

//Inner functions
int isClickOnRestart(int x, int y) {
  	if ((x >= 30 && x <= 230) && (y >=0 && y <= 50)) {
  		return 1;
  	}
  	return 0;
}

int isClickOnSave(int x, int y) {
  	if ((x >= 30 && x <= 230) && (y >= 100 && y <= 150)) {
  		return 1;
  	}
  	return 0;
}

int isClickOnGameWinLoad(int x, int y) {
  	if ((x >= 30 && x <= 230) && (y >= 200 && y <= 250)) {
  		return 1;
  	}
  	return 0;
}

int isClickOnUndo(int x, int y) {
  	if ((x >= 30 && x <= 230) && (y >= 300 && y <= 350) && history) {
  		return 1;
  	}
  	return 0;
}

int isClickOnMainMenu(int x, int y) {
  	if ((x >= 30 && x <= 230) && (y >= 470 && y <= 520)) {
  		return 1;
  	}
  	return 0;
}

int isClickOnGameWinExit(int x, int y) {
  	if ((x >= 0 && x <= 230) && (y >= 570 && y <= 620)) {
  		return 1;
  	}
  	return 0;
}

int isClickOnBoard(int x, int y) {
  	if ((x >= 260 && x <= 900) && (y >= 0 && y <= 640)) {
  		return 1;
  	}
  	return 0;
}



SPGameWin* spGameWindowCreate() {
	SPGameWin* res = (SPGameWin*) calloc(sizeof(SPGameWin), sizeof(char));
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	if (res == NULL ) {
		printf("ERROR: Couldn't create SPGameWin struct\n");
		return NULL ;
	}
	// Create an application window with the following settings:
	res->window = SDL_CreateWindow("Chess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			900,                               // width, in pixels
			700,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->window == NULL ) {
		// In the case that the window could not be made...
		printf("ERROR: Could not create window: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->renderer = SDL_CreateRenderer(res->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->renderer == NULL ) {
		printf("ERROR: Could not create a renderer: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}

	//Create a board texture:
	loadingSurface = SDL_LoadBMP("./graphics/images/restart.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create restart.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->restartTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->restartTexture == NULL ) {
		printf("ERROR: could not create restart.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/save.bmp");
	if (loadingSurface == NULL ) {
		printf("Could not create save.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	//We use SetColorKey to make texture background transparent
	//This function will make the magenta background transparent
	//SDL_SetColorKey(loadingSurface, SDL_TRUE,
			//SDL_MapRGB(loadingSurface->format, 255, 0, 255));
	res->saveTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->saveTexture == NULL ) {
		printf("Could not create save.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/load.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create load.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->loadTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->loadTexture == NULL ) {
		printf("ERROR: could not create load.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/undo.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create undo.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->undoTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->undoTexture == NULL ) {
		printf("ERROR: could not create undo.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/unundo.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create unundo.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->unundoTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->unundoTexture == NULL ) {
		printf("ERROR: could not create unundo.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/mainMenu.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create mainMenu.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->mainTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->mainTexture == NULL ) {
		printf("ERROR: could not create mainMenu.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/exit.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create exit.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->exitTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->exitTexture == NULL ) {
		printf("ERROR: could not create exit.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/board.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create board.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->boardTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->boardTexture == NULL ) {
		printf("ERROR: could not create board.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/whiteKing.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create whiteKing.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->WKTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->WKTexture == NULL ) {
		printf("ERROR: could not create whiteKing.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/whiteQueen.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create whiteQueen.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->WQTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->WQTexture == NULL ) {
		printf("ERROR: could not create whiteQueen.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/whiteBishop.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create whieBishop.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->WBTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->WBTexture == NULL ) {
		printf("ERROR: could not create whiteBishop.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/whiteKnight.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create whiteKnight.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->WNTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->WNTexture == NULL ) {
		printf("ERROR: could not create whiteKnight.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/whiteRook.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create whiteRook.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->WRTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->WRTexture == NULL ) {
		printf("ERROR: could not create whiteRook.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/whitePawn.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create whitePawn.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->WMTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->WMTexture == NULL ) {
		printf("ERROR: could not create whitePawn.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/blackKing.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create blackKing.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->BKTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->BKTexture == NULL ) {
		printf("ERROR: could not create blackKing.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/blackQueen.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create blackQueen.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->BQTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->BQTexture == NULL ) {
		printf("ERROR: could not create blackQueen.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/blackBishop.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create blackBishop.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->BBTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->BBTexture == NULL ) {
		printf("ERROR: could not create blackBishop.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/blackKnight.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create blackKnight.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->BNTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->BNTexture == NULL ) {
		printf("ERROR: could not create blackKnight.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/blackRook.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create blackRook.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->BRTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->BRTexture == NULL ) {
		printf("ERROR: could not create blackRook.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("./graphics/images/blackPawn.bmp");
	if (loadingSurface == NULL ) {
		printf("ERROR: could not create blackPawn.bmp surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->BMTexture = SDL_CreateTextureFromSurface(res->renderer,
	loadingSurface);
	if (res->BMTexture == NULL ) {
		printf("ERROR: could not create blackPawn.bmp texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it
	return res;
}

void spGameWindowDestroy(SPGameWin* src) {
	if (!src) {
		return;
	}
	if (src->restartTexture != NULL ) {
		SDL_DestroyTexture(src->restartTexture);
	}
	if (src->saveTexture != NULL ) {
		SDL_DestroyTexture(src->saveTexture);
	}
	if (src->loadTexture != NULL ) {
		SDL_DestroyTexture(src->loadTexture);
	}
	if (src->undoTexture != NULL ) {
		SDL_DestroyTexture(src->undoTexture);
	}
	if (src->unundoTexture != NULL ) {
		SDL_DestroyTexture(src->unundoTexture);
	}
	if (src->mainTexture != NULL ) {
		SDL_DestroyTexture(src->mainTexture);
	}
	if (src->exitTexture != NULL ) {
		SDL_DestroyTexture(src->exitTexture);
	}
	if (src->boardTexture != NULL ) {
		SDL_DestroyTexture(src->boardTexture);
	}
	if (src->WKTexture != NULL ) {
		SDL_DestroyTexture(src->WKTexture);
	}
	if (src->WQTexture != NULL ) {
		SDL_DestroyTexture(src->WQTexture);
	}
	if (src->WBTexture != NULL ) {
		SDL_DestroyTexture(src->WBTexture);
	}
	if (src->WNTexture != NULL ) {
		SDL_DestroyTexture(src->WNTexture);
	}
	if (src->WRTexture != NULL ) {
		SDL_DestroyTexture(src->WRTexture);
	}
	if (src->WMTexture != NULL ) {
		SDL_DestroyTexture(src->WMTexture);
	}
	if (src->BKTexture != NULL ) {
		SDL_DestroyTexture(src->BKTexture);
	}
	if (src->BQTexture != NULL ) {
		SDL_DestroyTexture(src->BQTexture);
	}
	if (src->BBTexture != NULL ) {
		SDL_DestroyTexture(src->BBTexture);
	}
	if (src->BNTexture != NULL ) {
		SDL_DestroyTexture(src->BNTexture);
	}
	if (src->BRTexture != NULL ) {
		SDL_DestroyTexture(src->BRTexture);
	}
	if (src->BMTexture != NULL ) {
		SDL_DestroyTexture(src->BMTexture);
	}
	if (src->renderer != NULL ) {
		SDL_DestroyRenderer(src->renderer);
	}
	if (src->window != NULL ) {
		SDL_DestroyWindow(src->window);
	}
	free(src);
}

void spGameWindowDraw(SPGameWin* src, char gameBoard[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS]) {
	if(src == NULL){
		return;
	}
	SDL_Rect restartR = { .x = 30, .y = 0, .h = 50, .w = 200 };
	SDL_Rect saveR = { .x = 30, .y = 100, .h = 50, .w = 200 };
	SDL_Rect loadR = { .x = 30, .y = 200, .h = 50, .w = 200};
	SDL_Rect undoR = { .x = 30, .y = 300, .h = 50, .w = 200};
	SDL_Rect mainR = { .x = 30, .y = 470, .h = 50, .w = 200};
	SDL_Rect exitR = { .x = 30, .y = 570, .h = 50, .w = 200 };
	SDL_SetRenderDrawColor(src->renderer, 255, 255, 255, 255);
	SDL_RenderClear(src->renderer);
	SDL_RenderCopy(src->renderer, src->restartTexture, NULL, &restartR);
	SDL_RenderCopy(src->renderer, src->saveTexture, NULL, &saveR);
	SDL_RenderCopy(src->renderer, src->loadTexture, NULL, &loadR);
	if(history)
		SDL_RenderCopy(src->renderer, src->undoTexture, NULL, &undoR);
	else
		SDL_RenderCopy(src->renderer, src->unundoTexture, NULL, &undoR);
	SDL_RenderCopy(src->renderer, src->mainTexture, NULL, &mainR);
	SDL_RenderCopy(src->renderer, src->exitTexture, NULL, &exitR);
	SDL_Rect boardR = { .x = 260, .y = 0, .w = 640, .h = 640 };
	SDL_RenderCopy(src->renderer, src->boardTexture, NULL, &boardR);
	for (int i = 0; i < SP_CH_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_CH_GAME_N_COLUMNS; j++) {
			boardR.x = (j) *80 +260;
			boardR.y = (7-i)*80;
			boardR.h = 80;
			boardR.w = 80;
			switch(gameBoard[i][j]){
				case 'k':
					SDL_RenderCopy(src->renderer, src->WKTexture, NULL, &boardR);
					break;
				case 'q':
					SDL_RenderCopy(src->renderer, src->WQTexture, NULL, &boardR);
					break;
				case 'b':
					SDL_RenderCopy(src->renderer, src->WBTexture, NULL, &boardR);
					break;
				case 'n':
					SDL_RenderCopy(src->renderer, src->WNTexture, NULL, &boardR);
					break;
				case 'r':
					SDL_RenderCopy(src->renderer, src->WRTexture, NULL, &boardR);
					break;
				case 'm':
					SDL_RenderCopy(src->renderer, src->WMTexture, NULL, &boardR);
					break;
				case 'K':
					SDL_RenderCopy(src->renderer, src->BKTexture, NULL, &boardR);
					break;
				case 'Q':
					SDL_RenderCopy(src->renderer, src->BQTexture, NULL, &boardR);
					break;
				case 'B':
					SDL_RenderCopy(src->renderer, src->BBTexture, NULL, &boardR);
					break;
				case 'N':
					SDL_RenderCopy(src->renderer, src->BNTexture, NULL, &boardR);
					break;
				case 'R':
					SDL_RenderCopy(src->renderer, src->BRTexture, NULL, &boardR);
					break;
				case 'M':
					SDL_RenderCopy(src->renderer, src->BMTexture, NULL, &boardR);
					break;
				case '_':
					break;
			}
		}
	}
	SDL_RenderPresent(src->renderer);
}

/**
 * this function copies the pieces(chars) from one board
 * to another. in case of a null argument prints an error message
 */

void copyBoard(char copy[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS], char board[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS]){
	if(copy == NULL || board == NULL){
		printf("ERROR: SPCHGameWin->copyBoard has failed - null argument exception");
		return;
	}
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			copy[row][col] = board[row][col];
		}
	}
}

/**
 * a function which draws a chess board
 */

void drawInMotion(SPGameWin* src ,char board[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS],int x,int y){
	spGameWindowDraw(src,board);
	SDL_Rect pieceR = { .x = x - 27, .y = y - 12, .h = 50, .w = 50 };
	//SDL_Rect pieceR = { .x = (src->srcCol)*96+200, .y = (src->srcRow)*96, .h = 50, .w = 50 };
	if (src->piece == 'k') {
		SDL_RenderCopy(src->renderer, src->WKTexture, NULL, &pieceR);}
	if (src->piece == 'q') {
		SDL_RenderCopy(src->renderer, src->WQTexture, NULL, &pieceR);}
	if (src->piece == 'K') {
		SDL_RenderCopy(src->renderer, src->BKTexture, NULL, &pieceR);}
	if (src->piece == 'Q') {
		SDL_RenderCopy(src->renderer, src->BQTexture, NULL, &pieceR);}
	if (src->piece == 'n') {
		SDL_RenderCopy(src->renderer, src->WNTexture, NULL, &pieceR);}
	if (src->piece == 'N') {
		SDL_RenderCopy(src->renderer, src->BNTexture, NULL, &pieceR);}
	if (src->piece == 'R') {
		SDL_RenderCopy(src->renderer, src->BRTexture, NULL, &pieceR);}
	if (src->piece == 'r') {
		SDL_RenderCopy(src->renderer, src->WRTexture, NULL, &pieceR);}
	if (src->piece == 'B') {
		SDL_RenderCopy(src->renderer, src->BBTexture, NULL, &pieceR);}
	if (src->piece == 'b') {
		SDL_RenderCopy(src->renderer, src->WBTexture, NULL, &pieceR);}
	if (src->piece == 'M') {
		SDL_RenderCopy(src->renderer, src->BMTexture, NULL, &pieceR);}
	if (src->piece == 'm') {
		SDL_RenderCopy(src->renderer, src->WMTexture, NULL, &pieceR);}
	SDL_RenderPresent(src->renderer);
}

void setValues(SPGameWin* src, char p, int  x, int y){
	src->piece=p;
	src->srcCol=x;
	src->srcRow=y;
}

void setHistory(SPCHGame* game){
	if(game->prevMoves->actualSize >= 2 && game->game_mode == 1)
		history = true;
	else
		history =false;
}

SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event, SPCHGame* game) {
	if (event == NULL || src == NULL ) {
		return SP_GAME_EVENT_INVALID_ARGUMENT;
	}
	if (game->game_mode == 1 && game->currentPlayer != game->user_color){
		//printf("xcvb");
		computerMove(game);
		//int comturn =computer_turn(game);
		setHistory(game);
		spGameWindowDraw(src, game->gameBoard);
		//if (comturn==3)
		//return SP_GAME_EVENT_QUIT;;
		SP_CH_GAME_MESSAGE msg = spCHCheckWinnerOrTie(game);
		if (msg == 4 || msg == 8){  //after user's turn only he can win
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CheckMate!",
						"The computer wins the game\n",NULL );
				spCHGameDestroy(game);
				return SP_GAME_EVENT_WHITE_WON;
		}
		if (msg==6){
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Tie",
					"%it's a tie",NULL );
			spCHGameDestroy(game);
			return SP_GAME_EVENT_TIE;
		}
		if(spCHCheckForCheck(game)){
			if(game->currentPlayer == game->user_color){  //should always be true
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Check:",
						"your King is threatened!",NULL );
				return SP_GAME_EVENT_BLACK_KING_THREATENED;  //the color doesn't matter here
			}
		}
	}
	int x = event->button.x;
	int y = event->button.y;
	char piece;
	int dx,dy;
//	int tx,ty;
	char temp[8][8];
	//SDL_Event* event2;
	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if(isClickOnBoard(event->button.x, event->button.y)){
//				tx=(x-260)/80;
//				ty=7-(y/80);
				piece = game->gameBoard[7-(y/80)][(x-240)/80];
				if((game->currentPlayer == 1)&&(piece < 's')&&(piece > 'a')&&(game->user_color == 1)){
					down=1;
					dx=(x-260)/80;
					dy=y/80;
					setValues(src,piece,dx,dy);
					copyBoard(temp,game->gameBoard);
					spGameWindowDraw(src, temp);
				}
				if((game->currentPlayer == 0)&&(piece < 'S')&&(piece > 'A')&&((game->user_color==0)||(game->game_mode==2))){
					down=1;
					dx=(x-260)/80;
					dy=y/80;
					setValues(src,piece,dx,dy);
					copyBoard(temp,game->gameBoard);
					spGameWindowDraw( src ,temp);
				}
			}
			return SP_GAME_EVENT_NONE;

		case SDL_MOUSEMOTION:
			if(down==1){
				while(1){
					copyBoard(temp,game->gameBoard);
					drawInMotion(src,temp,event->motion.x,event->motion.y);
					SDL_WaitEvent(event);
					if (event->type==SDL_MOUSEBUTTONUP){
						down=2;
						break;
					}
				}
				//return SP_GAME_EVENT_NONE;
				return spGameWindowHandleEvent( src,event, game);
			}
			return SP_GAME_EVENT_NONE;
			//break;

		case SDL_MOUSEBUTTONUP:
			if (isClickOnRestart(event->button.x, event->button.y)) {
				InitializeBoard(game);
				game->prevMoves->actualSize = 0;
				setHistory(game);
				spGameWindowDraw(src, game->gameBoard);
				return SP_GAME_EVENT_RESTART;
			}
			if (isClickOnSave(event->button.x, event->button.y)) {
				printf("save");
				int savedSuccessfully = spSaveToMemory(game);
				if(savedSuccessfully == -1) //unsuccessful
					return SP_GAME_EVENT_QUIT;
				return SP_GAME_EVENT_NONE;
			}
			if (isClickOnGameWinLoad(event->button.x, event->button.y)) {
				printf("load");
				return SP_GAME_EVENT_LOAD;
				break;
			}
			if (isClickOnUndo(event->button.x, event->button.y)) {
				undoCase(game);
				setHistory(game);
				spGameWindowDraw( src ,game->gameBoard);
				return SP_GAME_EVENT_NONE;
			}
			if (isClickOnMainMenu(event->button.x, event->button.y)) {
				return SP_GAME_EVENT_MAIN_MENU;
			}
			if (isClickOnGameWinExit(event->button.x, event->button.y)) {
				return SP_GAME_EVENT_QUIT;
			}
			if (isClickOnBoard(event->button.x, event->button.y)){
				if(down==2){
					down=0;
					x=(event->button.x-260)/80;
					y=event->button.y/80;
					//SP_FIAR_GAME_MESSAGE movecon;
					int array[4] = {7-src->srcRow,src->srcCol,7-y,x};
					int* coord = &array[0];
					spCHGameSetMove(game,coord);
					setHistory(game);
					spGameWindowDraw( src ,game->gameBoard);
					SP_CH_GAME_MESSAGE msg = spCHCheckWinnerOrTie(game);
					if (game->game_mode == 2){
						switch(msg){
							case 4:  //white won
								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CheckMate!",
										"White player wins the game\n",NULL );
								spCHGameDestroy(game);
								return SP_GAME_EVENT_WHITE_WON;
								break;
							case 6:
								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Tie",
										"%it's a tie",NULL );
								spCHGameDestroy(game);
								return SP_GAME_EVENT_TIE;
								break;
							case 8:  //black won
								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CheckMate!",
										"Black player wins the game\n",NULL );
								spCHGameDestroy(game);
								return SP_GAME_EVENT_BLACK_WON;
								break;
							default:
								break;
						}
						if(spCHCheckForCheck(game)){
							if(game->game_mode == 2){
								switch(game->currentPlayer){
									case 1:
										SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Check:",
												"white King is threatened!",NULL );
										return SP_GAME_EVENT_WHITE_KING_THREATENED;
									case 0:
										SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Check:",
												"black King is threatened!",NULL );
										return SP_GAME_EVENT_BLACK_KING_THREATENED;
								}
							}
						}
					}
					else {  //game mode == 1
						if (msg == 4 || msg == 8){  //after user's turn only he can win
								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CheckMate!",
										"you win the game\n",NULL );
								spCHGameDestroy(game);
								return SP_GAME_EVENT_WHITE_WON;
						}
						if (msg==6){
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Tie",
									"%it's a tie",NULL );
							spCHGameDestroy(game);
							return SP_GAME_EVENT_TIE;
						}
//						if(game->currentPlayer != game->user_color){
//							//printf("xcvb");
//							computerMove(game);
//							//int comturn =computer_turn(game);
//							setHistory(game);
//							spGameWindowDraw(src, game->gameBoard);
//							//if (comturn==3)
//							//return SP_GAME_EVENT_QUIT;;
//							SP_CH_GAME_MESSAGE msg = spCHCheckWinnerOrTie(game);
//							if (msg == 4 || msg == 8){  //after user's turn only he can win
//									SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CheckMate!",
//											"The computer wins the game\n",NULL );
//									spCHGameDestroy(game);
//									return SP_GAME_EVENT_WHITE_WON;
//							}
//							if (msg==6){
//								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Tie",
//										"%it's a tie",NULL );
//								spCHGameDestroy(game);
//								return SP_GAME_EVENT_TIE;
//							}
//							if(spCHCheckForCheck(game)){
//								if(game->currentPlayer == game->user_color){  //should always be true
//									SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Check:",
//											"your King is threatened!",NULL );
//									return SP_GAME_EVENT_BLACK_KING_THREATENED;  //the color doesn't matter here
//								}
//							}
//						}
					}
				}
			}
			break;

		case SDL_WINDOWEVENT:
			if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
				return SP_GAME_EVENT_QUIT;
			}
			break;
		default:
			return SP_GAME_EVENT_NONE;
			}
		return SP_GAME_EVENT_NONE;
}

void spGameWindowHide(SPGameWin* src) {
	SDL_HideWindow(src->window);
}

void spGameWindowShow(SPGameWin* src) {
	SDL_ShowWindow(src->window);
}


