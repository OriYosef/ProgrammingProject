/*
 * SPCHParser.c
 *
 *  Created on: Sep 30, 2017
 *      Author: Calypso
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024

//a type used to represent a command
typedef enum {
	game_mode,            ///settings state commands
	difficulty,
	user_color,
	load,					//3
	sp_default,
	print_setting,
	quit,					//6
	start,
	move_to,
	get_moves,					//9
	save,
	undo,
	reset,						//12
	invalid_command,
	casle,
} SP_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg; 									//is set to true if the line contains a valid argument
	int arg;
	bool validCoord; 								//is set to true if the line contains valid coordinates
	int* coordinates;
	char* address;
} SPCommand;

/*
 * this function creates a SPCommand pointer
 * it allocates the necessary memory
 * in case of a memory allocation failure prints
 * an ERROR message and returns NULL
 */
SPCommand* spCommandCreate(void){
	SPCommand* command = (SPCommand*) malloc(sizeof(SPCommand));
	if(!command){
		printf("ERROR: SPCommand memory allocation has failed");fflush (stdout);
		return NULL;
	}
	command->coordinates = malloc(sizeof(int)*5);
	if(!command->coordinates){
		printf("ERROR: SPCommand memory allocation has failed");fflush (stdout);
		free(command);
		return NULL;
	}
	command->address = malloc(sizeof(char)*200);  //200 is enough right??
	if(!command->address){
			printf("ERROR: SPCommand memory allocation has failed");fflush (stdout);
			free(command->coordinates);
			free(command);
			return NULL;
		}
	return command;
}

/*
 * this function receives a SPCommand pointer and frees the memory
 * allocated for it.
 * in case the received pointer is NULL this function does nothing
 */
void spCommandDestroy(SPCommand* cmd){
	if (cmd == NULL){
		printf("ERROR: spCommandDestroy null pointer input\n");fflush (stdout);
		return;
	}else{
		free(cmd->address);
		free(cmd->coordinates);
		free(cmd);
	}
}


/**
 * relevant to the three functions below
 * Checks if a specified string represents a valid integer for each case game_mode/difficulty/user_color.
 * It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool isGameModeInt(const char* str){
	int val;
	val = atoi(str);
	if ((val==1)||(val==2)){
		return true;
	}
	return false;
}


bool isDifficultyInt(const char* str){
	int val;
	val = atoi(str);
	if ((val > 0)&&(val < 6)){
		return true;
	}
	return false;
}


bool isColorInt(const char* str){
	int val;
	val = atoi(str);
	if ((val==1)||(val==0)){
		return true;
	}
	return false;
}

/*
 * checks if a given coordinate is within the game board limits
 */
bool isItOnBoard(int n, char a){
	if ((n >= 1) && (n <= 8) && (a >= 'A') && (a <= 'H')){
		return true;
	}
	else
		return false;
}

/*
 * checks if a given string is structured as follows "<int,char>"
 */
bool structAssert(char* str){
	int cnt=1;
	if (str[0] == '<'){
		if (&str[1] != 0){
			while (str[cnt] != ','){
				cnt++;
				if (cnt > 20)
					break;
			}
			cnt++;
			if (&str[cnt] != 0){
				while (str[cnt] != '>'){
					cnt++;
					if (cnt > 20)
						break;
				}
				if ((str[cnt] == '>') && (str[cnt+1] == '\0')){
					return true;
				}
			}
		}
	}
	return false;
}



