/**
 *  \file          cli.c
 *  
 *  \brief         Implements CLI state machine and 
 *                 command functions.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */

/***** Standard includes goes here *****/
#include <string.h>
/***** Project includes goes here *****/
#include "common.h"
#include "utils.h"
#include "ioroutines.h"
#ifdef USE_HIST_STACK
#include "historystack.h"
#endif
#include "cli.h"
#ifdef USE_AUTOCOMPL_STACK
#include "autocompletestack.h"
#endif
/***** Macros goes here *****/

/***** Typedefs and Enums goes here *****/

/***** Global Variables *****/
/* Command Table Definition */
CMD_TABLE cmdTable[] = {
    {"help",    help ,      0,  "help"},
    {"echo",    echo,       1,  "echo <string>"},
    {"add",     add ,       2,  "add  <num1> <num2>"},
#ifdef USE_HIST_STACK
    {"history", history,    0,  "history"},
#endif
    {"exit",    myexit,     0,  "exit"}
};
/* Global Message Structure */
PMSG g_pmsg;
/* Command Table Size */
int g_CmdTblSize = sizeof(cmdTable)/sizeof(CMD_TABLE);
/* Global array to store parse command line arguments */
int g_CmdArgs[MAX_NUM_PARAMS];
#ifdef USE_HIST_STACK
/* Global History Stack Structure */
HIST_STACK g_histstack;
#endif

#ifdef USE_AUTOCOMPL_STACK
/* Global Auto Complete Stack Structure */
AUTOCOMPL_STRUCT g_autocomplstack;
#endif
/***** Extern Variables *****/

/***** Function Forward References *****/
RET_CODE convertArgumentsIntoInteger(int nArgs);

/***** Function Definitions *****/

/**
 *  \brief      CLILoop
 *              Main CLI State Machine loops.
 *              Basically runs the CLI state machine.
 *
 *  \param[in]  void
 *  
 *  \return     RET_CODE
 */
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

/**
 *  \brief      InitState
 *              Initialises CLI state machine
 *              resets all data structures.
 *
 *  \param[in]  void *msg
 *  
 *  \return     funcPtr
 */
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

    custom_printstring(LINE_FEED);
    custom_printstring("CLI>>");

    return (funcPtr)InputState;
}


/**
 *  \brief      InputState
 *              CLI state takes input from user and store in
 *              internal buffer to be processed in later on states.
 *
 *  \param[in]  void *msg
 *  
 *  \return     funcPtr
 */
