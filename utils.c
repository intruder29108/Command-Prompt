/*
 *	utils.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	String and Math functions
 */


/* Project Includes */
 #include "utils.h"
 #include "ioroutines.h"


/* String Function Definition */


 /*
  *	Name		:	mystrlen
  *	Description	:	Custom string length function
  *					
  *
  *	In			: 	char *str
  *	Return		:	int
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

 /*
  *	Name		:	myatoi
  *	Description	:	Custom fuction to convert ASCII
  *					string into integer
  *
  *	In			: 	char *str, int base, int *result
  *	Return		:	RET_CODE
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


 /*
  *	Name		:	mystrcmp
  *	Description	:	Custom function to compare two ASCII
  *					Strings
  *
  *	In			: 	char *str1, char *str2
  *	Return		:	int 
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


/*
 *	Name		:	mystrstr
 *	Description	:	Custom implementation to check if one ASCII string
 *					starts with another.
 *
 *	In			: 	char *str1, char *str2
 *	Return		:	int
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




 /*
  *  Name       :  myprintstr
  *  Description:  Custom function to print a string
  *          
  *
  *  In         :  char *str
  *  Return     :  int 
  */
  int  myprintstr(char *str)
  {
    while(*str != EOS)
    {
      custom_iowrite(*str++);
    }
    return 0;
  }

 /*
  *  Name       :  myprintinteger
  *  Description:  Custom function to print integers
  *          
  *
  *  In         :  int number
  *  Return     :  void
  */
  void myprintinteger(int number)
  {
    unsigned char digit;

    if(number == 0)
    {
        return;
    }

    /* Get the last digit */
    digit = (number % 10) + ASCII_NUM;
    myprintinteger(number/10);
    custom_iowrite(digit);
    return;
  }


/* Math functions */

/*
 *	Name		:	mypower
 *	Description	:	Custom function to implement power 
 *					Function
 *
 *	In			: 	int x, int y
 *	Return		:	int
 */
int mypower(int x, int y)
{
	if(y == 0)
	{
		return 1;
	}

	return (x*mypower(x, y-1));
}

#ifdef USE_TERMIOS
/* Termios Functions */

/*
 *  Name        :   disableEchoCanonMode
 *  Description :   Custom Function to change default 
 *                  Terminal Behavior
 *
 *  In          :   struct termios *oldSettings
 *  Out         :   struct termios *oldSettings
 *  Return      :   void 
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

/*
 *  Name        :   restoreTerminalMode
 *  Description :   Custom Function to restore default
 *                  Terminal Behavior
 *
 *  In          :   struct termios *oldSettings
 *  Return      :   void
 */
 void restoreTerminalMode(struct termios *oldSettings)
 {
    /* Restore Default terminal Mode */
    tcsetattr(STDIN_FILENO, TCSANOW, oldSettings);
    return;
 }
 #endif