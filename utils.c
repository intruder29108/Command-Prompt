/*
 *	utils.c
 *
 *	Author		:	Antony Clince Alex
 *	Date		:	15 October 2013
 *  Description :	String and Math functions
 */

/* Standard Includes */


/* Project Includes */
 #include "utils.h"


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
  *  Name    :  myprintstr
  *  Description  :  Custom function to print a string
  *          
  *
  *  In      :   char *str
  *  Return    :  int 
  */
  int  myprintstr(char *str)
  {
    while(*str != EOS)
    {
      putchar(*str++);
    }
    return 0;
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

