/*
 * SPCHDifficultyWin.c
 *
 *  Created on: Sep 28, 2017
 *      Author: Calypso
 */


#include "SPCHDifficultyWin.h"
#include <SDL_video.h>
#include <stdio.h>

int selectedDiff = 0; //will indicate the selected diff, 0 means one has yet to be chosen

//Inner functions
int isClickOnNoob(int x, int y) {
	if ((x >= 260 && x <= 410) && (y >= 140 && y <= 190)) {
		return 1;
	}
	return 0;
}

int isClickOnEasy(int x, int y) {
	if ((x >= 260 && x <= 410) && (y >= 240 && y <= 290)) {
		return 1;
	}
	return 0;
}

int isClickOnModerate(int x, int y) {
	if ((x >= 260 && x <= 410) && (y >= 340 && y <= 390)) {
		return 1;
	}
	return 0;
}

int isClickOnHard(int x, int y) {
	if ((x >= 260 && x <= 410) && (y >= 440 && y <= 490)) {
		return 1;
	}
	return 0;
}

int isClickOnDiffBack(int x, int y) {
	if ((x >= 425 && x <= 575) && (y >= 600 && y <= 650)) {
		return 1;
	}
	return 0;
}

int isClickOnNext(int x, int y) {
	if ((x >= 100 && x <= 250) && (y >= 600 && y <= 650)) {
		return 1;
	}
	return 0;
}

SPDiffWin* spDiffWindowCreate() {
	SPDiffWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;
	res = (SPDiffWin*) calloc(sizeof(SPDiffWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->diffWindow = SDL_CreateWindow("Chess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			700,                               // width, in pixels
			700,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->diffWindow == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->diffRenderer = SDL_CreateRenderer(res->diffWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->diffRenderer == NULL ) {
		// In the case that the window could not be made...
		spDiffWindowDestroy(res);
		printf("ERROR: could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./graphics/images/HDifficulty.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create HDifficulty.bmp surface\n");
		return NULL ;
	}
	res->HDiffTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->HDiffTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create HDifficulty.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/noob.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create noob.bmp surface\n");
		return NULL ;
	}
	res->noobTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->noobTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create noob.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pnoob.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Pnoob.bmp surface\n");
		return NULL ;
	}
	res->PNoobTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->PNoobTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Pnoob.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/easy.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create easy.bmp surface\n");
		return NULL ;
	}
	res->easyTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->easyTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create easy.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Peasy.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Peasy.bmp surface\n");
		return NULL ;
	}
	res->PEasyTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->PEasyTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Peasy.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/moderate.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create moderate.bmp surface\n");
		return NULL ;
	}
	res->moderateTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->moderateTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create moderate.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pmoderate.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Pmoderate.bmp surface\n");
		return NULL ;
	}
	res->PModerateTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->PModerateTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Pmoderate.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/hard.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create hard.bmp surface\n");
		return NULL ;
	}
	res->hardTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->hardTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create hard.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Phard.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Phard.bmp surface\n");
		return NULL ;
	}
	res->PHardTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->PHardTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create Phard.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/next.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create next.bmp surface\n");
		return NULL ;
	}
	res->nextTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->nextTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create next.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/unnext.bmp");
	if (loadingSurface == NULL ) {
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create unnext.bmp surface\n");
		return NULL ;
	}
	res->unnextTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
	loadingSurface);
	if (res->unnextTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("ERROR: couldn't create unnext.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/back.bmp");
		if (loadingSurface == NULL ) {
			spDiffWindowDestroy(res);
			printf("ERROR: couldn't create back.bmp surface\n");
			return NULL ;
		}
		res->backTexture = SDL_CreateTextureFromSurface(res->diffRenderer,
		loadingSurface);
		if (res->backTexture == NULL ) {
			SDL_FreeSurface(loadingSurface);
			spDiffWindowDestroy(res);
			printf("ERROR: couldn't create back.bmp texture\n");
			return NULL ;
		}

		SDL_FreeSurface(loadingSurface);


	return res;
}

void spDiffWindowDestroy(SPDiffWin* src) {
	if (!src) {
		return;
	}
	if (src->HDiffTexture != NULL ) {
		SDL_DestroyTexture(src->HDiffTexture);
	}
	if (src->noobTexture != NULL ) {
		SDL_DestroyTexture(src->noobTexture);
	}
	if (src->PNoobTexture != NULL ) {
		SDL_DestroyTexture(src->PNoobTexture);
	}
	if (src->easyTexture != NULL ) {
		SDL_DestroyTexture(src->easyTexture);
	}
	if (src->PEasyTexture != NULL ) {
		SDL_DestroyTexture(src->PEasyTexture);
	}
	if (src->moderateTexture != NULL ) {
		SDL_DestroyTexture(src->moderateTexture);
	}
	if (src->PModerateTexture != NULL ) {
		SDL_DestroyTexture(src->PModerateTexture);
	}
	if (src->hardTexture != NULL ) {
		SDL_DestroyTexture(src->hardTexture);
	}
	if (src->PHardTexture != NULL ) {
		SDL_DestroyTexture(src->PHardTexture);
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->nextTexture != NULL ) {
		SDL_DestroyTexture(src->nextTexture);
	}
	if (src->unnextTexture != NULL ) {
		SDL_DestroyTexture(src->unnextTexture);
	}
	if (src->diffRenderer != NULL ) {
		SDL_DestroyRenderer(src->diffRenderer);
	}
	if (src->diffWindow != NULL ) {
		SDL_DestroyWindow(src->diffWindow);
	}
	free(src);
}

