/*
 *	cli.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	CLI Implementation
 */

/* Standard Library includes goes here */
#include <stdio.h>
#include <string.h>

/* Project includes goes here */
#include "cli.h"
#include "autocompletestack.h"


/* Command Table Definition */
CMD_TABLE cmdTable[] = {
	{"help",	help ,		0,	"help"},
	{"echo",	echo,		1,	"echo <string>"},
	{"add",		add ,		2,	"add  <num1> <num2>"},
	{"history", history,	0,  "history"},
	{"exit",	myexit, 	0,	"exit"}
};

/* Global Variable goes here */

/* Global Message Structure */
PMSG g_pmsg;
/* Global History Stack Structure */
HIST_STACK g_histstack;
/* Global Auto Complete Stack Structure */
AUTOCOMPL_STRUCT g_autocomplstack;


/* Function definitions goes here */

RET_CODE CLILoop(void)
{
 	/* Set the SMHandler to Init */
	static pFuncPtr SMHandler = (pFuncPtr)InitState;

 	/* Loop the state machine */
	SMHandler = (pFuncPtr)SMHandler((void *)&g_pmsg);

	if(SMHandler == NULL)
	{
 		/* Exit CLI with Error */
		return EXIT_CODE;
	}

	return SUCCESS;
}

funcPtr InitState(void *msg)
{
	PMSG *pmsg = (PMSG *)msg;

 	/* Initialize message structure */
	memset(pmsg, 0, sizeof(PMSG));
	pmsg->parse_state = OUTSIDE_WORD;
	pmsg->left_right_state = LEFT_RIGHT_IDLE;

	/* Initialize access index for history stack */
	g_histstack.accessIndex = STACK_HEAD;
	g_histstack.stack_pop_state = STACK_IDLE;

	/* Initialize autocomplete stack */
	initializeAutoCompl(&g_autocomplstack);

	putchar('>');

	return (funcPtr)InputState;
}

