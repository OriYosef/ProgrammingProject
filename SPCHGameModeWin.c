/*
 * SPCHGameModeWin.c
 *
 *  Created on: Sep 26, 2017
 *      Author: Calypso
 */
#include "SPCHGameModeWin.h"
#include <SDL_video.h>
#include <stdio.h>

int mode = 0; //to indicate to chosen number of players; 0 means no choice has been made yet

//Inner functions
int isClickOnOnePlayer(int x, int y) {
	if ((x >= 100 && x <= 300) && (y >= 100 && y <= 200)) {
		return 1;
	}
	return 0;
}

int isClickOnTwoPlayers(int x, int y) {
	if ((x >= 500 && x <= 700) && (y >= 100 && y <= 200)) {
		return 1;
	}
	return 0;
}

int isClickOnGMNext(int x, int y) {
	if ((x >= 100 && x <= 300) && (y >= 300 && y <= 400)) {
		return 1;
	}
	return 0;
}

int isClickOnGMBack(int x, int y) {
	if ((x >= 500 && x <= 700) && (y >= 300 && y <= 400)) {
		return 1;
	}
	return 0;
}

SPGMWin* spGMWindowCreate() {
	SPGMWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;
	res = (SPGMWin*) calloc(sizeof(SPGMWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->gameModeWindow = SDL_CreateWindow("Chess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			500,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->gameModeWindow == NULL ) {
		spGMWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->gameModeRenderer = SDL_CreateRenderer(res->gameModeWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->gameModeRenderer == NULL ) {
		// In the case that the window could not be made...
		spGMWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./graphics/images/gameMode.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create gameMode.bmp surface\n");
		return NULL ;
	}
	res->gameModeTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->gameModeTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create gameMode.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/onePlayer.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create onePlayer.bmp surface\n");
		return NULL ;
	}
	res->onePlayerTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->onePlayerTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create onePlayer.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/twoPlayers.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create twoPlayers.bmp surface\n");
		return NULL ;
	}
	res->twoPlayersTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->twoPlayersTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create twoPlayers.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/PonePlayer.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create PonePlayer.bmp surface\n");
		return NULL ;
	}
	res->POnePlayerTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->POnePlayerTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create PonePlayer.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/PtwoPlayers.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create PtwoPlayers.bmp surface\n");
		return NULL ;
	}
	res->PTwoPlayersTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->PTwoPlayersTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create PtwoPlayers.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/next.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create next.bmp surface\n");
		return NULL ;
	}
	res->nextTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->nextTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create next.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/unnext.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create unnext.bmp surface\n");
		return NULL ;
	}
	res->unnextTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->unnextTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create unnext.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/start.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create start.bmp surface\n");
		return NULL ;
	}
	res->startTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->startTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create start.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/back.bmp");
	if (loadingSurface == NULL ) {
		spGMWindowDestroy(res);
		printf("couldn't create back.bmp surface\n");
		return NULL ;
	}
	res->backTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer,
	loadingSurface);
	if (res->backTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spGMWindowDestroy(res);
		printf("couldn't create back.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);
	return res;
}

void spGMWindowDestroy(SPGMWin* src) {
	if (!src) {
		return;
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->gameModeTexture != NULL ) {
		SDL_DestroyTexture(src->gameModeTexture);
	}
	if (src->nextTexture != NULL ) {
		SDL_DestroyTexture(src->nextTexture);
	}
	if (src->unnextTexture != NULL ) {
		SDL_DestroyTexture(src->unnextTexture);
	}
	if (src->startTexture != NULL ) {
		SDL_DestroyTexture(src->startTexture);
	}
	if (src->onePlayerTexture != NULL ) {
		SDL_DestroyTexture(src->onePlayerTexture);
	}
	if (src->POnePlayerTexture != NULL ) {
		SDL_DestroyTexture(src->POnePlayerTexture);
	}
	if (src->twoPlayersTexture != NULL ) {
		SDL_DestroyTexture(src->twoPlayersTexture);
	}
	if (src->PTwoPlayersTexture != NULL ) {
		SDL_DestroyTexture(src->PTwoPlayersTexture);
	}
	if (src->gameModeRenderer != NULL ) {
		SDL_DestroyRenderer(src->gameModeRenderer);
	}
	if (src->gameModeWindow != NULL ) {
		SDL_DestroyWindow(src->gameModeWindow);
	}
	free(src);
}

void spGMWindowDraw(SPGMWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect gameModeR = { .x = 295, .y = 20, .h = 50, .w = 200 };
	SDL_Rect onePlayerR = { .x = 100, .y = 100, .h = 100, .w = 200 };
	SDL_Rect twoPlayersR = { .x = 500, .y = 100, .h = 100, .w = 200 };
	SDL_Rect nextR = { .x = 100, .y = 300, .h = 100, .w = 200 };
	SDL_Rect backR = { .x = 500, .y = 300, .h = 100, .w = 200 };
	SDL_SetRenderDrawColor(src->gameModeRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->gameModeRenderer);
	SDL_RenderCopy(src->gameModeRenderer, src->gameModeTexture, NULL, &gameModeR);
	switch(mode){
	case 0:
		SDL_RenderCopy(src->gameModeRenderer, src->onePlayerTexture, NULL, &onePlayerR);
		SDL_RenderCopy(src->gameModeRenderer, src->twoPlayersTexture, NULL, &twoPlayersR);
		SDL_RenderCopy(src->gameModeRenderer, src->unnextTexture, NULL, &nextR);
		break;
	case 1:
		SDL_RenderCopy(src->gameModeRenderer, src->POnePlayerTexture, NULL, &onePlayerR);
		SDL_RenderCopy(src->gameModeRenderer, src->twoPlayersTexture, NULL, &twoPlayersR);
		SDL_RenderCopy(src->gameModeRenderer, src->nextTexture, NULL, &nextR);
		break;
	case 2:
		SDL_RenderCopy(src->gameModeRenderer, src->onePlayerTexture, NULL, &onePlayerR);
		SDL_RenderCopy(src->gameModeRenderer, src->PTwoPlayersTexture, NULL, &twoPlayersR);
		SDL_RenderCopy(src->gameModeRenderer, src->startTexture, NULL, &nextR);
	}
	SDL_RenderCopy(src->gameModeRenderer, src->backTexture, NULL, &backR);
	SDL_RenderPresent(src->gameModeRenderer);
}

SP_GM_EVENT spGMWindowHandleEvent(SPGMWin* src, SDL_Event* event) {
	if (!src) {
		return SP_GM_INVALID_ARGUMENT;
	}
	if (!event) {
		return SP_GM_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnOnePlayer(event->button.x, event->button.y)) {
			mode = 1;
			return SP_GM_NONE;
		}
		if (isClickOnTwoPlayers(event->button.x, event->button.y)) {
			mode = 2;
			return SP_GM_NONE;
		}
		if (isClickOnGMNext(event->button.x, event->button.y) && mode > 0) {
			switch(mode){
			case 1:
				return SP_GM_ONE_PLAYER;
			case 2:
				return SP_GM_TWO_PLAYERS;
			}
		}
		if (isClickOnGMBack(event->button.x, event->button.y)) {
			return SP_GM_BACK;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_GM_EXIT;
		}
		break;
	default:
		return SP_GM_NONE;
	}
	return SP_GM_NONE;
}

void spGMWindowHide(SPGMWin* src) {
	SDL_HideWindow(src->gameModeWindow);
}

void spGMWindowShow(SPGMWin* src) {
	SDL_ShowWindow(src->gameModeWindow);
}


