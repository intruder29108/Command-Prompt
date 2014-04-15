/**
 *  \file          common.h
 *  
 *  \brief         Contains configuration macros used to
 *                 configure CLI
 *
 *  Author         Antony Clince Alex
 *  Date           5 November 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */
#ifndef COMMON_H
#define COMMON_H

/***** Standard includes goes here *****/

/***** Project includes goes here *****/

/***** Macros goes here *****/
/* Enable/Disable UNIX Libraries */
#define USE_TERMIOS
/* Enable/Disable getch */
#undef  USE_GETCH
/* Enable/Disable Autocomplete Feature */
#define USE_AUTOCOMPL_STACK
/* Enable/Disable History Stack Feature */
#define USE_HIST_STACK
/* Enable Print Integer Routine */
#define INCLUDE_PRINTINTEGER
/* Enable Print Hex Routine */
#undef  INCLUDE_PRINTHEX

/* Platform specific macros */
#ifdef USE_TERMIOS
#undef USE_GETCH
#endif

#ifdef USE_GETCH
#undef USE_TERMIOS
#endif
/***** Typedefs and Enums goes here *****/

/***** Global Variables *****/

/***** Extern Variables *****/

/***** Function Declarations *****/

#endif

