/*
 *	main.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	Main Loop
 */
    
 /* Project includes goes here */
 #include "common.h"
 #include "historystack.h"
 #include "cli.h"
 
 /* Standard Library includes */
 #ifdef USE_TERMIOS
 #include <termios.h>
 #endif

 #ifdef USE_TERMIOS
 /* Global Variables to store terminal settings */
 static struct termios g_OldSettings;
 #endif

 /* Reference to global message structure */
 extern PMSG g_pmsg;
 /* Reference to history stack strucuture */
 extern HIST_STACK g_histstack;

 /*
  *	Name		:	main
  *	Description	:	Main Program Loop
  *					
  *
  *	In			: 	int argc, char *argv[]
  *	Return		:	int
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
    myprintstr("Welcome to CLI");
    myprintstr(LINE_FEED);
    myprintstr("Type \"help\" for list of commands");
    myprintstr(LINE_FEED);
    myprintstr(LINE_FEED);
#ifdef USE_AUTOCOMPL_STACK
    myprintstr("Hit    <TAB>     for autocomplete");
    myprintstr(LINE_FEED);
#endif
#ifdef USE_HIST_STACK
    myprintstr("Hit <UP>/<DOWN>  for command history");
    myprintstr(LINE_FEED);
#endif
    myprintstr("Hit <LEFT/RIGHT> for line editing");

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
