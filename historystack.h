/**
 *  \file          historystack.h
 *  
 *  \brief         Implements data structures and macros for
 *                 implementing History Stack.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */
#ifndef HISTORYSTACK_H
#define HISTORYSTACK_H

/***** Standard includes goes here *****/

/***** Project includes goes here *****/

/***** Macros goes here *****/
#define STACK_HEAD				0
#define MAX_STACK_SIZE			5
#define MAX_STACK_BUFF_SIZE		40
#define IS_STACK_EMPTY(stack)	((stack).stackSize == 0 ? TRUE : FALSE)

/***** Typedefs and Enums goes here *****/
typedef enum {STACK_IDLE, STACK_UP, STACK_DOWN} STACK_POP_STATE;

typedef struct
{
	char stackBuffer[MAX_STACK_SIZE][MAX_STACK_BUFF_SIZE];
	int  stackSize;
	int  accessIndex;
	STACK_POP_STATE	stack_pop_state;
} HIST_STACK;

typedef enum {STACK_SUCCESS,  STACK_FAILURE, STACK_UNDERFLOW, STACK_OVERFLOW, STACK_ERROR, STACK_NULL_ERR} STACK_RET_CODE;

/***** Global Variables *****/

/***** Extern Variables *****/

/***** Function Declarations *****/
STACK_RET_CODE stack_initialize(HIST_STACK *);
STACK_RET_CODE stack_push(HIST_STACK *,  char *);
STACK_RET_CODE stack_pop_up(HIST_STACK *, char *);
STACK_RET_CODE stack_pop_down(HIST_STACK *, char *);
STACK_RET_CODE checkIfElementExists(HIST_STACK *, char *);

#endif