/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
void spParserPraseLine(SPCommand* oneCommand, const char* str){
	char copy[1024];
	char* str2;
	char* str3;
	char* str4;
	char* str5;
	char* str6;
	const char s[8] = " \t\r\n";
	oneCommand->cmd = 13;
	oneCommand->validArg = false;
	oneCommand->validCoord = false;
	if(!str){       ////for NULL pointer case
			oneCommand->cmd = 13;
			oneCommand->validArg = false;
			oneCommand->validCoord = false;
			return ;
		}
		if (str[0] == '\n'){
			oneCommand->cmd = 13;
			oneCommand->validArg = false;
			oneCommand->validCoord = false;
			return ;
		}
	strcpy(copy, str);
	str2 = strtok(copy, s);
	str3 = strtok(NULL, s);
	str4 = strtok(NULL, s);
	str5 = strtok(NULL, s);
	str6 = strtok(NULL, s);
	if(!str2){       ////for NULL pointer case
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((str3 == NULL)&&(strcmp(str2, "default")==0)){
		oneCommand->cmd = 4;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((str3 == NULL)&&(strcmp(str2, "print_setting")==0)){
		oneCommand->cmd = 5;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}
	if ((str3 == NULL)&&(strcmp(str2, "quit")==0)){
		oneCommand->cmd = 6;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}
	if ((str3 == NULL)&&(strcmp(str2, "start")==0)){
		oneCommand->cmd = 7;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}
	if ((str3 == NULL)&&(strcmp(str2, "undo")==0)){
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		oneCommand->cmd = 11;
		return ;
	}
	if ((str3 == NULL)&&(strcmp(str2, "reset")==0)){
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		oneCommand->cmd = 12;
		return ;
	}

	if ((str3 == NULL)&&(strcmp(str2, "game_mode") == 0)){
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((strcmp(str2, "game_mode") == 0)&&(isGameModeInt(str3))&&(str4 == NULL)){
		oneCommand->cmd = 0;
		oneCommand->validArg = true;
		oneCommand->validCoord = false;
		oneCommand->arg = atoi(str3);
	}

	if ((strcmp(str2, "game_mode") == 0)&&(!isGameModeInt(str3))&&(str4 == NULL)){
		oneCommand->cmd = 0;
		oneCommand->validArg =false;
		oneCommand->validCoord = false;
	}

	if ((str3 == NULL)&&(strcmp(str2, "difficulty") == 0)){
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((strcmp(str2, "difficulty") == 0)&&(isDifficultyInt(str3))&&(str4 == NULL)){
		oneCommand->cmd = 1;
		oneCommand->validArg = true;
		oneCommand->validCoord = false;
		oneCommand->arg = atoi(str3);
	}

	if ((strcmp(str2, "difficulty") == 0)&&(!isDifficultyInt(str3))&&(str4 == NULL)){
		oneCommand->cmd = 1;
		oneCommand->validArg =false;
		oneCommand->validCoord = false;
	}

	if ((str3 == NULL)&&(strcmp(str2, "user_color") == 0)){
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((strcmp(str2, "user_color") == 0)&&(isColorInt(str3))&&(str4 == NULL)){
		oneCommand->cmd = 2;
		oneCommand->validArg = true;
		oneCommand->validCoord = false;
		oneCommand->arg = atoi(str3);
	}

	if ((strcmp(str2, "user_color") == 0)&&(!isColorInt(str3))&&(str4 == NULL)){
		oneCommand->cmd = 2;
		oneCommand->validArg =false;
		oneCommand->validCoord = false;
	}

	if ((str3 == NULL)&&(strcmp(str2, "load") == 0)){
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((strcmp(str2, "load") == 0)&&(str3 != NULL)&&(str4 == NULL)){
		oneCommand->cmd = 3;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		strcpy(oneCommand->address, str3);
	}

	if ((str3 == NULL)&&(strcmp(str2, "save") == 0)){
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		return ;
	}

	if ((strcmp(str2, "save") == 0)&&(str3 != NULL)&&(str4 == NULL)){
		oneCommand->cmd = 10;
		oneCommand->validArg = false;
		oneCommand->validCoord = false;
		strcpy(oneCommand->address, str3);
	}

	if ((str3 == NULL)&&(strcmp(str2, "move") == 0)){
		oneCommand->cmd = 13;
		oneCommand->validArg = false;
		return ;
	}

	if ((strcmp(str2, "move") == 0)&&(str3 != NULL)&&(str4 != NULL) && (strcmp(str4, "to") == 0) && (str5 != NULL)&&(str6 == NULL)){
		if(structAssert(str3) && structAssert(str5)){
			if(isItOnBoard(atoi(&str3[1]), str3[3]) && isItOnBoard(atoi(&str5[1]),str5[3])){
				oneCommand->cmd = 8;
				oneCommand->validCoord = true;
				oneCommand->coordinates[0] = (int)str3[1] - 49; 				 ///working on 0-7 base
				oneCommand->coordinates[1] = (int)str3[3] - 65;     ///to get 0 if the char is 'A'
				oneCommand->coordinates[2] = (int) str5[1] - 49;
				oneCommand->coordinates[3] = (int)str5[3] - 65;
				return ;
			}else{               ///in this case will print "invalid position on the board"
				oneCommand->cmd = 8;
				oneCommand->validCoord = false;
				return ;
			}
		}
	}

	if ((str3 == NULL)&&(strcmp(str2, "get_moves") == 0)){
			oneCommand->cmd = 13;
			oneCommand->validArg = false;
			return ;
		}
	if ((strcmp(str2, "get_moves") == 0) && (str3 != NULL) && (str4 == NULL)){
		if(structAssert(str3)){
			if(isItOnBoard(atoi(&str3[1]),str3[3])){
				oneCommand->cmd = 9;
				oneCommand->validCoord = true;
				oneCommand->coordinates[0] = str3[1] - 49;
				oneCommand->coordinates[1] = (int)str3[3] - 65;
				return ;

			}
			else{
				oneCommand->cmd = 9;
				oneCommand->validCoord = false;
				return ;
			}
		}
	}


	if ((str3 == NULL)&&(strcmp(str2, "castle") == 0)){
				oneCommand->cmd = 13;
				oneCommand->validArg = false;
				return ;
			}
		if ((strcmp(str2, "castle") == 0) && (str3 != NULL) && (str4 == NULL)){
			if(structAssert(str3)){
				if(isItOnBoard(atoi(&str3[1]),str3[3])){
					oneCommand->cmd = 14;
					oneCommand->validCoord = true;
					oneCommand->coordinates[0] = str3[1] - 49;
					oneCommand->coordinates[1] = (int)str3[3] - 65;
					return ;
				}else{
					oneCommand->cmd = 14;
					oneCommand->validCoord = false;
					return ;
				}
			}
		}
		return ;
	}



