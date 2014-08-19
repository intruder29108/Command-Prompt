/**
 *  \file          main.c
 *  
 *  \brief         Main Program Loop.
 *                 
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */

/***** Standard includes goes here *****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
 
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
#ifdef USE_TERMIOS
/* Global Variables to store terminal settings */
static struct termios g_OldSettings;
#endif
/* Reference to global message structure */
extern PMSG g_pmsg;
/* Reference to history stack strucuture */
extern HIST_STACK g_histstack;

/***** Extern Variables *****/

/***** Function Forward References *****/

/***** Function Definitions *****/
/**
 *  \brief      main
 *              Main Program Loop
 *              
 *
 *  \param[in]  int argc, char *argv[]
 *  
 *  \return     int
 */
int main(int argc, char *argv[])
{

#ifdef USE_HIST_STACK
    /* Initialize history stack */
    if(stack_initialize(&g_histstack) != STACK_SUCCESS)
    {
      return -1;
    }
#endif

#ifdef USE_TERMIOS
    /* Change Terminal Settings */
    disableEchoCanonMode(&g_OldSettings);
#endif
    /* Welcome notes */
    custom_printstring(" _______________________________________________________ \n\r");
    custom_printstring("/                                                       \\\n\r");
    custom_printstring("|               Welcome to CLI                          |\n\r");
    custom_printstring("\\_________              _______________________________/ \n\r");
    custom_printstring("          \\_         __/    ___---------__               \n\r");
    custom_printstring("            \\      _/      /              \\_             \n\r");
    custom_printstring("             \\    /       /                 \\            \n\r");
    custom_printstring("              |  /       | _    _ \\          \\           \n\r");
    custom_printstring("              | |       / / \\  / \\ |          \\          \n\r");
    custom_printstring("              | |       ||   ||   ||           |         \n\r");
    custom_printstring("              | |       | \\_//\\\\_/ |           |         \n\r");
    custom_printstring("              | |         |  ||     | _  / /   |         \n\r");
    custom_printstring("               \\ \\        |_________|| \\/ /   /          \n\r");
    custom_printstring("                \\ \\_       |_|_|_|_|/|  _/___/           \n\r");
    custom_printstring("                 \\__>       _ _/_ _ /  |                 \n\r");
    custom_printstring("                          .|_|_|_|_|   |                 \n\r");
    custom_printstring("                          |           /                  \n\r");
    custom_printstring("                          |__________/                   \n\r");
    custom_printstring("Welcome to CLI");
    custom_printstring(LINE_FEED);
    custom_printstring("Type \"help\" for list of commands");
    custom_printstring(LINE_FEED);
    custom_printstring(LINE_FEED);
#ifdef USE_AUTOCOMPL_STACK
    custom_printstring("Hit    <TAB>     for autocomplete");
    custom_printstring(LINE_FEED);
#endif
#ifdef USE_HIST_STACK
    custom_printstring("Hit <UP>/<DOWN>  for command history");
    custom_printstring(LINE_FEED);
#endif
    custom_printstring("Hit <LEFT/RIGHT> for line editing");

    /* Enter CLI Loop */  
    while(1)
    {
       if(CLILoop() == EXIT_CODE)
       {

#ifdef USE_TERMIOS
        /* Restore Previous Termianl Settings */
         restoreTerminalMode(&g_OldSettings);
#endif
         break;
       }
    }

  return 0;
}

