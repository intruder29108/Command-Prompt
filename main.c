/*
 *	main.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	Main Loop
 */
    
 /* Standard Library includes */
 #include <termios.h>

 /* Project includes goes here */
 #include "historystack.h"
 #include "cli.h"
 

 /* Global Variables to store terminal settings */
 static struct termios g_OldSettings;
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

  	/* Initialize history stack */
  	if(stack_initialize(&g_histstack) != STACK_SUCCESS)
  	{
  		return -1;
  	}

    /* Change Termianl Settings */
    disableEchoCanonMode(&g_OldSettings);
    /* Welcome notes */
    myprintstr("Welcome to CLI");
    myprintstr(LINE_FEED);
    myprintstr("Type \"Help\" for list of commands");
    myprintstr(LINE_FEED);
    myprintstr(LINE_FEED);
    myprintstr("Hit    <TAB>     for autocomplete");
    myprintstr(LINE_FEED);
    myprintstr("Hit <UP>/<DOWN>  for command history");
    myprintstr(LINE_FEED);
    myprintstr("Hit <LEFT/RIGHT> for line editing");

  	/* Enter CLI Loop */  
    while(1)
    {
       if(CLILoop() == EXIT_CODE)
       {
        /* Restore Previous Termianl Settings */
         restoreTerminalMode(&g_OldSettings);
         break;
       }
    }

  return 0;
}
