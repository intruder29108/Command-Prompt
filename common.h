/*
 *	common.h
 * 
 *	Author		:	Antony Clince Alex
 *	Date		:	5 November 2013
 *	Description	:	Includes Configuration macros for CLI
 *					
 */

 #ifndef	COMMON_H
 #define	COMMON_H

 /* Enable/Disable UNIX Libraries */
 #define USE_TERMIOS
 /* Enable/Disable getch */
 #undef  USE_GETCH
 /* Enable/Disable Autocomplete Feature */
 #undef	 USE_AUTOCOMPL_STACK
 /* Enable/Disable History Stack Feature */
 #undef	 USE_HIST_STACK
 
/* Platform specific macros */
#ifdef USE_TERMIOS
 #undef USE_GETCH
#endif

#ifdef USE_GETCH
 #undef USE_TERMIOS
#endif

#endif