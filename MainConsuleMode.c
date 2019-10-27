/*
 * MainGuiMode.c
 *
 *  Created on: 6 баеч 2017
 *      Author: ori
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "SPCHParser.h"
#include "SPCHGame.h"
#include "SPCHMainAux.h"

#define SP_MAX_LINE_LENGTH 1024
#define SP_CH_SETTING_STATE 'S'
#define SP_CH_GAME_STATE 'G'


int ConsuleMode(){
char str[SP_MAX_LINE_LENGTH];
	char state = SP_CH_SETTING_STATE;
	SPCommand* command = spCommandCreate();
	while (command == NULL)																			// protective programming
		command = spCommandCreate();
	SPCHGame* game = spCHGameCreate(1, 2, 1, 1);
	while (game == NULL)																			// protective programming
		game = spCHGameCreate(1, 2, 1, 1);
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n"); fflush (stdout);
	while(true){
		if ( (state != SP_CH_GAME_STATE) || spCHPlayerTurn(game)){									// get another command
			fgets(str, sizeof(str), stdin);
			spParserPraseLine(command,str);
		}
		if(command->cmd == 6){     																	//quit command
			printf("Exiting...\n"); fflush (stdout);
			spCommandDestroy(command); spCHGameDestroy(game); return 0;
		}
		if (state == SP_CH_SETTING_STATE){														//settings state behavior
				if(command->cmd == 7){         															//start command
					state = SP_CH_GAME_STATE;
					if (!spCHPlayerTurn(game)){															//AI turn
							//(game->game_mode == 1) && (game->currentPlayer != game->user_color)){
						computerMove(game);
						int tmp = terminalComCase(game);
						if(tmp == 8 || tmp == 6 || tmp == 4){											//game is over
							spCommandDestroy(command); spCHGameDestroy(game); return 0;
						}
					}
					spCHAnotherRound(game);																//another round
					continue;
				}else
					settingState(command,game);															//setting command
		}else{																					//game state behavior
			if (spCHPlayerTurn(game) ){																//if not AI
				if (command->cmd == 12){																//reset command
					resetCase(game);
					state = SP_CH_SETTING_STATE;
					continue;
				}
				if (gameState(command,game)){															//Implementing the command
					int tmp = terminalCase(game);
					if (tmp == 8 || tmp == 6 || tmp == 4){												//game is over
						spCommandDestroy(command); spCHGameDestroy(game); return 0;
					}
					if (game->game_mode == 2){															//2 players massages
						checkCase(game);
						spCHGamePrintBoard(game);
					}
				}
				printEM(game);																			//enter move massage
			}else{																					//AI turn
				computerMove(game);																		//implementing move
				int tmp = terminalComCase(game);
				if (tmp == 8 || tmp == 6 || tmp == 4){													//game is over
					spCommandDestroy(command); spCHGameDestroy(game); return 0;
				}else{
					spCHAnotherRound(game);																//another round
				}
			}
		}
	}
	spCommandDestroy(command); spCHGameDestroy(game); return 0;
	return 0;
}
