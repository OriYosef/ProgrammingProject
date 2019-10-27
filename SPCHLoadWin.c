/*
 * SPCHLoadWin.c
 *
 *  Created on: Sep 25, 2017
 *      Author: Calypso
 */

#include "SPCHLoadWin.h"
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include "SPCHMemoryCard.h"


int selectedSlot = 0;  //this field is for indicating which slot was pushed

//Inner functions
int isClickOnGS1(int x, int y) {
	if ((x >= 180 && x <= 330) && (y >= 50 && y <= 100)) {
		return 1;
	}
	return 0;
}

int isClickOnGS2(int x, int y) {
	if ((x >= 180 && x <= 330) && (y >= 150 && y <= 200)) {
		return 1;
	}
	return 0;
}

int isClickOnGS3(int x, int y) {
	if ((x >= 180 && x <= 330) && (y >= 250 && y <= 300)) {
		return 1;
	}
	return 0;
}

int isClickOnGS4(int x, int y) {
	if ((x >= 180 && x <= 330) && (y >= 350 && y <= 400)) {
		return 1;
	}
	return 0;
}

int isClickOnGS5(int x, int y) {
	if ((x >= 180 && x <= 330) && (y >= 450 && y <= 500)) {
		return 1;
	}
	return 0;
}

int isClickOnBack(int x, int y) {
	if ((x >= 60 && x <= 210) && (y >= 550 && y <= 600)) {
		return 1;
	}
	return 0;
}

int isClickOnLoad(int x, int y) {
	if ((x >= 320 && x <= 470) && (y >= 550 && y <= 600)) {
		return 1;
	}
	return 0;
}

