/*
 * MainConsuleMode.c
 *
 *  Created on: 6 баеч 2017
 *      Author: ori
 */

#include <stdio.h>
#include "SPCHGUIManager.h"


int GuiMode(){
if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SPCHGame* game = spCHGameCreate(1,2,1,1);
	if (game == NULL){
		SDL_Quit();
		return 0;
	}
	SPGuiManager* manager = spManagerCreate();
	if (manager == NULL ) {
		spCHGameDestroy(game);
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		if (spManagerHandleEvent(manager, &event, game) == SP_MANAGER_QUIT) {
			break;
		}
		spManagerDraw(manager);
	}
	spManagerDestroy(manager);
	spCHGameDestroy(game);
	SDL_Quit();
	return 0;
}
