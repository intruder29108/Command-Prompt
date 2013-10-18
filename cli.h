/*
 *	cli.h
 *
 *	Author		: Antony Clince Alex
 *	Date		: 15 October 2013
 *	Description :
 *					Structures, Macros and typedefs used by CLI
 */

#ifndef CLI_H
#define CLI_H


/* Project includes goes here */
#include "utils.h"
#include "historystack.h"

/* Macro Definition goes here */
#define MAX_CMD_BUFFER_SIZE		100
#define MAX_PARAM_BUFFER_SIZE 	20
#define MAX_NUM_PARAMS			10

/* Typedefs goes here */
typedef enum {INSIDE_WORD, OUTSIDE_WORD} PARSE_STATE;
typedef enum {LEFT_RIGHT_ACTIVE, LEFT_RIGHT_IDLE} LEFT_RIGHT_STATE;
typedef int (*funcPtr)(void *);
typedef funcPtr (*pFuncPtr)(void *);  
typedef struct 
{
	char cmdBuff[MAX_CMD_BUFFER_SIZE];
	int argc;
	char argv[MAX_NUM_PARAMS][MAX_PARAM_BUFFER_SIZE];
	int numTrailingWhiteSpaces[MAX_NUM_PARAMS];
	PARSE_STATE parse_state;
	LEFT_RIGHT_STATE left_right_state;
} PMSG;
typedef struct 
{
	char cmdName[MAX_PARAM_BUFFER_SIZE];
	RET_CODE (*cmdFuncPtr)(PMSG *);
	int argNum;
	char cmdUsage[MAX_PARAM_BUFFER_SIZE];
} CMD_TABLE;

/* Forward declaration for functions */

/* State machine implementation */
funcPtr InitState(void *);
funcPtr InputState(void *);
funcPtr ParseState(void *);
funcPtr RespondState(void *);
RET_CODE CLILoop();

/* Command Function declarations */
RET_CODE help(PMSG *);
RET_CODE echo(PMSG *);
RET_CODE add(PMSG *);
RET_CODE history(PMSG *);
RET_CODE myexit(PMSG *);

#endif