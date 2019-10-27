/*
 * SPCHGUIManager.c
 *
 *  Created on: Sep 30, 2017
 *      Author: Calypso
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPCHGUIManager.h"
#include <string.h>
#include "SPCHMainAux.h"
#include "SPCHMemoryCard.h"
int from;  //will be used to indicate from which window did we get to the load win, 1 == main, 0 == game

SPGuiManager* spManagerCreate() {
	SPGuiManager* res = (SPGuiManager*) malloc(sizeof(SPGuiManager));
	if (res == NULL ) {
		return NULL ;
	}
	res->mainWin = spMainWindowCreate();
	if (res->mainWin == NULL ) {
		free(res);
		return NULL ;
	}
	res->gameWin = NULL;
	res->colorWin=NULL;
	res->diffWin=NULL;
	res->gameModeWin=NULL;
	res->loadWin=NULL;
	res->activeWin = SP_MAIN_WINDOW_ACTIVE;
	return res;
}
void spManagerDestroy(SPGuiManager* src) {
	if (!src) {
		return;
	}
	if (src->activeWin) {
		spLoadWindowDestroy(src->loadWin);
	}
	if (src->activeWin) {
		spDiffWindowDestroy(src->diffWin);
	}
	if (src->activeWin) {
		spGMWindowDestroy(src->gameModeWin);
	}
	if (src->activeWin) {
		spColorWindowDestroy(src->colorWin);
	}
	if (src->activeWin){
		spGameWindowDestroy(src->gameWin);
	}
	if(src->mainWin)   //checks that it's not NULL
		spMainWindowDestroy(src->mainWin);
	free(src);
}

void spManagerDraw(SPGuiManager* src) {
	if (!src) {
		return;
	}
	if (src->activeWin == SP_MAIN_WINDOW_ACTIVE) {
		spMainWindowDraw(src->mainWin);
	} else if (src->activeWin == SP_LOAD_WINDOW_ACTIVE){
		spLoadWindowDraw(src->loadWin);
	}else if (src->activeWin == SP_DIFF_WINDOW_ACTIVE){
		spDiffWindowDraw(src->diffWin);
	}else if (src->activeWin == SP_GM_WINDOW_ACTIVE){
		spGMWindowDraw(src->gameModeWin);
	}else if (src->activeWin == SP_COLOR_WINDOW_ACTIVE){
		spColorWindowDraw(src->colorWin);
	}//else if (src->activeWin == SP_GAME_WINDOW_ACTIVE){
		//spGameWindowDraw(src->gameWin);
	//}
}
SP_MANAGER_EVENT handleManagerDueToColorEvent(SPGuiManager* src,
		SP_COLOR_EVENT event,SPCHGame* game) {
	if (event == SP_COLOR_NONE || src == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_COLOR_BACK) {
		src->diffWin=spDiffWindowCreate();
		if (src->diffWin == NULL ) {
			printf("Couldn't create difficulty window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_DIFF_WINDOW_ACTIVE;
		spDiffWindowDraw(src->diffWin);
		spColorWindowDestroy(src->colorWin);
		src->colorWin = NULL;
		return SP_MANAGER_NONE;
		}
	if (event == SP_COLOR_WHITE) {
		game->user_color=1;
		spColorWindowDestroy(src->colorWin);
		src->colorWin=NULL;
		src->gameWin = spGameWindowCreate();
		src->activeWin=SP_GAME_WINDOW_ACTIVE;
		if (src->gameWin == NULL ) {
			printf("Couldn't create game window\n");
			return SP_MANAGER_QUIT;
		}
		spGameWindowDraw(src->gameWin,game->gameBoard);
		return SP_MANAGER_NONE;
	}
	if (event == SP_COLOR_BLACK) {
		game->user_color=0;
		spColorWindowDestroy(src->colorWin);
		src->colorWin=NULL;
		src->gameWin = spGameWindowCreate();
		src->activeWin=SP_GAME_WINDOW_ACTIVE;
		if (src->gameWin == NULL ) {
			printf("Couldn't create game window\n");
			return SP_MANAGER_QUIT;
		}
		spGameWindowDraw(src->gameWin,game->gameBoard);
		return SP_MANAGER_NONE;
	}
	spColorWindowDestroy(src->colorWin);
	src->colorWin = NULL;
	src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	spMainWindowShow(src->mainWin);
	return SP_MANAGER_NONE;

}

SP_MANAGER_EVENT handleManagerDueToGameModeEvent(SPGuiManager* src,
		SP_GM_EVENT event,SPCHGame* game) {
	if (event == SP_GM_NONE || src == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_GM_BACK) {
		spGMWindowDestroy(src->gameModeWin);
		src->gameModeWin = NULL;
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		spMainWindowShow(src->mainWin);
		return SP_MANAGER_NONE;
		}
	if (event == SP_GM_ONE_PLAYER) {
		game->game_mode=1;
		src->diffWin = spDiffWindowCreate();
		if (src->diffWin == NULL ) {
			printf("Couldn't create difficulty window\n");
			return SP_MANAGER_QUIT;
		}
		spGMWindowDestroy(src->gameModeWin);
		src->gameModeWin = NULL;
		src->activeWin = SP_DIFF_WINDOW_ACTIVE;
		spDiffWindowDraw(src->diffWin);
		return SP_MANAGER_NONE;
	}
	if (event == SP_GM_TWO_PLAYERS) {
		game->game_mode=2;
		spGMWindowDestroy(src->gameModeWin);
		src->gameModeWin = NULL;
		src->gameWin = spGameWindowCreate();
		if (src->gameWin == NULL ) {
			printf("Couldn't create game window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin=SP_GAME_WINDOW_ACTIVE;
		spGameWindowDraw(src->gameWin,game->gameBoard);
		return SP_MANAGER_NONE;
	}
	spGMWindowDestroy(src->gameModeWin);
	src->gameModeWin = NULL;
	src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	spMainWindowShow(src->mainWin);
	return SP_MANAGER_NONE;
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENT handleManagerDueToDiffEvent(SPGuiManager* src,
		SP_DIFF_EVENT event,SPCHGame* game) {
	if (event == SP_DIFF_NONE || src == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_DIFF_BACK) {
		src->gameModeWin=spGMWindowCreate();
		if (src->gameModeWin == NULL ) {
			printf("Couldn't create gameMode window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_GM_WINDOW_ACTIVE;
		spGMWindowDraw(src->gameModeWin);
		spDiffWindowDestroy(src->diffWin);
		src->diffWin = NULL;
		return SP_MANAGER_NONE;
		}
	if (event == SP_DIFF_NOOB) {
		game->difficulty=1;
		src->colorWin = spColorWindowCreate();
		if (src->colorWin == NULL ) {
			printf("Couldn't create color window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_COLOR_WINDOW_ACTIVE;
		spColorWindowDraw(src->colorWin);
		spDiffWindowDestroy(src->diffWin);
		src->diffWin = NULL;
		return SP_MANAGER_NONE;
	}
	if (event == SP_DIFF_EASY) {
		game->difficulty = 2;
		src->colorWin = spColorWindowCreate();
		if (src->colorWin == NULL ) {
			printf("Couldn't create color window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_COLOR_WINDOW_ACTIVE;
		spColorWindowDraw(src->colorWin);
		spDiffWindowDestroy(src->diffWin);
		src->diffWin = NULL;
		return SP_MANAGER_NONE;
	}
	if (event == SP_DIFF_MODERATE) {
		game->difficulty = 3;
		src->colorWin = spColorWindowCreate();
		if (src->colorWin == NULL ) {
			printf("Couldn't create color window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_COLOR_WINDOW_ACTIVE;
		spColorWindowDraw(src->colorWin);
		spDiffWindowDestroy(src->diffWin);
		src->diffWin = NULL;
		return SP_MANAGER_NONE;
	}
	if (event == SP_DIFF_HARD) {
		game->difficulty = 4;
		src->colorWin = spColorWindowCreate();
		if (src->colorWin == NULL ) {
			printf("Couldn't create color window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_COLOR_WINDOW_ACTIVE;
		spColorWindowDraw(src->colorWin);
		spDiffWindowDestroy(src->diffWin);
		src->diffWin = NULL;
		return SP_MANAGER_NONE;
	}
	spDiffWindowDestroy(src->diffWin);
	src->diffWin = NULL;
	src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	spMainWindowShow(src->mainWin);
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENT handleManagerDueToMainEvent(SPGuiManager* src,
		SP_MAIN_EVENT event,SPCHGame* game) {
	if (game == NULL ) {
		return SP_MANAGER_NONE;
	}

	if (event == SP_MAIN_NONE || src == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_MAIN_NEW_GAME) {
		spMainWindowHide(src->mainWin);
		src->gameModeWin = spGMWindowCreate();
		if (src->gameModeWin == NULL ) {
			printf("Couldn't create mode window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_GM_WINDOW_ACTIVE;
		spGMWindowDraw(src->gameModeWin);
	}
	if (event == SP_MAIN_LOAD) {
		from = 1;
		spMainWindowHide(src->mainWin);
		src->loadWin = spLoadWindowCreate();
		if (src->loadWin == NULL ) {
			printf("Couldn't create load window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_LOAD_WINDOW_ACTIVE;
		spLoadWindowDraw(src->loadWin);
	}
	else if (event == SP_MAIN_EXIT) {
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENT handleManagerDueToLoadEvent(SPGuiManager* src,
		SP_LOAD_EVENT event,SPCHGame* game) {
	if (event == SP_LOAD_NONE || src == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_LOAD_BACK) {
		if(from == 1){  //main
			spLoadWindowDestroy(src->loadWin);
			src->loadWin=NULL;
			spMainWindowShow(src->mainWin);
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
			return SP_MANAGER_NONE;
		}
		else{  //game
			spLoadWindowDestroy(src->loadWin);
			src->loadWin=NULL;
			spGameWindowShow(src->gameWin);
			spGameWindowDraw(src->gameWin,game->gameBoard);
			src->activeWin = SP_GAME_WINDOW_ACTIVE;
			return SP_MANAGER_NONE;
		}
	}
	int successfulLoad;
	if (event == SP_LOAD_GS1) {
		successfulLoad = spLoadFromMemory(1,game);
	}
	if (event == SP_LOAD_GS2) {
		successfulLoad = spLoadFromMemory(2,game);
	}
	if (event == SP_LOAD_GS3) {
		successfulLoad = spLoadFromMemory(3,game);
	}
	if (event == SP_LOAD_GS4) {
		successfulLoad = spLoadFromMemory(4,game);
	}
	if (event == SP_LOAD_GS5) {
		successfulLoad = spLoadFromMemory(5,game);
	}
	if(successfulLoad == -1)
		return SP_MANAGER_QUIT;
	else {
		spLoadWindowDestroy(src->loadWin);
		src->loadWin=NULL;
		src->gameWin = spGameWindowCreate();
		src->activeWin = SP_GAME_WINDOW_ACTIVE;
		if (src->gameWin == NULL ) {
			printf("ERROR: Couldn't create game window\n");
			return SP_MANAGER_QUIT;
		}
		spMainWindowHide(src->mainWin);
		src->activeWin=SP_GAME_WINDOW_ACTIVE;
		spGameWindowDraw(src->gameWin,game->gameBoard);
		return SP_MANAGER_NONE;
	}
	spLoadWindowDestroy(src->loadWin);
	src->loadWin = NULL;
	src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	spMainWindowShow(src->mainWin);
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENT handleManagerDueToGameEvent(SPGuiManager* src,
		SP_GAME_EVENT event,SPCHGame* game) {
	if ((event == SP_GAME_EVENT_NONE)||(event==SP_GAME_EVENT_WHITE_KING_THREATENED)
			|| (event = SP_GAME_EVENT_BLACK_KING_THREATENED) || (src == NULL) || (game == NULL) ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_EVENT_BLACK_WON  || event == SP_GAME_EVENT_WHITE_WON) {
		spGameWindowDestroy(src->gameWin);
		src->gameWin = NULL;
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		spMainWindowShow(src->mainWin);
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_EVENT_LOAD) {
		printf("load");
		from = 0;
		spGameWindowHide(src->gameWin);
		src->activeWin = SP_LOAD_WINDOW_ACTIVE;
		src->loadWin = spLoadWindowCreate();
		if (src->loadWin == NULL ) {
			printf("Couldn't create load window\n");
			return SP_MANAGER_QUIT;
		}
		spLoadWindowDraw(src->loadWin);
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_EVENT_RESTART) {
		/*InitializeBoard(game);
		game->prevMoves->actualSize = 0;
		setHistory(game);
		spGameWindowDraw(src->gameWin, game->gameBoard);*/  //I've put it in the gameWin handle event
		return SP_MANAGER_NONE;
	}
	else if (event == SP_GAME_EVENT_TIE) {
		spGameWindowDestroy(src->gameWin);
		src->gameWin = NULL;
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		spMainWindowShow(src->mainWin);
		return SP_MANAGER_NONE;
	}
	spGameWindowDestroy(src->gameWin);
	src->gameWin = NULL;
	src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	spMainWindowShow(src->mainWin);
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENT spManagerHandleEvent(SPGuiManager* src, SDL_Event* event, SPCHGame* game) {
	if (src == NULL || event == NULL || game == NULL) {
		return SP_MANAGER_NONE;
	}
	if (src->activeWin == SP_MAIN_WINDOW_ACTIVE) {
		SP_MAIN_EVENT mainEvent = spMainWindowHandleEvent(src->mainWin, event);
		return handleManagerDueToMainEvent(src, mainEvent,game);
	} else if (src->activeWin == SP_LOAD_WINDOW_ACTIVE){
		SP_LOAD_EVENT LoadEvent = spLoadWindowHandleEvent(src->loadWin, event);
		return handleManagerDueToLoadEvent(src, LoadEvent,game);
	} else if (src->activeWin == SP_GM_WINDOW_ACTIVE){
		SP_GM_EVENT ModeEvent=spGMWindowHandleEvent(src->gameModeWin, event);
		return handleManagerDueToGameModeEvent(src, ModeEvent,game);
	}
	else if (src->activeWin == SP_COLOR_WINDOW_ACTIVE) {
		SP_COLOR_EVENT colorEvent = spColorWindowHandleEvent(src->colorWin, event);
		return handleManagerDueToColorEvent(src, colorEvent,game);
	}else if (src->activeWin == SP_DIFF_WINDOW_ACTIVE) {
		SP_DIFF_EVENT diffEvent = spDiffWindowHandleEvent(src->diffWin, event);
		return handleManagerDueToDiffEvent(src, diffEvent ,game);
	}
	else if (src->activeWin == SP_GAME_WINDOW_ACTIVE) {
		SP_GAME_EVENT GameEvent = spGameWindowHandleEvent(src->gameWin, event,game);
		return handleManagerDueToGameEvent(src, GameEvent ,game);
	}
	return SP_MANAGER_NONE;
}

