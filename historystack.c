/**
 *  \file          historystack.c
 *  
 *  \brief         Implements history stack feature for
 *                 CLI Console.
 *
 *  Author         Antony Clince Alex
 *  Date           15 October 2013
 *  Copyright(c) 2013 GPL. All Rights Reserved.
 */

/***** Standard includes goes here *****/
 #include <string.h>

/***** Project includes goes here *****/
#include "common.h"
#include "utils.h"
#include "historystack.h"
/***** Macros goes here *****/

/***** Typedefs and Enums goes here *****/

/***** Global Variables *****/

/***** Extern Variables *****/

/***** Function Forward References *****/

/***** Function Definitions *****/

/**
 *  \brief      stack_initialize
 *              Initializes CLI history stack memory
 *              and resets its size to zero.
 *
 *  \param[in]  HIST_STACK *histStack
 *  
 *  \return     STACK_RET_CODE
 */
STACK_RET_CODE stack_initialize(HIST_STACK *histStack)
{
    /* Catch Null pointer Error */
    if(histStack == NULL)
    {
        return STACK_NULL_ERR;
    }

    histStack->stackSize = 0;
    memset(histStack->stackBuffer, 0, MAX_STACK_BUFF_SIZE);

    return STACK_SUCCESS;
}

/**
 *  \brief      stack_push
 *              Pushs and entry into history
 *              stack.
 *
 *  \param[in]  HIST_STACK *histStack, char *entry
 *  
 *  \return     STACK_RET_CODE
 */
STACK_RET_CODE stack_push(HIST_STACK *histStack, char *entry)
{
    int index = 0;

    /* Catch NULL Pointer Error */
    if(histStack == NULL)
    {
        return STACK_NULL_ERR;
    }
#ifdef CHK_STACK_DUPENTRY
    /* Check if element exists in stack */
    if(checkIfElementExists(histStack, entry) == STACK_SUCCESS)
    {
        /* Skip this entry */
        return STACK_SUCCESS;
    }
#endif
    /* Skip entry if length is zero */
    if(mystrlen(entry) == 0)
    {
        /* Skip this entry */
        return STACK_SUCCESS;
    }

    /* Push down */
    for(index = MAX_STACK_SIZE - 1; index > 0; index--)
    {
        memcpy(histStack->stackBuffer[index],
             histStack->stackBuffer[index - 1], mystrlen(histStack->stackBuffer[index - 1]) + 1);
    }

    /* Increment Stack size */
    if(histStack->stackSize < MAX_STACK_SIZE)
    {
        histStack->stackSize++;
    }
    /* Copy to head */
    memcpy(histStack->stackBuffer[STACK_HEAD], entry, mystrlen(entry) + 1);

    return STACK_SUCCESS;
}

/**
 *  \brief      stack_pop_up
 *              Pops and entry from history
 *              stack buffer from bottom up.
 *
 *  \param[in]  HIST_STACK *histStack, char *entry
 *  
 *  \return     STACK_RET_CODE
 */
STACK_RET_CODE stack_pop_up(HIST_STACK *histStack, char *entry)
{
    int prevaccessIndex = histStack->accessIndex;
    if(histStack->stackSize == 0)
    {
        return STACK_UNDERFLOW;
    }

    if(histStack->stack_pop_state == STACK_DOWN)
    {
        histStack->accessIndex++;
    }

    if(histStack->accessIndex < histStack->stackSize)
    {
        /* Copy head */
        memcpy(entry, histStack->stackBuffer[histStack->accessIndex]
                        , mystrlen(histStack->stackBuffer[histStack->accessIndex]) + 1);
        histStack->accessIndex++;
        return STACK_SUCCESS;
    }
    histStack->accessIndex = prevaccessIndex;
    return STACK_UNDERFLOW;
}

/**
 *  \brief      stack_pop_down
 *              Pops entry from history stack
 *              from bottom down.
 *
 *  \param[in]  HIST_STACK *histStack, char *entry
 *  
 *  \return     STACK_RET_CODE
 */
STACK_RET_CODE stack_pop_down(HIST_STACK *histStack, char *entry)
{
    int prevaccessIndex = histStack->accessIndex;

    if(histStack->stackSize == 0)
    {
        return STACK_UNDERFLOW;
    }

    if(histStack->stack_pop_state == STACK_UP)
    {
        histStack->accessIndex--;
    }

    if(histStack->accessIndex > 0)
    {
        histStack->accessIndex--;
        /* Copy head */
        memcpy(entry, histStack->stackBuffer[histStack->accessIndex]
                , mystrlen(histStack->stackBuffer[histStack->accessIndex]) + 1);

        return STACK_SUCCESS;

    }

    histStack->accessIndex = prevaccessIndex;
    return STACK_UNDERFLOW;
}

/**
 *  \brief      checkIfElementExists
 *              Checks if an entry is already present in
 *              stack
 *
 *  \param[in]  HIST_STACK *histStack, char *entry
 *  
 *  \return     STACK_RET_CODE
 */
STACK_RET_CODE checkIfElementExists(HIST_STACK *histStack, char *entry)
{
    int iVal;

    for(iVal = 0; iVal < histStack->stackSize; iVal++)
    {
        /* Compare each element */
        if(mystrcmp(histStack->stackBuffer[iVal], entry) == 0)
        {
            return STACK_SUCCESS;
        }
    }

    return STACK_FAILURE;
}

