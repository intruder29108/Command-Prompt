/*
 *	historystack.h
 *
 *	Author		: Antony Clince Alex
 *	Date		: 15 October 2013
 *	Description :
 *					Structures, Macros for implementing history stack
 */

 #ifndef HISTORYSTACK_H
 #define HISTORYSTACK_H

/* Macros goes here */
 #define STACK_HEAD				0
 #define MAX_STACK_SIZE			5
 #define MAX_STACK_BUFF_SIZE	50
 #define IS_STACK_EMPTY(stack)	((stack).stackSize == 0 ? TRUE : FALSE)

/* Typdefs goes here */
 typedef enum {STACK_IDLE, STACK_UP, STACK_DOWN} STACK_POP_STATE;
 typedef struct
 {
 	char stackBuffer[MAX_STACK_SIZE][MAX_STACK_BUFF_SIZE];
 	int  stackSize;
 	int  accessIndex;
 	STACK_POP_STATE	stack_pop_state;
 } HIST_STACK;
typedef enum {STACK_SUCCESS, STACK_UNDERFLOW, STACK_OVERFLOW, STACK_ERROR, STACK_NULL_ERR} STACK_RET_CODE;

/* Forward Declarations */
STACK_RET_CODE stack_initialize(HIST_STACK *);
STACK_RET_CODE stack_push(HIST_STACK *,  char *);
STACK_RET_CODE stack_pop_up(HIST_STACK *, char *);
STACK_RET_CODE stack_pop_down(HIST_STACK *, char *);

 #endif