funcPtr InputState(void *msg)
{
    PMSG *pmsg = (PMSG *)msg;
    int ch;
    static int len = 0;         /* Keep track of command buffer length */
    int iVal, prevLen;
    int cmdBufLen = 0;          /* Cache string length */

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
                    /* Cache copy of command buffer length before history stack */
                    prevLen = mystrlen(pmsg->cmdBuff);
                    /* Pop the TOS to command buffer */
                    if(stack_pop_up(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
                    {
                        return (funcPtr)InputState;
                    }
                    /* Set up stack pop state */
                    g_histstack.stack_pop_state = STACK_UP;

                    /* Move cursor to end  */
                    for(iVal = len; iVal < prevLen; iVal++)
                    {
                        custom_iowrite(SPACE);
                    }

                    /* Clear the current line */
                    while(prevLen--)
                    {
                        custom_iowrite(BACKSPACE);
                        custom_iowrite(SPACE);
                        custom_iowrite(BACKSPACE);
                    }
                    /* Update length field */
                    len = mystrlen(pmsg->cmdBuff);
                    custom_printstring(pmsg->cmdBuff);
                }
                else
                {
                    custom_iowrite(BELL);
                }
#endif
            }
            else if(ch == DOWN)
            {
#ifdef USE_HIST_STACK
                /* Check if History Stack is empty */
                if(IS_STACK_EMPTY(g_histstack) == FALSE)
                {
                    prevLen = mystrlen(pmsg->cmdBuff);
                    /* Clean the command buffer */
                    if(stack_pop_down(&g_histstack, pmsg->cmdBuff) != STACK_SUCCESS)
                    {
                        return (funcPtr)InputState;
                    }
                    /* Set up stack pop state */
                    g_histstack.stack_pop_state = STACK_DOWN;
                    for(iVal = len; iVal < prevLen; iVal++)
                    {
                        custom_iowrite(SPACE);
                    }
                    /* Clear the current line */
                    while(prevLen--)
                    {
                        custom_iowrite(BACKSPACE);
                        custom_iowrite(SPACE);
                        custom_iowrite(BACKSPACE);
                    }
                    /* Update length field */
                    len = mystrlen(pmsg->cmdBuff);
                    custom_printstring(pmsg->cmdBuff);
                }
                else
                {
                    custom_iowrite(BELL);
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
#ifdef  USE_AUTOCOMPL_STACK
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
            custom_printstring(pmsg->cmdBuff);
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
                custom_printstring("Error : Stack Push !!!");
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
        if(len < MAX_CMD_BUFFER_SIZE - 1)
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

/**
 *  \brief      ParseState
 *              Parse through the data in the internal
 *              buffer and parameterise it into command and arguments.
 *
 *  \param[in]  void *msg
 *  
 *  \return     funcPtr
 */
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

/**
 *  \brief      RespondState
 *              Finds match for the command and 
 *              calls the respective handler function.
 *
 *  \param[in]  void *msg
 *  
 *  \return     funcPtr
 */
funcPtr RespondState(void *msg)
{
    PMSG *pmsg = (PMSG *)msg;
    int iVal = 0;
    int numCmds;
    int cmdFound = FALSE;


    /* Return incase of no input */
    if(pmsg->argc == 0)
    {
        return (funcPtr)InitState;
    }
    /* Move the next line */
    custom_printstring(LINE_FEED);
    numCmds = sizeof(cmdTable)/sizeof(CMD_TABLE);
    for(iVal = 0; iVal < numCmds; iVal++)
    {
        if(mystrcmp(pmsg->argv[0], cmdTable[iVal].cmdName) == 0)
        {
            cmdFound = TRUE;
            if((pmsg->argc - 1) != cmdTable[iVal].argNum)
            {
                custom_printstring("Incorrect Usage !!!");
                custom_printstring(LINE_FEED);
                custom_printstring(cmdTable[iVal].cmdUsage);
                break;
            }

            switch(cmdTable[iVal].cmdFuncPtr(pmsg))
            {
                case PARAM_ERR:
                    MSG_PRINT(MSG_PREFIX, "Parameter Error");
                    break;
                case PARAM1_ERR:
                    MSG_PRINT(MSG_PREFIX, "Parameter #1 Error");
                    break;
                case PARAM2_ERR:
                    MSG_PRINT(MSG_PREFIX, "Parameter #2 Error");
                    break;
                case PARAM3_ERR:
                    MSG_PRINT(MSG_PREFIX, "Parameter #3 Error");
                    break;
                case PARAM4_ERR:
                    MSG_PRINT(MSG_PREFIX, "Parameter #4 Error");
                    break;
                case CMD_FAIL:
                    MSG_PRINT(MSG_PREFIX, "Command Failed");
                    break;
                case EXIT_CODE:
                    return (funcPtr)NULL;
                    break;
                default:
                    break;
            }
        }
    }

    if(cmdFound == FALSE)
    {
        custom_printstring("Command not found !!!, Try \"help\" for list");
    }


    /* Loop to start of SM */
    return (funcPtr)InitState;
}

/* Command Definitions */

/**
 *  \brief      help
 *              Help Function
 *              
 *
 *  \param[in]  PMSG *pmsg
 *  
 *  \return     RET_CODE
 */
RET_CODE help(PMSG *pmsg)
{
    int numCmds, iVal;

    numCmds = g_CmdTblSize;
    custom_printstring(PRINT_HEADER);
    custom_printstring("Command Name\n\r");
    custom_printstring(PRINT_HEADER);
    for(iVal = 0; iVal < numCmds; iVal++)
    {
        custom_printstring(cmdTable[iVal].cmdName);
        custom_printstring("\n\r");
    }
    return SUCCESS;
}

/**
 *  \brief      echo
 *              Implements ECHO command.
 *              
 *
 *  \param[in]  PMSG *pmsg
 *  
 *  \return     RET_CODE
 */
RET_CODE echo(PMSG *pmsg)
{
    custom_printstring(pmsg->argv[1]);
    return SUCCESS;
}

/**
 *  \brief      add
 *              Implements add routine.
 *              
 *
 *  \param[in]  PMSG *pmsg
 *  
 *  \return     RET_CODE
 */
RET_CODE add(PMSG *pmsg)
{
    if(convertArgumentsIntoInteger(pmsg->argc) != SUCCESS)
    {
        return PARAM_ERR;
    }

    myprintinteger(g_CmdArgs[1], TRUE);
    custom_printstring(" + ");
    myprintinteger(g_CmdArgs[2], TRUE);
    custom_printstring(" = ");
    myprintinteger(g_CmdArgs[1] + g_CmdArgs[2], TRUE);

    return SUCCESS;
}

/**
 *  \brief      history
 *              Implements history command to
 *              display contents of history stack.
 *
 *  \param[in]  PMSG *pmsg
 *  
 *  \return     RET_CODE
 */
RET_CODE history(PMSG *pmsg)
{
    int iVal = 0;
    /* Print the history stack */
    for(iVal = 0; iVal < g_histstack.stackSize; iVal++)
    {
        custom_printstring(g_histstack.stackBuffer[iVal]);
        custom_printstring(LINE_FEED);
    }

    return SUCCESS;
}

/**
 *  \brief      myexit
 *              Implements exit routine to exit 
 *              from CLI
 *
 *  \param[in]  PMSG *pmsg
 *  
 *  \return     RET_CODE
 */
RET_CODE myexit(PMSG *pmsg)
{
    MSG_PRINT(MSG_PREFIX, "Exiting");
    return EXIT_CODE;
}

/**
 *  \brief      convertArgumentsIntoInteger
 *              Converts arguments into integer
 *              format.
 *
 *  \param[in]  int nArgs
 *  
 *  \return     RET_CODE
 */
RET_CODE convertArgumentsIntoInteger(int nArgs)
{
    int iVal;

    for(iVal = 1; iVal < nArgs; iVal++)
    {
        if(ISHEX(g_pmsg.argv[iVal]))
        {
            if(myatoi(&g_pmsg.argv[iVal][2], HEX, (int *)&g_CmdArgs[iVal]) == PARAM_ERR)
            {
                return PARAM_ERR;
            }
        }
        else
        {
            if(myatoi(&g_pmsg.argv[iVal][0], DEC, (int *)&g_CmdArgs[iVal]) == PARAM_ERR)
            {
                return PARAM_ERR;
            }
        }
    }

    return SUCCESS;
}