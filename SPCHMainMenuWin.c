#include "SPCHMainMenuWin.h"
#include <SDL_video.h>
#include <stdio.h>

//Inner functions
int isClickOnNewGame(int x, int y) {
	if ((x >= 75 && x <= 250) && (y >= 100 && y <= 200)) {
		return 1;
	}
	return 0;
}

int isClickOnMainLoad(int x, int y) {
	if ((x >= 75 && x <= 250) && (y >= 250 && y <= 350)) {
		return 1;
	}
	return 0;
}

int isClickOnExit(int x, int y) {
	if ((x >= 75 && x <= 250) && (y >= 450 && y <= 550)) {
		return 1;
	}
	return 0;
}

SPMainWin* spMainWindowCreate() {
	SPMainWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;
	res = (SPMainWin*) calloc(sizeof(SPMainWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("Chess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			400,                               // width, in pixels
			650,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->mainWindow == NULL ) {
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL ) {
		// In the case that the window could not be made...
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./graphics/images/newGame.bmp");
	if (loadingSurface == NULL ) {
		spMainWindowDestroy(res);
		printf("couldn't create newGame.bmp surface\n");
		return NULL ;
	}
	res->newGameTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->newGameTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spMainWindowDestroy(res);
		printf("couldn't create newGame.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/exit.bmp");
	if (loadingSurface == NULL ) {
		spMainWindowDestroy(res);
		printf("couldn't create exit.bmp surface\n");
		return NULL ;
	}
	res->exitTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->exitTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spMainWindowDestroy(res);
		printf("couldn't create exit.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/load.bmp");
		if (loadingSurface == NULL ) {
			spMainWindowDestroy(res);
			printf("couldn't create load.bmp surface\n");
			return NULL ;
		}
		res->loadTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
				loadingSurface);
		if (res->loadTexture == NULL ) {
			SDL_FreeSurface(loadingSurface);
			spMainWindowDestroy(res);
			printf("couldn't create load.bmp texture\n");
			return NULL ;
		}
		SDL_FreeSurface(loadingSurface);
		return res;
}

void spMainWindowDestroy(SPMainWin* src) {
	if (!src) {
		return;
	}
	if (src->newGameTexture != NULL ) {
		SDL_DestroyTexture(src->newGameTexture);
	}
	if (src->exitTexture != NULL ) {
		SDL_DestroyTexture(src->exitTexture);
	}
	if (src->loadTexture != NULL ) {
		SDL_DestroyTexture(src->loadTexture);
	}
	if (src->mainRenderer != NULL ) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->mainWindow != NULL ) {
		SDL_DestroyWindow(src->mainWindow);
	}
	free(src);
}

void spMainWindowDraw(SPMainWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect newGameR = { .x = 75, .y = 100, .h = 100, .w = 250 };
	SDL_Rect loadR = { .x = 75, .y = 250, .h = 100, .w = 250};
	SDL_Rect exitR = { .x = 75, .y = 450, .h = 100, .w = 250 };
	SDL_SetRenderDrawColor(src->mainRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->mainRenderer);
	SDL_RenderCopy(src->mainRenderer, src->newGameTexture, NULL, &newGameR);
	SDL_RenderCopy(src->mainRenderer, src->loadTexture, NULL, &loadR);
	SDL_RenderCopy(src->mainRenderer, src->exitTexture, NULL, &exitR);
	SDL_RenderPresent(src->mainRenderer);
}

SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event) {
	if (!src) {
		return SP_MAIN_INVALID_ARGUMENT;
	}
	if (!event) {
		return SP_MAIN_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnNewGame(event->button.x, event->button.y)) {
			return SP_MAIN_NEW_GAME;
		}
		if (isClickOnMainLoad(event->button.x, event->button.y)) {
					return SP_MAIN_LOAD;
		}
		else if (isClickOnExit(event->button.x, event->button.y)) {
			return SP_MAIN_EXIT;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_MAIN_EXIT;
		}
		break;
	default:
		return SP_MAIN_NONE;
	}
	return SP_MAIN_NONE;
}

void spMainWindowHide(SPMainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void spMainWindowShow(SPMainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}
