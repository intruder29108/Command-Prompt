/**
 *  \file          ioroutines.c
 *  
 *  \brief         Implements Hardware specific IO Routine
 *                 to read and write from UART.
 *
 *  Author         Antony Clince Alex
 *  Date           5 November 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */

/***** Standard Library Includes goes here *****/
#include <stdio.h>
#ifdef USE_GETCH
#include <conio.h>
#endif

/***** Project includes goes here *****/
#include "common.h"
#include "ioroutines.h"


/***** Function definitions *****/

/**
 *  \brief      custom_ioread
 *              Reads a single byte from
 *              UART.
 *
 *  \param[in]  void
 *  
 *  \return     char
 */
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

/**
 *  \brief      custom_iowrite
 *              Writes a single byte
 *              to UART Console.
 *
 *  \param[in]  char ch
 *  
 *  \return     void
 */
void custom_iowrite(char ch)
{
    /* Change to platform specific routine */
    putchar(ch);
    return;
}