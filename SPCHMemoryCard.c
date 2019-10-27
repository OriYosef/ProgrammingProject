/*
 * SPCHMemoryCard.c
 *
 *  Created on: Oct 3, 2017
 *      Author: Calypso
 */

#include "SPCHMemoryCard.h"

/**
 * this function receives the game slot number from which
 * the user demands to load a game, and an existing game.
 * it loads the save game onto the existing game(runs over "yaanu doreset")
 * returns 0 on success, -1 on failure
 */

int spLoadFromMemory(int gs, SPCHGame* game){
	int successfulLoad;
	switch(gs){
	case 1:
		successfulLoad = loadGame("./memoryCard/gs1", game);
		break;
	case 2:
		successfulLoad = loadGame("./memoryCard/gs2", game);
		break;
	case 3:
		successfulLoad = loadGame("./memoryCard/gs3", game);
		break;
	case 4:
		successfulLoad = loadGame("./memoryCard/gs4", game);
		break;
	case 5:
		successfulLoad = loadGame("./memoryCard/gs5", game);
		break;
	}
	if(successfulLoad == -1)
		return -1;
	return 0;
}

/**
 * returns the number of full game slots from the file
 * holding this information. on failure returns -1
 */
int getFullSlotsNum(void){
	FILE* fp = fopen("./memoryCard/fullSlotsNum","r");
	if(!fp){
		printf("ERROR: getFullSlotsNum has failed - File doesn't exist or cannot be opened\n");
		return -1;
	}
	char* buf = malloc(sizeof(char)*365);
	if (!buf){
		printf("ERROR: getFullSlotsNum memory allocation has failed\n"); fflush (stdout);
		fclose(fp);
		return -1;
	}
	int fsn;
	while(fgets(buf,364,fp) != NULL){   //NULL will indicate the end of the file and end the loop
		const char del[8] = " \t\r\n";
		char copy[SP_MAX_LINE_LENGTH];
		strcpy(copy,buf);
		char* token = strtok(copy,del);
		fsn = (int)(token[0] - 48); //full slots number (48 = char value of '0')
	}
	fclose(fp);
	free(buf);
	return fsn;
}

/**
 * this function writes a number to a predetermined text file
 * returns -1 on failure
 */

int setFullSlotsNum(int fsn){
	FILE* fp = fopen("./memoryCard/fullSlotsNum","w");
	if(!fp){
		printf("ERROR: setFullSlotsNum has failed - File doesn't exist or cannot be opened\n");
		return -1;
	}
	char c = fsn + '0';
	if(c != fputc(c,fp)){
		printf("ERROR: setFullSlotsNum has failed - fputc failure\n");
		fclose(fp);
		return -1;
	}
	fclose(fp);
	return 0;
}

/**
 * saves the game received to the predetermined files, where gs1 would hold the most recent
 * saved game in a FIFO structure. the function also updates the number of full game slots
 * in the predetermined file
 * returns 0 on success , -1 on failure
 */

int spSaveToMemory(SPCHGame* game){
	if(!game){
		printf("ERROR: spSaveToMemory has failed - null pointer argument\n");fflush (stdout);
		return -1; //indicates failure
	}
	int a,b,c,d,e,f;
	f = remove("./memoryCard/gs5");
	a = rename("./memoryCard/gs4","./memoryCard/gs5");
	b = rename("./memoryCard/gs3","./memoryCard/gs4");
	c = rename("./memoryCard/gs2","./memoryCard/gs3");
	d = rename("./memoryCard/gs1","./memoryCard/gs2");
	e = saveGame("./memoryCard/gs1", game);
	if (a == -1 || b == -1 || c == -1 || d == -1 || e == 0){
		return -1;
	}
	int fsn = getFullSlotsNum();
	if(fsn == -1)
		return -1;
	else if(fsn < 5){
		fsn += 1;
		setFullSlotsNum(fsn);
	}
	if (f == 18787878)
		return 0;
	return 0;
}