SPLoadWin* spLoadWindowCreate() {
	SPLoadWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;
	res = (SPLoadWin*) calloc(sizeof(SPLoadWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->loadWindow = SDL_CreateWindow("Chess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			525,                               // width, in pixels
			700,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->loadWindow == NULL ) {
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->loadRenderer = SDL_CreateRenderer(res->loadWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->loadRenderer == NULL ) {
		// In the case that the window could not be made...
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./graphics/images/gs1.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create gs1.bmp surface\n");
		return NULL ;
	}
	res->gs1Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->gs1Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create gs1.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pgs1.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs1.bmp surface\n");
		return NULL ;
	}
	res->Pgs1Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->Pgs1Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs1.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/gs2.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create gs2.bmp surface\n");
		return NULL ;
	}
	res->gs2Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->gs2Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create gs2.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pgs2.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs2.bmp surface\n");
		return NULL ;
	}
	res->Pgs2Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->Pgs2Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs2.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/gs3.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create gs3.bmp surface\n");
		return NULL ;
	}
	res->gs3Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->gs3Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create gs3.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pgs3.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs3.bmp surface\n");
		return NULL ;
	}
	res->Pgs3Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->Pgs3Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs3.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/gs4.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create gs4.bmp surface\n");
		return NULL ;
	}
	res->gs4Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->gs4Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create gs4.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pgs4.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs4.bmp surface\n");
		return NULL ;
	}
	res->Pgs4Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->Pgs4Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs4.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/gs5.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create gs5.bmp surface\n");
		return NULL ;
	}
	res->gs5Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->gs5Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create gs5.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/Pgs5.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs5.bmp surface\n");
		return NULL ;
	}
	res->Pgs5Texture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->Pgs5Texture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create Pgs5.bmp texture\n");
		return NULL ;
	}

	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/back.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create back.bmp surface\n");
		return NULL ;
	}
	res->backTexture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->backTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create back.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/load.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create load.bmp surface\n");
		return NULL ;
	}
	res->loadTexture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->loadTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create load.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./graphics/images/unload.bmp");
	if (loadingSurface == NULL ) {
		spLoadWindowDestroy(res);
		printf("couldn't create unload.bmp surface\n");
		return NULL ;
	}
	res->unloadTexture = SDL_CreateTextureFromSurface(res->loadRenderer,
	loadingSurface);
	if (res->unloadTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spLoadWindowDestroy(res);
		printf("couldn't create unload.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}

void spLoadWindowDestroy(SPLoadWin* src) {
	if (!src) {
		return;
	}
	if (src->gs1Texture != NULL ) {
		SDL_DestroyTexture(src->gs1Texture);
	}
	if (src->gs2Texture != NULL ) {
		SDL_DestroyTexture(src->gs2Texture);
	}
	if (src->gs3Texture != NULL ) {
		SDL_DestroyTexture(src->gs3Texture);
	}
	if (src->gs4Texture != NULL ) {
		SDL_DestroyTexture(src->gs4Texture);
	}
	if (src->gs5Texture != NULL ) {
		SDL_DestroyTexture(src->gs5Texture);
	}
	if (src->Pgs1Texture != NULL ) {
		SDL_DestroyTexture(src->Pgs1Texture);
	}
	if (src->Pgs2Texture != NULL ) {
		SDL_DestroyTexture(src->Pgs2Texture);
	}
	if (src->Pgs3Texture != NULL ) {
		SDL_DestroyTexture(src->Pgs3Texture);
	}
	if (src->Pgs4Texture != NULL ) {
		SDL_DestroyTexture(src->Pgs4Texture);
	}
	if (src->Pgs5Texture != NULL ) {
		SDL_DestroyTexture(src->Pgs5Texture);
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->loadTexture != NULL ) {
		SDL_DestroyTexture(src->loadTexture);
	}
	if (src->unloadTexture != NULL ) {
		SDL_DestroyTexture(src->unloadTexture);
	}
	if (src->loadRenderer != NULL ) {
		SDL_DestroyRenderer(src->loadRenderer);
	}
	if (src->loadWindow != NULL ) {
		SDL_DestroyWindow(src->loadWindow);
	}
	free(src);
}

void spLoadWindowDraw(SPLoadWin* src) {
	if(src==NULL){
		return;
	}
	int fsn = getFullSlotsNum(); //full slots number, num of saved games
	if(fsn==-1)
		return;
	SDL_Rect gs1R = { .x = 180, .y = 50, .h = 50, .w = 150 };
	SDL_Rect gs2R = { .x = 180, .y = 150, .h = 50, .w = 150 };
	SDL_Rect gs3R = { .x = 180, .y = 250, .h = 50, .w = 150 };
	SDL_Rect gs4R = { .x = 180, .y = 350, .h = 50, .w = 150 };
	SDL_Rect gs5R = { .x = 180, .y = 450, .h = 50, .w = 150 };
	SDL_Rect backR = { .x = 60, .y = 550, .h = 50, .w = 150 };
	SDL_Rect loadR = { .x = 320, .y = 550, .h = 50, .w = 150};
	SDL_SetRenderDrawColor(src->loadRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->loadRenderer);
	if(fsn >= 1)
		SDL_RenderCopy(src->loadRenderer, src->gs1Texture, NULL, &gs1R);
	if(fsn >= 2)
		SDL_RenderCopy(src->loadRenderer, src->gs2Texture, NULL, &gs2R);
	if(fsn >= 3)
		SDL_RenderCopy(src->loadRenderer, src->gs3Texture, NULL, &gs3R);
	if(fsn >= 4)
		SDL_RenderCopy(src->loadRenderer, src->gs4Texture, NULL, &gs4R);
	if(fsn == 5)
		SDL_RenderCopy(src->loadRenderer, src->gs5Texture, NULL, &gs5R);


	switch(selectedSlot){  //handle event function takes care that the selctedSlot number won't be higher than fsn
		/*case 0:
			SDL_RenderCopy(src->loadRenderer, src->gs1Texture, NULL, &gs1R);
			SDL_RenderCopy(src->loadRenderer, src->gs2Texture, NULL, &gs2R);
			SDL_RenderCopy(src->loadRenderer, src->gs3Texture, NULL, &gs3R);
			SDL_RenderCopy(src->loadRenderer, src->gs4Texture, NULL, &gs4R);
			SDL_RenderCopy(src->loadRenderer, src->gs5Texture, NULL, &gs5R);
			break;*/
		case 1:
			SDL_RenderCopy(src->loadRenderer, src->Pgs1Texture, NULL, &gs1R);
			/*SDL_RenderCopy(src->loadRenderer, src->gs2Texture, NULL, &gs2R);
			SDL_RenderCopy(src->loadRenderer, src->gs3Texture, NULL, &gs3R);
			SDL_RenderCopy(src->loadRenderer, src->gs4Texture, NULL, &gs4R);
			SDL_RenderCopy(src->loadRenderer, src->gs5Texture, NULL, &gs5R);*/
			break;
		case 2:
			//SDL_RenderCopy(src->loadRenderer, src->gs1Texture, NULL, &gs1R);
			SDL_RenderCopy(src->loadRenderer, src->Pgs2Texture, NULL, &gs2R);
			/*SDL_RenderCopy(src->loadRenderer, src->gs3Texture, NULL, &gs3R);
			SDL_RenderCopy(src->loadRenderer, src->gs4Texture, NULL, &gs4R);
			SDL_RenderCopy(src->loadRenderer, src->gs5Texture, NULL, &gs5R);*/
			break;
		case 3:
			/*SDL_RenderCopy(src->loadRenderer, src->gs1Texture, NULL, &gs1R);
			SDL_RenderCopy(src->loadRenderer, src->gs2Texture, NULL, &gs2R);*/
			SDL_RenderCopy(src->loadRenderer, src->Pgs3Texture, NULL, &gs3R);
			/*SDL_RenderCopy(src->loadRenderer, src->gs4Texture, NULL, &gs4R);
			SDL_RenderCopy(src->loadRenderer, src->gs5Texture, NULL, &gs5R);*/
			break;
		case 4:
			/*SDL_RenderCopy(src->loadRenderer, src->gs1Texture, NULL, &gs1R);
			SDL_RenderCopy(src->loadRenderer, src->gs2Texture, NULL, &gs2R);
			SDL_RenderCopy(src->loadRenderer, src->gs3Texture, NULL, &gs3R);*/
			SDL_RenderCopy(src->loadRenderer, src->Pgs4Texture, NULL, &gs4R);
			//SDL_RenderCopy(src->loadRenderer, src->gs5Texture, NULL, &gs5R);
			break;
		case 5:
			/*SDL_RenderCopy(src->loadRenderer, src->gs1Texture, NULL, &gs1R);
			SDL_RenderCopy(src->loadRenderer, src->gs2Texture, NULL, &gs2R);
			SDL_RenderCopy(src->loadRenderer, src->gs3Texture, NULL, &gs3R);
			SDL_RenderCopy(src->loadRenderer, src->gs4Texture, NULL, &gs4R);*/
			SDL_RenderCopy(src->loadRenderer, src->Pgs5Texture, NULL, &gs5R);
			break;

	}
	SDL_RenderCopy(src->loadRenderer, src->backTexture, NULL, &backR);
	if(selectedSlot > 0)
		SDL_RenderCopy(src->loadRenderer, src->loadTexture, NULL, &loadR);
	else
		SDL_RenderCopy(src->loadRenderer, src->unloadTexture, NULL, &loadR);
	SDL_RenderPresent(src->loadRenderer);
}

SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event) {
	if (!src) {
		return SP_LOAD_INVALID_ARGUMENT;
	}
	if (!event) {
		return SP_LOAD_INVALID_ARGUMENT;
	}
	int fsn = getFullSlotsNum();
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnGS1(event->button.x, event->button.y) && fsn >= 1) {  //fsn indicates if GS1 button is shown on the screen
			selectedSlot = 1;
			return SP_LOAD_NONE;
			//return SP_LOAD_GS1;
		}
		if (isClickOnGS2(event->button.x, event->button.y) && fsn >= 2) {
			selectedSlot = 2;
			return SP_LOAD_NONE;
			//return SP_LOAD_GS2;
		}
		if (isClickOnGS3(event->button.x, event->button.y) && fsn >= 3) {
			selectedSlot = 3;
			return SP_LOAD_NONE;
			//return SP_LOAD_GS3;
		}
		if (isClickOnGS4(event->button.x, event->button.y) && fsn >= 4) {
			selectedSlot = 4;
			return SP_LOAD_NONE;
			//return SP_LOAD_GS4;
		}
		if (isClickOnGS5(event->button.x, event->button.y) && fsn >= 5) {
			selectedSlot = 5;
			return SP_LOAD_NONE;
			//return SP_LOAD_GS5;
		}
		if (isClickOnBack(event->button.x, event->button.y)) {
			return SP_LOAD_BACK;
		}
		else if (isClickOnLoad(event->button.x, event->button.y) && selectedSlot > 0) {
			switch(selectedSlot){
				case 1:
					return SP_LOAD_GS1;
				case 2:
					return SP_LOAD_GS2;
				case 3:
					return SP_LOAD_GS3;
				case 4:
					return SP_LOAD_GS4;
				case 5:
					return SP_LOAD_GS5;
			}
			//return SP_LOAD_LOAD;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_LOAD_EXIT;
		}
		break;
	default:
		return SP_LOAD_NONE;
	}
	return SP_LOAD_NONE;
}

void spLoadWindowHide(SPLoadWin* src) {
	SDL_HideWindow(src->loadWindow);
}

void spLoadWindowShow(SPLoadWin* src) {
	SDL_ShowWindow(src->loadWindow);
}

