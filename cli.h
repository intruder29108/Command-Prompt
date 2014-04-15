/**
 *  \file          cli.h
 *  
 *  \brief         Implements Data structures and macros used
 *                 to handle CLI.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */
#ifndef CLI_H
#define CLI_H

/***** Standard includes goes here *****/

/***** Project includes goes here *****/

/***** Macros goes here *****/
#define MAX_CMD_BUFFER_SIZE     40
#define MAX_PARAM_BUFFER_SIZE   10
#define MAX_CMD_DESCBUFF_SIZE   30
#define MAX_NUM_PARAMS          4

/***** Typedefs and Enums goes here *****/
typedef enum {INSIDE_WORD, OUTSIDE_WORD} PARSE_STATE;
typedef enum {LEFT_RIGHT_ACTIVE, LEFT_RIGHT_IDLE} LEFT_RIGHT_STATE;
typedef int (*funcPtr)(void *);
typedef funcPtr (*pFuncPtr)(void *);  
typedef struct 
{
    char cmdBuff[MAX_CMD_BUFFER_SIZE];
    int argc;
    char argv[MAX_NUM_PARAMS][MAX_PARAM_BUFFER_SIZE];
    PARSE_STATE parse_state;
    LEFT_RIGHT_STATE left_right_state;
} PMSG;
typedef struct 
{
    char cmdName[MAX_PARAM_BUFFER_SIZE];
    RET_CODE (*cmdFuncPtr)(PMSG *);
    int argNum;
    char cmdUsage[MAX_CMD_DESCBUFF_SIZE];
} CMD_TABLE;
/***** Global Variables *****/

/***** Extern Variables *****/

/***** Function Declarations *****/
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
#ifdef USE_HIST_STACK
RET_CODE history(PMSG *);
#endif
RET_CODE myexit(PMSG *);

#endif
