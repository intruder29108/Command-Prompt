/*
 *	utils.h
 *
 *	Author		: Antony Clince Alex
 *	Date		: 15 October 2013
 *	Description :
 *					String and math functions
 */

 #ifndef UTILS_H
 #define UTILS_H

 /* Project Includes goes here */
 #include "common.h"

 /* Standard Includes Goes here */
 #ifdef USE_TERMIOS
 #include <termios.h>
 #include <unistd.h>
 #endif

 /* Macros goes here */
 #define 	HEX			16
 #define 	DEC 		10
 #define 	EOS			'\0'
 #define 	TRUE		1
 #define 	FALSE		0
 #define 	ESC_SEQ		0x1b
 #define 	ESC_SEQ1	0x5b
 #define 	UP			'A'
 #define 	DOWN		'B'
 #define 	LEFT		'D'
 #define 	RIGHT 		'C'
 #define 	ASCII_NUM	0x30
 #define 	DELETE		127
 #define	BACKSPACE	'\b'
 #define 	CARR_RET	'\r'
 #define 	NEW_LINE	'\n'
 #define 	TAB			'\t'
 #define 	SPACE		' '
 #define 	LINE_FEED	"\r\n"
 #define 	ISHEX(str)	((((str[0]) == '0') && (((str[1]) == 'x') || ((str[1]) == 'X'))) ? TRUE : FALSE)
 #define	ISALPHA(ch) (((((ch) >= 'A') && ((ch) <= 'Z')) || (((ch) >= 'a') && ((ch) <= 'z'))) ? TRUE : FALSE)
 #define 	ISNUM(ch)	((((ch) >= '0') && ((ch) <= '9')) ? TRUE : FALSE)
 #define 	ISUPPER(ch)	((((ch) >= 'A') && ((ch) <= 'Z')) ? TRUE : FALSE)
 #define 	TOUPPER(ch)	((ISUPPER(ch) || ISNUM(ch)) ? (ch) : (ch - ('a' - 'A')))
 #define 	ISVALIDDEC(ch)	((((ch) >= '0') && ((ch) <= '9')) ? TRUE : FALSE)
 #define 	ISVALIDHEX(ch)	((((TOUPPER(ch)) >= 'A') && ((TOUPPER(ch)) <= 'F')) ? TRUE : FALSE)
 #define	INTVAL(ch)	((ISALPHA(ch)) ? ((TOUPPER(ch)) - 'A' + DEC) : (ch - '0'))

 /* Typedefs goes here */
 typedef enum {SUCCESS, NULL_ERR, BASE_ERR, PARSE_ERR, PARAM_ERR, EXIT_CODE} RET_CODE;

 /* Forward declarations */

 /* String functions */
 int mystrlen(char *);
 int mystrcmp(char *, char *);
 int mystrstr(char *, char *);
 int myprintstr(char *);
 void myprintinteger(int);
 RET_CODE myatoi(char *, int, int *);

 /* Math Functions */
 int mypower(int, int);

#ifdef USE_TERMIOS
 /* Termios Functions */
 void disableEchoCanonMode(struct termios *);
 void restoreTerminalMode(struct termios *);
#endif

 #endif