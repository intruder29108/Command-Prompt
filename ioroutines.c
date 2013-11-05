/*
 *	ioroutines.c
 * 
 *	Author		:	Antony Clince Alex
 *	Date		:	5 November 2013
 *	Description	:	Implementation of Platform specific	
 *					IO Routines	
 */

 /* Project includes goes here */
 #include "common.h"
 #include "ioroutines.h"

  /* Standard Library Includes goes here */
 #include <stdio.h>
 #ifdef USE_GETCH
 #include <conio.h>
 #endif


 /* Function definitions */
 char custom_ioread()
 {
 	char ch;
 	/* Change to platform specific routine */
 #ifdef USE_GETCH
 	ch = getch();
 #else
 	ch = getchar();
 #endif

 	return ch;
 }

 void custom_iowrite(char ch)
 {
 	/* Change to platform specific routine */
 	putchar(ch);
 	return;
 }