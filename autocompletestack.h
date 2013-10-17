/*
 *	autocompletestack.h
 *
 *	Author		: Antony Clince Alex
 *	Date		: 15 October 2013
 *	Description :
 *					Structures, Macros for implementing autocomplete stack
 */

 #ifndef AUTOCOMPLETESTACK_H
 #define AUTOCOMPLETESTACK_H

 /* Macros goes here */
 #define MAX_NUM_MATCHES	20
 #define MAX_MATCH_SIZE		20

 /* Typedefs goes here */
 typedef enum {AUTOCOMPL_NOHITS, AUTOCOMPL_NULL_ERR, AUTOCOMPL_ERR, AUTOCOMPL_SUCCESS} AUTOCOMPL_RET_CODE;
 typedef enum {AUTO_IDLE, AUTO_ACTIVE} AUTOCOMPL_STATE;
 typedef struct
 {
 	char matchList[MAX_NUM_MATCHES][MAX_MATCH_SIZE];			/**< Can hold max 20 matches each of 20 characters long */
 	int  matchIndex;										/**< Points to next location in match list */
 	int  totalHits;											/**< Total number of hits */
 	AUTOCOMPL_STATE autoComplState;							/**< Tracks the state of AutoComplete Stack */
 }AUTOCOMPL_STRUCT;

 /* Forward Declarations */
 AUTOCOMPL_RET_CODE initializeAutoCompl(AUTOCOMPL_STRUCT *);
 AUTOCOMPL_RET_CODE buildMatchList(AUTOCOMPL_STRUCT *, char *, CMD_TABLE *, int );
 AUTOCOMPL_RET_CODE findMatch(AUTOCOMPL_STRUCT *, char *, CMD_TABLE *, int);

 #endif