/*
 * SPCHColorWin.c
 *
 *  Created on: Sep 26, 2017
 *      Author: Calypso
 */

#include "SPCHColorWin.h"
#include <SDL_video.h>
#include <stdio.h>

int selectedColor = -1; //to indicate the chosen color -1 means no color has been chosen yet

//Inner functions
int isClickOnWhite(int x, int y) {
	if ((x >= 150 && x <= 250) && (y >= 100 && y <= 200)) {
		return 1;
	}
	return 0;
}

int isClickOnBlack(int x, int y) {
	if ((x >= 550 && x <= 650) && (y >= 100 && y <= 200)) {
		return 1;
	}
	return 0;
}

int isClickOnColorStart(int x, int y) {
	if ((x >= 100 && x <= 300) && (y >= 300 && y <= 400)) {
		return 1;
	}
	return 0;
}

int isClickOnColorBack(int x, int y) {
	if ((x >= 500 && x <= 700) && (y >= 300 && y <= 400)) {
		return 1;
	}
	return 0;
}

SPColorWin* spColorWindowCreate() {
	SPColorWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;
	res = (SPColorWin*) calloc(sizeof(SPColorWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->colorWindow = SDL_CreateWindow("Chess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			500,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->colorWindow == NULL ) {
		spColorWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->colorRenderer = SDL_CreateRenderer(res->colorWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->colorRenderer == NULL ) {
		// In the case that the window could not be made...
		spColorWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./graphics/images/white.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create white.bmp surface\n");
		return NULL ;
	}
	res->whiteTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->whiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create white.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pwhite.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create Pwhite.bmp surface\n");
		return NULL ;
	}
	res->PwhiteTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->PwhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create Pwhite.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/black.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create black.bmp surface\n");
		return NULL ;
	}
	res->blackTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->blackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create black.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pblack.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create Pblack.bmp surface\n");
		return NULL ;
	}
	res->PblackTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->PblackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create Pblack.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/start.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create start.bmp surface\n");
		return NULL ;
	}
	res->startTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->startTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create start.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/unstart.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create unstart.bmp surface\n");
		return NULL ;
	}
	res->unstartTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->unstartTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create unstart.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/back.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create back.bmp surface\n");
		return NULL ;
	}
	res->backTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->backTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create back.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/selectYourColor.bmp");
	if (loadingSurface == NULL ) {
		spColorWindowDestroy(res);
		printf("couldn't create selectYourColor.bmp surface\n");
		return NULL ;
	}
	res->selectTexture = SDL_CreateTextureFromSurface(res->colorRenderer,
	loadingSurface);
	if (res->selectTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spColorWindowDestroy(res);
		printf("couldn't create selectYourColor.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);
	return res;
}

void spColorWindowDestroy(SPColorWin* src) {
	if (!src) {
		return;
	}
	if (src->whiteTexture != NULL ) {
		SDL_DestroyTexture(src->whiteTexture);
	}
	if (src->blackTexture != NULL ) {
		SDL_DestroyTexture(src->blackTexture);
	}
	if (src->PwhiteTexture != NULL ) {
		SDL_DestroyTexture(src->PwhiteTexture);
	}
	if (src->PblackTexture != NULL ) {
		SDL_DestroyTexture(src->PblackTexture);
	}
	if (src->startTexture != NULL ) {
		SDL_DestroyTexture(src->startTexture);
	}
	if (src->unstartTexture != NULL ) {
		SDL_DestroyTexture(src->unstartTexture);
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->selectTexture != NULL ) {
		SDL_DestroyTexture(src->selectTexture);
	}
	if (src->colorRenderer != NULL ) {
		SDL_DestroyRenderer(src->colorRenderer);
	}
	if (src->colorWindow != NULL ) {
		SDL_DestroyWindow(src->colorWindow);
	}
	free(src);
}

void spColorWindowDraw(SPColorWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect selectR = { .x = 275, .y = 40, .h = 50, .w = 250 };
	SDL_Rect whiteR = { .x = 150, .y = 100, .h = 100, .w = 100 };
	SDL_Rect blackR = { .x = 550, .y = 100, .h = 100, .w = 100 };
	SDL_Rect startR = { .x = 100, .y = 300, .h = 100, .w = 200 };
	SDL_Rect backR = { .x = 500, .y = 300, .h = 100, .w = 200 };
	SDL_SetRenderDrawColor(src->colorRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->colorRenderer);
	SDL_RenderCopy(src->colorRenderer, src->selectTexture, NULL, &selectR);
	SDL_RenderCopy(src->colorRenderer, src->backTexture, NULL, &backR);
	switch(selectedColor){
		case -1:
			SDL_RenderCopy(src->colorRenderer, src->whiteTexture, NULL, &whiteR);
			SDL_RenderCopy(src->colorRenderer, src->blackTexture, NULL, &blackR);
			SDL_RenderCopy(src->colorRenderer, src->unstartTexture, NULL, &startR);
			break;
		case 1:
			SDL_RenderCopy(src->colorRenderer, src->PwhiteTexture, NULL, &whiteR);
			SDL_RenderCopy(src->colorRenderer, src->blackTexture, NULL, &blackR);
			SDL_RenderCopy(src->colorRenderer, src->startTexture, NULL, &startR);
			break;
		case 0:
			SDL_RenderCopy(src->colorRenderer, src->whiteTexture, NULL, &whiteR);
			SDL_RenderCopy(src->colorRenderer, src->PblackTexture, NULL, &blackR);
			SDL_RenderCopy(src->colorRenderer, src->startTexture, NULL, &startR);
			break;
	}
	SDL_RenderPresent(src->colorRenderer);
}

SP_COLOR_EVENT spColorWindowHandleEvent(SPColorWin* src, SDL_Event* event) {
	if (!src)
		return SP_COLOR_INVALID_ARGUMENT;
	if (!event) {
		return SP_COLOR_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnWhite(event->button.x, event->button.y)) {
			selectedColor = 1;
			return SP_COLOR_NONE;
		}
		if (isClickOnBlack(event->button.x, event->button.y)) {
			selectedColor = 0;
			return SP_COLOR_NONE;
		}
		if (isClickOnColorStart(event->button.x, event->button.y) && selectedColor >= 0) {
			switch(selectedColor){
			case 1:
				return SP_COLOR_WHITE;
			case 0:
				return SP_COLOR_BLACK;
			}
		}
		if (isClickOnColorBack(event->button.x, event->button.y)) {
			return SP_COLOR_BACK;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_COLOR_EXIT;
		}
		break;
	default:
		return SP_COLOR_NONE;
	}
	return SP_COLOR_NONE;
}

void spColorWindowHide(SPColorWin* src) {
	SDL_HideWindow(src->colorWindow);
}

void spColorWindowShow(SPColorWin* src) {
	SDL_ShowWindow(src->colorWindow);
}


