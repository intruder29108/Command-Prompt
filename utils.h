/**
 *  \file          utils.h
 *  
 *  \brief         Implements string and other utility
 *                 functions and macros.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */

#ifndef UTILS_H
#define UTILS_H

/***** Standard Includes Goes here *****/
#ifdef USE_TERMIOS
#include <termios.h>
#include <unistd.h>
#endif

/***** Project Includes goes here *****/
#include "common.h"

/***** Macros goes here *****/
#define     HEX         16
#define     DEC         10
#define     EOS         '\0'
#define     TRUE        1
#define     FALSE       0
#define     ESC_SEQ     0x1b
#define     ESC_SEQ1    0x5b
#define     UP          'A'
#define     DOWN        'B'
#define     LEFT        'D'
#define     RIGHT       'C'
#define     BELL        0x07
#define     ASCII_NUM   0x30
#define     DELETE      127
#define     BACKSPACE   '\b'
#define     CARR_RET    '\r'
#define     NEW_LINE    '\n'
#define     TAB         '\t'
#define     SPACE       ' '
#define     LINE_FEED   "\r\n"
#define     ISHEX(str)  \
            ((((str[0]) == '0') && (((str[1]) == 'x') || ((str[1]) == 'X'))) ? TRUE : FALSE)
#define     ISALPHA(ch) \
            (((((ch) >= 'A') && ((ch) <= 'Z')) || (((ch) >= 'a') && ((ch) <= 'z'))) ? TRUE : FALSE)
#define     ISNUM(ch)   \
            ((((ch) >= '0') && ((ch) <= '9')) ? TRUE : FALSE)
#define     ISUPPER(ch) \
            ((((ch) >= 'A') && ((ch) <= 'Z')) ? TRUE : FALSE)
#define     TOUPPER(ch) \
            ((ISUPPER(ch) || ISNUM(ch)) ? (ch) : (ch - ('a' - 'A')))
#define     ISVALIDDEC(ch) \
            ((((ch) >= '0') && ((ch) <= '9')) ? TRUE : FALSE)
#define     ISVALIDHEX(ch) \
            ((((TOUPPER(ch)) >= 'A') && ((TOUPPER(ch)) <= 'F')) ? TRUE : FALSE)
#define     INTVAL(ch) \
            ((ISALPHA(ch)) ? ((TOUPPER(ch)) - 'A' + DEC) : (ch - '0'))
#define     PRINT_HEADER    "----------------------------------------------------------------\n\r"
#define     MSG_PREFIX      "******"
#define     NONE            ""
#define     PADDING         40
#define     PRINT_NEWLINE \
            custom_printstring("\n\r")
#define     MSG_PRINT(prefix, msg) \
    custom_printstring(prefix" "msg" "prefix"\n\r")
#define     FMT_PRINT(head, body)                                   \
{                                                                   \
    custom_printstring(head);                                       \
    for(g_iVal = mystrlen(head); g_iVal < PADDING; g_iVal++)        \
    {                                                               \
        custom_iowrite_polled(SPACE);                               \
    }                                                               \
    custom_printstring(": "body);                                   \
}

/***** Typedefs goes here *****/
typedef enum {SUCCESS, CMD_FAIL, CMD_FAILQUIET,  NULL_ERR, BASE_ERR, PARSE_ERR, PARAM_ERR, PARAM1_ERR, PARAM2_ERR, PARAM3_ERR, PARAM4_ERR, EXIT_CODE} RET_CODE;

/***** Forward declarations *****/
/* Miscellaneous functions */
void clearscreen();
/* String functions */
int mystrlen(char *);
int mystrcmp(char *, char *);
int mystrstr(char *, char *);
int custom_printstring(char *);
#ifdef INCLUDE_PRINTINTEGER
void myprintinteger(int , unsigned char);
#endif
#ifdef  INCLUDE_PRINTHEX
void myprinthex(int , unsigned char)
#endif
RET_CODE myatoi(char *, int, int *);
/* Math Functions */
int mypower(int, int);
#ifdef USE_TERMIOS
void disableEchoCanonMode(struct termios *);
void restoreTerminalMode(struct termios *);
#endif

#endif