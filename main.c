/*
 *	main.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	Main Loop
 */
    
 /* Standard Library includes */
 #include <stdio.h>

 /* Project includes goes here */
 #include "historystack.h"
 #include "cli.h"
 
 extern PMSG g_pmsg;
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
  		printf("Error : Stack Initialization !!!\n");
  		return -1;
  	}

 	/* Enter CLI Loop */
    while(1)
    {
       if(CLILoop() == EXIT_CODE)
       {
         break;
       }
    }

  return 0;
}
