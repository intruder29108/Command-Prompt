/**
 *  \file          autocompletestack.h
 *  
 *  \brief         Implements Data Structures and macros
 *                 to implement auto-complete feature.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */
#ifndef AUTOCOMPLETESTACK_H
#define AUTOCOMPLETESTACK_H

/***** Standard includes goes here *****/

/***** Project includes goes here *****/

/***** Macros goes here *****/
#define MAX_NUM_MATCHES		20
#define MAX_MATCH_SIZE		20

/***** Typedefs and Enums goes here *****/
typedef enum {AUTOCOMPL_NOHITS, AUTOCOMPL_NULL_ERR, AUTOCOMPL_ERR, AUTOCOMPL_SUCCESS} AUTOCOMPL_RET_CODE;
typedef enum {AUTO_IDLE, AUTO_ACTIVE} AUTOCOMPL_STATE;
typedef struct
{
	char matchList[MAX_NUM_MATCHES][MAX_MATCH_SIZE];		/* Buffer to hold matched entires */	
	int  matchIndex;										/* Points to next location in match list */
	int  totalHits;											/* Total number of hits */
	AUTOCOMPL_STATE autoComplState;							/* Tracks the state of AutoComplete Stack */
}AUTOCOMPL_STRUCT;

/***** Global Variables *****/

/***** Extern Variables *****/

/***** Function Declarations *****/
AUTOCOMPL_RET_CODE initializeAutoCompl(AUTOCOMPL_STRUCT *autoComplStruct);
AUTOCOMPL_RET_CODE buildMatchList(AUTOCOMPL_STRUCT *autoComplStruct, char *searchStr, CMD_TABLE *cmdTable, int cmdTableSize);
AUTOCOMPL_RET_CODE findMatch(AUTOCOMPL_STRUCT *autoComplStruct, char *searchStr, CMD_TABLE *cmdTable, int cmdTableSize);

#endif
