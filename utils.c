/**
 *  \file          utils.c
 *  
 *  \brief         Implements utility function and macros
 *                 for text processing and pretty printing.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */

/***** Standard includes goes here *****/
#include <stdio.h>
#ifdef USE_TERMIOS
#include <termios.h>
#endif
/***** Project includes goes here *****/
#include "common.h"
#include "utils.h"
#include "ioroutines.h"

/***** Macros goes here *****/

/***** Typedefs and Enums goes here *****/

/***** Global Variables *****/
char g_iVal = 0;
/***** Extern Variables *****/

/***** Function Forward References *****/

/***** Function Definitions *****/

/**
 *  \brief         clearscreen
 *                 Clears the terminal screen using ANSI VT100
 *                 character sequence.
 *
 *  \param[in]     void
 *
 *  \return        void
 */
void clearscreen(void)
{
    char clearScreenSeq[] = {0x1B, '[', '2', 'J', 0x00};
    char cursorHomeSeq[] = {0x1B, '[', 'H'};

    custom_printstring(clearScreenSeq);
    custom_printstring(cursorHomeSeq);

    return;
}

/* String Function Definition */
/**
 *  \brief      mystrlen
 *              Custom string length function
 *              
 *
 *  \param[in]  char *str
 *  
 *  \return     int
 */
int mystrlen(char *str)
{
    int len = 0;

    while(str[len] != EOS)
    {
       len++;
    }

    return len;
}

/**
 *  \brief      myatoi
 *              Custom function to convert ASCII
 *              string into integer.
 *
 *  \param[in]  char *str, int base, int *result
 *  
 *  \return     RET_CODE
 */
RET_CODE myatoi(char *str, int base, int *result)
{
    int iVal = mystrlen(str) - 1;
    *result = 0;

    if(base == HEX)
    {
        while(*str != EOS)
        {
            if(!ISVALIDHEX(*str)&& !ISVALIDDEC(*str))
            {
                return PARAM_ERR;
            }

            *result += (INTVAL(*str) * mypower(HEX, iVal));
            iVal--;
            str++;
        }
    }
    else if(base == DEC)
    {
        while(*str != EOS)
        {
            if(!ISVALIDDEC(*str))
            {
                return PARAM_ERR;
            }

            *result += (INTVAL(*str) * mypower(DEC, iVal));
            iVal--;
            str++;
        }
    }
    else
    {
        return BASE_ERR;
    }

    return SUCCESS;
}


/**
 *  \brief      mystrcmp
 *              Custom function to compare two ASCII
 *              Strings.
 *
 *  \param[in]  char *str1, char *str2
 *  
 *  \return     int
 */
int mystrcmp(char *str1, char *str2)
{
    while(*str1 == *str2 && *str1 != '\0')
    {
        str1++;
        str2++;
    }

    return (*str1 - *str2);
}

/**
 *  \brief      mystrstr
 *              Custom implementation to check if one 
 *              ASCII string starts with another.
 *
 *  \param[in]  char *str1, char *str2
 *  
 *  \return     int
 */
int mystrstr(char *str1, char *str2)
{
    while(*str1 == *str2)
    {
        str1++;
        str2++;
    }
 
    if(*str1 == EOS)
    {
        return TRUE;
    }

    return FALSE;
}


/**
 *  \brief      custom_printstring
 *              Custom function to print an ASCII
 *              string.
 *
 *  \param[in]  char *str
 *  
 *  \return     int
 */
int custom_printstring(char *str)
{
    while(*str != EOS)
    {
      custom_iowrite(*str++);
    }
    return 0;
}

#ifdef INCLUDE_PRINTINTEGER
/**
 *  \brief         myprintinteger
 *                 Prints a integer value.
 *  
 *  \param[in]     int number, unsigned char iStart
 *  
 *  \return        void 
 */
void myprintinteger(int number, unsigned char iStart)
{
    unsigned char digit;

    if(number == 0)
    {
        /* Check for very first Call */
        if(iStart)
        {   
            custom_iowrite('0');
        }
        return;
    }

    /* Get the last digit */
    digit = (number % 10) + ASCII_NUM;
    myprintinteger(number/10, FALSE);
    custom_iowrite(digit);
    return;
}
#endif


#ifdef  INCLUDE_PRINTHEX
/**
 *  \brief      myprinthex
 *              Routine to print in Hexadecimal.
 *              
 *
 *  \param[in]  int num, unsigned char precision
 *  
 *  \return     void
 */
void myprinthex(int num, unsigned char precision)
{
    unsigned char data;

    switch(precision)
    {
        case 8:
            data = (num & 0xFF000000) >> 24;
            custom_iowrite(g_HexASCIITable[(data >> 4) & 0xF]);
            custom_iowrite(g_HexASCIITable[(data) & 0xF]);
            data = (num & 0x00FF0000) >> 16;
            custom_iowrite(g_HexASCIITable[(data >> 4) & 0xF]);
            custom_iowrite(g_HexASCIITable[(data) & 0xF]);
        case 4:
            data = (num & 0x0000FF00) >> 8;
            custom_iowrite(g_HexASCIITable[(data >> 4) & 0xF]);
            custom_iowrite(g_HexASCIITable[(data) & 0xF]);
        case 2:
            data = (num & 0x000000FF);
            custom_iowrite(g_HexASCIITable[(data >> 4) & 0xF]);
            custom_iowrit(g_HexASCIITable[(data) & 0xF]);
            break;
        case 1:
            data = (num & 0x0000000F);
            custom_iowrite(g_HexASCIITable[(data) & 0xF]);
            break;
    }

    return;
}
#endif


/* Math functions */

/**
 *  \brief      mypower
 *              Custom function to implement
 *              power function
 *
 *  \param[in]  int x, int y
 *  
 *  \return     int
 */
int mypower(int x, int y)
{
    if(y == 0)
    {
        return 1;
    }

    return (x*mypower(x, y-1));
}

/* Termios Functions */
#ifdef USE_TERMIOS
/**
 *  \brief      disableEchoCanonMode
 *              Custom function to change the 
 *              default terminal behavior.
 *
 *  \param[in]  struct termios *oldSettings
 *  
 *  \return     void
 */
void disableEchoCanonMode(struct termios *oldSettings)
{
    struct termios newSettings; 
    /* Get current terminal settings */
    tcgetattr(STDIN_FILENO, oldSettings);
    /* Now make a mutable copy */
    newSettings = *oldSettings;
    /* Disable Cannonical and Echo Mode */
    newSettings.c_lflag &= ~(ICANON | ECHO);
    /* Apply the new settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    return;
}

/**
 *  \brief      restoreTerminalMode
 *              Custom Function to restore the default
 *              Terminal Behavior
 *
 *  \param[in]  struct termios *oldSettings
 *  
 *  \return     void
 */
void restoreTerminalMode(struct termios *oldSettings)
{
    /* Restore Default terminal Mode */
    tcsetattr(STDIN_FILENO, TCSANOW, oldSettings);
    return;
}
#endif