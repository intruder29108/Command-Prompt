/*
 *	cli.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	CLI Implementation
 */

/* Standard Library includes goes here */
#include <string.h>

/* Project includes goes here */
#include "ioroutines.h"
#include "cli.h"

#ifdef USE_HIST_STACK
#include "historystack.h"
#endif

#ifdef USE_AUTOCOMPL_STACK
#include "autocompletestack.h"
#endif


/* Command Table Definition */
CMD_TABLE cmdTable[] = {
	{"help",	help ,		0,	"help"},
	{"echo",	echo,		1,	"echo <string>"},
	{"add",		add ,		2,	"add  <num1> <num2>"},
#ifdef USE_HIST_STACK
	{"history", history,	0,  "history"},
#endif
	{"exit",	myexit, 	0,	"exit"}
};

/* Global Variable goes here */

/* Global Message Structure */
PMSG g_pmsg;

#ifdef USE_HIST_STACK
/* Global History Stack Structure */
HIST_STACK g_histstack;
#endif

#ifdef USE_AUTOCOMPL_STACK
/* Global Auto Complete Stack Structure */
AUTOCOMPL_STRUCT g_autocomplstack;
#endif

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

#ifdef USE_HIST_STACK
	/* Initialize access index for history stack */
	g_histstack.accessIndex = STACK_HEAD;
	g_histstack.stack_pop_state = STACK_IDLE;
#endif

#ifdef USE_AUTOCOMPL_STACK
	/* Initialize autocomplete stack */
	initializeAutoCompl(&g_autocomplstack);
