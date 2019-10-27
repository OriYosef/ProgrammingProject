/*
 * SPCHArrayList.c
 *
 *  Created on: Sep 27, 2017
 *      Author: Calypso
 */


#define SP_CH_GAME_N_ROWS 8
#define SP_CH_GAME_N_COLUMNS 8
#define SP_CH_GAME_N_MAX_PREV_MOVES 6
#define SP_CH_GAME_TOTAL_SIZE SP_CH_GAME_N_ROWS*SP_CH_GAME_N_COLUMNS


#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * SPCHArrayList summary:
 *
 * A container that represents a fixed size linked list. The capcity of the list
 * is specified at the creation.
 * the elements to cr store are matrixes. the information is a list of char from index <0,0> to <7,7>.
 * A summary of the supported functions
 * is given below:
 *
 * spArrayListCreate       - Creates an empty array list with a specified
 *                           max capacity.
 * spArrayListCopy         - Creates an exact copy of a specified array list.
 * spArrayListDestroy      - Frees all memory resources associated with an array
 *                           list.
 * spArrayListAddFirst2    - Inserts an element at the beginning of the array
 * spArrayListDeleteLast   - Removes an element from the end of the array list
 * spArrayListIsEmpty      - Returns true if the array list contains no elements.
 */


typedef struct sp_array_list_t {
	char* elements;
	int actualSize;
	int maxSize;
} SPArrayList;

/**
 * A type used for massages
 */
typedef enum sp_array_list_message_t {
	SP_ARRAY_LIST_SUCCESS,
	SP_ARRAY_LIST_INVALID_ARGUMENT,
} SP_ARRAY_LIST_MESSAGE;

/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
SPArrayList* spArrayListCreate(int maxSize){   ////max size should always be 6!!
	if (maxSize<=0){
		return NULL;
	}
	SPArrayList* instant = (SPArrayList*) malloc(sizeof(SPArrayList));
	if (!instant){
		printf("ERROR: spArrayListCreate memory allocation has failed\n"); fflush (stdout);
		return NULL;
	}
	instant->elements = malloc(sizeof(char)*maxSize*SP_CH_GAME_TOTAL_SIZE);     ////may be problematic
	if (instant->elements == NULL){
		printf("ERROR: spArrayListCreate memory allocation has failed\n"); fflush (stdout);
		free(instant);
		return NULL;
	}
	instant->maxSize = maxSize;
	instant->actualSize = 0;
	return instant;
}
/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
SPArrayList* spArrayListCopy(SPArrayList* src){
	if (src == NULL){
		printf("ERROR: spArrayListCopy null pointer input\n");fflush (stdout);
		return NULL;
	}
	SPArrayList* instant = spArrayListCreate(src->maxSize);
	while (instant == NULL)
		instant = spArrayListCreate(src->maxSize);
	if (src->elements == NULL){
		instant->elements = NULL;
	}else{
		for (int i=0; i< src->actualSize*SP_CH_GAME_TOTAL_SIZE; i++){
					instant->elements[i] = src->elements[i];
			}
		}
	instant->actualSize = src->actualSize;
	return instant;
	}

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void spArrayListDestroy(SPArrayList* src){
	if (src == NULL){
		printf("ERROR: spArrayListDestroy null pointer input\n");fflush (stdout);
		return;
	}else{
		free(src->elements);
		free(src);
	}
}

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * @param src   - the source array list
 * @param elem  - the new gameBoard to store
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddFirst2(SPArrayList* src, char elem[SP_CH_GAME_N_ROWS][SP_CH_GAME_N_COLUMNS]){
	if (src == NULL){
		printf("ERROR: spArrayListAddFirst2 null pointer input\n");fflush (stdout);
		return 1;
	}
	for (int i= (SP_CH_GAME_TOTAL_SIZE*src->maxSize -1) ; i>(SP_CH_GAME_TOTAL_SIZE-1) ; i--){			// moving the elements to the right
		src->elements[i]=src->elements[i-SP_CH_GAME_TOTAL_SIZE];
	}
	for (int row=0 ; row<SP_CH_GAME_N_ROWS; row++){
		for (int col=0 ; col<SP_CH_GAME_N_COLUMNS; col++){
				src->elements[row*SP_CH_GAME_N_ROWS+col]= elem[row][col];								// inserting new elements
		}
	}
	if (src->actualSize<SP_CH_GAME_N_MAX_PREV_MOVES)
		src->actualSize++;
	return 0;
}


/**
 * shifting all the elements one place down.
 * @param src - the source array list
 */
void spArrayListDeleteLast(SPArrayList* src){
	if (src == NULL){
		printf("ERROR: spArrayListDeleteLast null pointer input\n");fflush (stdout);
		return;
	}else{
		for (int i=0; i < SP_CH_GAME_TOTAL_SIZE*(src->maxSize -1) ; i++){						// moving the elements to the left
			src->elements[i]=src->elements[i+SP_CH_GAME_TOTAL_SIZE];
		}
		for (int i=(src->maxSize-1)*SP_CH_GAME_TOTAL_SIZE; i < SP_CH_GAME_TOTAL_SIZE*src->maxSize ; i++){
			src->elements[i]=0;
		}
		src->actualSize--;
	}
}



/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList* src){
	if (src == NULL){
		printf("ERROR: spArrayListIsEmpty null pointer input\n");fflush (stdout);
		return false;
	}else{
		if (src->actualSize==0){
			return true;
		}else{
			return false;
		}
	}
}