void spDiffWindowDraw(SPDiffWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect HDiffR = { .x = 260, .y = 40, .h = 50, .w = 150 };
	SDL_Rect noobR = { .x = 260, .y = 140, .h = 50, .w = 150 };
	SDL_Rect easyR = { .x = 260, .y = 240, .h = 50, .w = 150 };
	SDL_Rect moderateR = { .x = 260, .y = 340, .h = 50, .w = 150 };
	SDL_Rect hardR = { .x = 260, .y = 440, .h = 50, .w = 150 };
	SDL_Rect nextR = { .x = 100, .y = 600, .h = 50, .w = 150 };
	SDL_Rect backR = { .x = 425, .y = 600, .h = 50, .w = 150 };
	SDL_SetRenderDrawColor(src->diffRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->diffRenderer);
	SDL_RenderCopy(src->diffRenderer, src->HDiffTexture, NULL, &HDiffR);
	switch(selectedDiff){
		case 0:
			SDL_RenderCopy(src->diffRenderer, src->noobTexture, NULL, &noobR);
			SDL_RenderCopy(src->diffRenderer, src->easyTexture, NULL, &easyR);
			SDL_RenderCopy(src->diffRenderer, src->moderateTexture, NULL, &moderateR);
			SDL_RenderCopy(src->diffRenderer, src->hardTexture, NULL, &hardR);
			break;
		case 1:
			SDL_RenderCopy(src->diffRenderer, src->PNoobTexture, NULL, &noobR);
			SDL_RenderCopy(src->diffRenderer, src->easyTexture, NULL, &easyR);
			SDL_RenderCopy(src->diffRenderer, src->moderateTexture, NULL, &moderateR);
			SDL_RenderCopy(src->diffRenderer, src->hardTexture, NULL, &hardR);
			break;
		case 2:
			SDL_RenderCopy(src->diffRenderer, src->noobTexture, NULL, &noobR);
			SDL_RenderCopy(src->diffRenderer, src->PEasyTexture, NULL, &easyR);
			SDL_RenderCopy(src->diffRenderer, src->moderateTexture, NULL, &moderateR);
			SDL_RenderCopy(src->diffRenderer, src->hardTexture, NULL, &hardR);
			break;
		case 3:
			SDL_RenderCopy(src->diffRenderer, src->noobTexture, NULL, &noobR);
			SDL_RenderCopy(src->diffRenderer, src->easyTexture, NULL, &easyR);
			SDL_RenderCopy(src->diffRenderer, src->PModerateTexture, NULL, &moderateR);
			SDL_RenderCopy(src->diffRenderer, src->hardTexture, NULL, &hardR);
			break;

		case 4:
			SDL_RenderCopy(src->diffRenderer, src->noobTexture, NULL, &noobR);
			SDL_RenderCopy(src->diffRenderer, src->easyTexture, NULL, &easyR);
			SDL_RenderCopy(src->diffRenderer, src->moderateTexture, NULL, &moderateR);
			SDL_RenderCopy(src->diffRenderer, src->PHardTexture, NULL, &hardR);
			break;
	}
	if (selectedDiff > 0)
		SDL_RenderCopy(src->diffRenderer, src->nextTexture, NULL, &nextR);
	else
		SDL_RenderCopy(src->diffRenderer, src->unnextTexture, NULL, &nextR);
	SDL_RenderCopy(src->diffRenderer, src->backTexture, NULL, &backR);
	SDL_RenderPresent(src->diffRenderer);
}

SP_DIFF_EVENT spDiffWindowHandleEvent(SPDiffWin* src, SDL_Event* event) {
	if (!src) {
		return SP_DIFF_INVALID_ARGUMENT;
	}
	if (!event) {
		return SP_DIFF_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnNoob(event->button.x, event->button.y)) {
			selectedDiff = 1;
			return SP_DIFF_NONE;
		}
		if (isClickOnEasy(event->button.x, event->button.y)) {
			selectedDiff = 2;
			return SP_DIFF_NONE;
		}
		if (isClickOnModerate(event->button.x, event->button.y)) {
			selectedDiff = 3;
			return SP_DIFF_NONE;
		}
		if (isClickOnHard(event->button.x, event->button.y)) {
			selectedDiff = 4;
			return SP_DIFF_NONE;
		}
		if (isClickOnNext(event->button.x, event->button.y) && selectedDiff > 0) {
			switch(selectedDiff){
			case 1:
				return SP_DIFF_NOOB;
			case 2:
				return SP_DIFF_EASY;
			case 3:
				return SP_DIFF_MODERATE;
			case 4:
				return SP_DIFF_HARD;
			}
		}
		if (isClickOnDiffBack(event->button.x, event->button.y)) {
			return SP_DIFF_BACK;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_DIFF_EXIT;
		}
		break;
	default:
		return SP_DIFF_NONE;
	}
	return SP_DIFF_NONE;
}

void spDiffWindowHide(SPDiffWin* src) {
	SDL_HideWindow(src->diffWindow);
}

void spDiffWindowShow(SPDiffWin* src) {
	SDL_ShowWindow(src->diffWindow);
}