funcPtr InputState(void *msg)
{
	PMSG *pmsg = (PMSG *)msg;
	int ch;
 	static int len = 0;		/**< Keep track of command buffer length */
 	int iVal;
 	int cmdBufLen = 0;			/**< Cache string length */

	/* Catch Null Pointer Exception */
 	if(pmsg == NULL)
 	{
 		return (funcPtr)NULL;
 	}

 	/* Get user input */
 	ch = getch();

 	/* Combined key presses */
 	if(ch == ESC_SEQ)
 	{
 		/* Get second key code */
 		ch = getch();
 		if(ch == UP)
 		{
 			/* Check if History Stack is empty */
 			if(IS_STACK_EMPTY(g_histstack) == FALSE)
 			{
 				/* Clean the command buffer */
 				if(stack_pop_up(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
 				{
 					return (funcPtr)InputState;
 				}
 				/* Set up stack pop state */
 				g_histstack.stack_pop_state = STACK_UP;

 				/* Clear the current Line */
 				while(len--)
 				{
 					putchar(BACKSPACE);
 					putchar(SPACE);
 					putchar(BACKSPACE);
 				}
 				len = mystrlen(pmsg->cmdBuff);
 				printf("%s", pmsg->cmdBuff);
 			}
 
 		}
 		else if(ch == DOWN)
 		{
 			/* Check if History Stack is empty */
 			if(IS_STACK_EMPTY(g_histstack) == FALSE)
 			{
 				/* Clean the command buffer */
 				if(stack_pop_down(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
 				{
 					return (funcPtr)InputState;
 				}
 				/* Set up stack pop state */
 				g_histstack.stack_pop_state = STACK_DOWN;
 				/* Clear the current Line */
 				while(len--)
 				{
 					putchar(BACKSPACE);
 					putchar(SPACE);
 					putchar(BACKSPACE);
 				}
 				len = mystrlen(pmsg->cmdBuff);
 				printf("%s", pmsg->cmdBuff);
 			}
 		}
 		else if(ch == LEFT)
 		{
 			if(len > 0)
 			{
 				pmsg->left_right_state = LEFT_RIGHT_ACTIVE;
 				putchar(BACKSPACE);
 				len--;
 			}
 			
 		}
 		else if(ch == RIGHT)
 		{
 			if(len < mystrlen(pmsg->cmdBuff))
 			{
 				pmsg->left_right_state = LEFT_RIGHT_ACTIVE;
 				putchar(pmsg->cmdBuff[len++]);
 			}
 			else
 			{
 				pmsg->left_right_state = LEFT_RIGHT_IDLE;
 			}
 		}

 	}

 	else if(ch == TAB)
 	{
 		/* Search the string in command table */
 		if(findMatch(&g_autocomplstack, pmsg->cmdBuff, cmdTable
 					, sizeof(cmdTable)/sizeof(CMD_TABLE)) == AUTOCOMPL_SUCCESS)
 		{ 		
 			/* Clear the current Line */
 			while(len--)
 			{
 				putchar(BACKSPACE);
 				putchar(SPACE);
 				putchar(BACKSPACE);
 			}
 			len = mystrlen(pmsg->cmdBuff);
 			printf("%s", pmsg->cmdBuff);
 		}
 	}

 	else if(ch == CARR_RET || ch == NEW_LINE)
 	{
 		if(len <= MAX_CMD_BUFFER_SIZE)
 		{
 			/* If LEFT/RIGHT active in middle of word, take entire word */
 			if(pmsg->left_right_state == LEFT_RIGHT_ACTIVE)
 			{
 				len = mystrlen(pmsg->cmdBuff);
 			}
 			pmsg->cmdBuff[len] = EOS;
 			putchar(ch);
 			len = 0;

 			/* Push to history stack */
 			if(stack_push(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
 			{
 				printf("Error : Stack Push !!!");
 			}

 			return (funcPtr)ParseState;
 		}
 	}
 	else if(ch == BACKSPACE)
 	{
 		if(len)
 		{
 			len--;
 			/* Terminate string only if not in middle of word */
 			if(pmsg->left_right_state == LEFT_RIGHT_IDLE)
 			{
 				pmsg->cmdBuff[len] = EOS;				
 				putchar(BACKSPACE);
 				putchar(SPACE);
 				putchar(BACKSPACE);

 			}
 			/* Shift entire command buffer to left once */
 			else
 			{
 				putchar(BACKSPACE);
 				cmdBufLen = mystrlen(pmsg->cmdBuff);
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					putchar(pmsg->cmdBuff[iVal + 1]);
 					pmsg->cmdBuff[iVal] = pmsg->cmdBuff[iVal + 1];
 				}
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					putchar(BACKSPACE);
 				}
 			}
 			/* Reset AutoComplete State */
 			g_autocomplstack.autoComplState = AUTO_IDLE;
 		}
 	}
 	else
 	{
 		if(len < MAX_CMD_BUFFER_SIZE)
 		{
 			if(pmsg->left_right_state == LEFT_RIGHT_IDLE)
 			{
 				pmsg->cmdBuff[len++] = ch;
	 			putchar(ch);	
 			}
 			else
 			{
 				cmdBufLen = mystrlen(pmsg->cmdBuff);
 				putchar(ch);	
 				/* RePrint Rest of the command buffer to create moving effect */
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					putchar(pmsg->cmdBuff[iVal]);
 				}
 				/* Push the entire command buffer right by one position */
 				for(iVal = cmdBufLen; iVal >= len; iVal--)
 				{
 					pmsg->cmdBuff[iVal + 1] = pmsg->cmdBuff[iVal];
 				}
 				/* Reset cursor to next location */
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					putchar(BACKSPACE);
 				}
 				pmsg->cmdBuff[len++] = ch;
 			}

 			/* Reset AutoComplete State */
 			g_autocomplstack.autoComplState = AUTO_IDLE;
 			
 		}
 	}

 	return (funcPtr)InputState;
}

funcPtr ParseState(void *msg)
{
	PMSG *pmsg = (PMSG *)msg;
	int len = 0;
	int cmdLen = 0;

	while(pmsg->cmdBuff[cmdLen] != EOS)
	{
		if(pmsg->cmdBuff[cmdLen] == SPACE)
		{
			if(pmsg->parse_state == INSIDE_WORD)
			{
				pmsg->argv[pmsg->argc++][len] = EOS;
				pmsg->parse_state = OUTSIDE_WORD;
				len = 0;
			}
		}
		else
		{
			pmsg->argv[pmsg->argc][len++] = pmsg->cmdBuff[cmdLen];
			pmsg->parse_state = INSIDE_WORD;
		}
		cmdLen++;
	}

	if(pmsg->parse_state == INSIDE_WORD)
	{
		pmsg->argv[pmsg->argc++][len] = EOS;
	}

	return (funcPtr)RespondState;
}


funcPtr RespondState(void *msg)
 {
 	PMSG *pmsg = (PMSG *)msg;
 	int iVal = 0;
 	int parameter;
 	int numCmds;
 	int cmdFound = FALSE;


 	/* Return incase of no input */
 	if(pmsg->argc == 0)
 	{
 		printf("\n");
 		return (funcPtr)InitState;
 	}

 	numCmds = sizeof(cmdTable)/sizeof(CMD_TABLE);
 	printf("\n\n");

 	for(iVal = 0; iVal < numCmds; iVal++)
 	{
 		if(mystrcmp(pmsg->argv[0], cmdTable[iVal].cmdName) == 0)
 		{
 			cmdFound = TRUE;
 			if((pmsg->argc - 1) != cmdTable[iVal].argNum)
 			{
 				printf("Incorrect Usage !!!\n");
 				printf("%s\n", cmdTable[iVal].cmdUsage);
 				break;
 			}

 			switch(cmdTable[iVal].cmdFuncPtr(pmsg))
 			{
 				case PARAM_ERR:
 					printf("Parameter Error !!!\n");
 					break;
 				case EXIT_CODE:
 					return (funcPtr)NULL;
 					break;
 			}
 		}
 	}

 	if(cmdFound == FALSE)
 	{
 		printf("Command not found !!!, Try \"help\" for list\n");
 	}


 	/* Loop to start of SM */
 	return (funcPtr)InitState;
 }


 /* Command Definitions */

 RET_CODE help(PMSG *pmsg)
 {
 	int numCmds, iVal;

 	numCmds = sizeof(cmdTable)/sizeof(CMD_TABLE);
 	printf("Command\tUsage\n");
 	printf("--------------\n");
 	for(iVal = 0; iVal < numCmds; iVal++)
 	{
 		printf("%s\t%s\n", cmdTable[iVal].cmdName, cmdTable[iVal].cmdUsage);
 	}

 	return SUCCESS;
 }

 RET_CODE echo(PMSG *pmsg)
 {
 	printf("%s\n", pmsg->argv[1]);
 	return SUCCESS;
 }

 RET_CODE add(PMSG *pmsg)
 {
 	int x, y;

 	if(ISHEX(pmsg->argv[1]))
 	{
 		if(myatoi(&pmsg->argv[1][2], HEX, (int *)&x) == PARAM_ERR)
 		{
 			return PARAM_ERR;
 		}
 	}
 	else
 	{
 		if(myatoi(&pmsg->argv[1][0], DEC, (int *)&x) == PARAM_ERR)
 		{
 			return PARAM_ERR;
 		}

 	}

 	if(ISHEX(pmsg->argv[2]))
 	{
 		if(myatoi(&pmsg->argv[2][2], HEX, (int *)&y) == PARAM_ERR)
 		{
 			return PARAM_ERR;
 		}
 	}
 	else
 	{
 		if(myatoi(&pmsg->argv[2][0], DEC, (int *)&y) == PARAM_ERR)
 		{
 			return PARAM_ERR;
 		}
 	}
 	printf("%d + %d = %d\n",x, y, x+y);

 	return SUCCESS;
 }

 RET_CODE history(PMSG *pmsg)
 {
 	int iVal = 0;
 	/* Print the history stack */
 	for(iVal = 0; iVal < g_histstack.stackSize; iVal++)
 	{
 		printf("%s\n", g_histstack.stackBuffer[iVal]);
 	}

 	return SUCCESS;
 }

 RET_CODE myexit(PMSG *pmsg)
 {
 	printf("Exiting ...\n");
 	return EXIT_CODE;
 }