#endif

	myprintstr(LINE_FEED);
	myprintstr("CLI>>");

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
 	ch = custom_ioread();

 	/* Combined key presses */
 	if(ch == ESC_SEQ)
 	{
 		/* Get second key code */
 		ch = custom_ioread();
 		if(ch == ESC_SEQ1)
 		{
 			 /* Get third key code */
 			ch = custom_ioread();
 			if(ch == UP)
 			{
#ifdef USE_HIST_STACK
 				/* Check if History Stack is empty */
 				if(IS_STACK_EMPTY(g_histstack) == FALSE)
 				{
 					/* Pop the TOS to command buffer */
 					if(stack_pop_up(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
 					{
 						return (funcPtr)InputState;
 					}
 					/* Set up stack pop state */
 					g_histstack.stack_pop_state = STACK_UP;

 					/* Clear the current Line */
 					while(len--)
 					{
 						custom_iowrite(BACKSPACE);
 						custom_iowrite(SPACE);
 						custom_iowrite(BACKSPACE);
 					}
 					len = mystrlen(pmsg->cmdBuff);
 					myprintstr(pmsg->cmdBuff);
 				}
#endif
 			}
 			else if(ch == DOWN)
 			{
#ifdef USE_HIST_STACK
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
 						custom_iowrite(BACKSPACE);
 						custom_iowrite(SPACE);
 						custom_iowrite(BACKSPACE);
 					}
 					len = mystrlen(pmsg->cmdBuff);
 					myprintstr(pmsg->cmdBuff);
 				}
#endif
 			}
 			else if(ch == LEFT)
 			{
 				if(len > 0)
 				{
 					/* Move the cursor back by one */
 					pmsg->left_right_state = LEFT_RIGHT_ACTIVE;
 					custom_iowrite(BACKSPACE);
 					len--;
 				}

 			}
 			else if(ch == RIGHT)
 			{
 				if(len < mystrlen(pmsg->cmdBuff))
 				{
 					/* Move the cursor forward and reprint */
 					pmsg->left_right_state = LEFT_RIGHT_ACTIVE;
 					custom_iowrite(pmsg->cmdBuff[len++]);
 				}
 				else
 				{
 					pmsg->left_right_state = LEFT_RIGHT_IDLE;
 				}
 			}
 		}
 	}

 	else if(ch == TAB)
 	{
#ifdef 	USE_AUTOCOMPL_STACK
 		/* Search the string in command table */
 		if(findMatch(&g_autocomplstack, pmsg->cmdBuff, cmdTable
 					, sizeof(cmdTable)/sizeof(CMD_TABLE)) == AUTOCOMPL_SUCCESS)
 		{ 		
 			/* Clear the current Line */
 			while(len--)
 			{
 				custom_iowrite(BACKSPACE);
 				custom_iowrite(SPACE);
 				custom_iowrite(BACKSPACE);
 			}
 			len = mystrlen(pmsg->cmdBuff);
 			myprintstr(pmsg->cmdBuff);
 		}
#endif
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
 			len = 0;
#ifdef USE_HIST_STACK
 			/* Push to history stack */
 			if(stack_push(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
 			{
 				myprintstr("Error : Stack Push !!!");
 			}
#endif
 			return (funcPtr)ParseState;
 		}
 	}
 	else if(ch == DELETE || ch == BACKSPACE)
 	{
 		if(len)
 		{
 			len--;
 			/* Terminate string only if not in middle of word */
 			if(pmsg->left_right_state == LEFT_RIGHT_IDLE)
 			{
 				pmsg->cmdBuff[len] = EOS;				
 				custom_iowrite(BACKSPACE);
 				custom_iowrite(SPACE);
 				custom_iowrite(BACKSPACE);

 			}
 			/* Shift entire command buffer to left once */
 			else
 			{
 				custom_iowrite(BACKSPACE);
 				cmdBufLen = mystrlen(pmsg->cmdBuff);
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					/* Since EOS is nonprintable replacing with space */
 					if(pmsg->cmdBuff[iVal + 1] == EOS)
 					{
 						custom_iowrite(SPACE);
 					}
 					else
 					{
 						custom_iowrite(pmsg->cmdBuff[iVal + 1]);
 					}
 					pmsg->cmdBuff[iVal] = pmsg->cmdBuff[iVal + 1];
 				}
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					custom_iowrite(BACKSPACE);
 				}
 			}
#ifdef USE_AUTOCOMPL_STACK
 			/* Reset AutoComplete State */
 			g_autocomplstack.autoComplState = AUTO_IDLE;
#endif
 		}
 	}
 	else
 	{
 		if(len < MAX_CMD_BUFFER_SIZE)
 		{
 			if(pmsg->left_right_state == LEFT_RIGHT_IDLE)
 			{
 				pmsg->cmdBuff[len++] = ch;
	 			custom_iowrite(ch);	
 			}
 			else
 			{
 				cmdBufLen = mystrlen(pmsg->cmdBuff);
 				custom_iowrite(ch);	
 				/* RePrint Rest of the command buffer to create moving effect */
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					custom_iowrite(pmsg->cmdBuff[iVal]);
 				}
 				/* Push the entire command buffer right by one position */
 				for(iVal = cmdBufLen; iVal >= len; iVal--)
 				{
 					pmsg->cmdBuff[iVal + 1] = pmsg->cmdBuff[iVal];
 				}
 				/* Reset cursor to next location */
 				for(iVal = len; iVal < cmdBufLen; iVal++)
 				{
 					custom_iowrite(BACKSPACE);
 				}
 				pmsg->cmdBuff[len++] = ch;
 			}
#ifdef USE_AUTOCOMPL_STACK
 			/* Reset AutoComplete State */
 			g_autocomplstack.autoComplState = AUTO_IDLE;
#endif 			
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
 		return (funcPtr)InitState;
 	}
 	/* Move the next line */
 	myprintstr(LINE_FEED);
 	numCmds = sizeof(cmdTable)/sizeof(CMD_TABLE);
 	for(iVal = 0; iVal < numCmds; iVal++)
 	{
 		if(mystrcmp(pmsg->argv[0], cmdTable[iVal].cmdName) == 0)
 		{
 			cmdFound = TRUE;
 			if((pmsg->argc - 1) != cmdTable[iVal].argNum)
 			{
 				myprintstr("Incorrect Usage !!!");
 				myprintstr(LINE_FEED);
 				myprintstr(cmdTable[iVal].cmdUsage);
 				break;
 			}

 			switch(cmdTable[iVal].cmdFuncPtr(pmsg))
 			{
 				case PARAM_ERR:
 					myprintstr("Parameter Error !!!");
 					break;
 				case EXIT_CODE:
 					return (funcPtr)NULL;
 					break;
 			}
 		}
 	}

 	if(cmdFound == FALSE)
 	{
 		myprintstr("Command not found !!!, Try \"help\" for list");
 	}


 	/* Loop to start of SM */
 	return (funcPtr)InitState;
 }


 /* Command Definitions */

 RET_CODE help(PMSG *pmsg)
 {
 	int numCmds, iVal;

 	numCmds = sizeof(cmdTable)/sizeof(CMD_TABLE);
 	myprintstr("Command\tUsage");
 	myprintstr(LINE_FEED);
 	myprintstr("--------------");
 	myprintstr(LINE_FEED);
 	for(iVal = 0; iVal < numCmds; iVal++)
 	{
 		myprintstr(cmdTable[iVal].cmdName);
 		custom_iowrite(TAB);
 		myprintstr(cmdTable[iVal].cmdUsage);
 		myprintstr(LINE_FEED);
 	}

 	return SUCCESS;
 }

 RET_CODE echo(PMSG *pmsg)
 {
 	myprintstr(pmsg->argv[1]);
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
 	myprintinteger(x);
 	myprintstr(" + ");
 	myprintinteger(y);
 	myprintstr(" = ");
 	myprintinteger(x+y);

 	return SUCCESS;
 }

#ifdef USE_HIST_STACK
 RET_CODE history(PMSG *pmsg)
 {
 	int iVal = 0;
 	/* Print the history stack */
 	for(iVal = 0; iVal < g_histstack.stackSize; iVal++)
 	{
 		myprintstr(g_histstack.stackBuffer[iVal]);
 		myprintstr(LINE_FEED);
 	}

 	return SUCCESS;
 }
#endif

 RET_CODE myexit(PMSG *pmsg)
 {
 	myprintstr("Exiting ...");
 	myprintstr(LINE_FEED);
 	return EXIT_CODE;
 }
