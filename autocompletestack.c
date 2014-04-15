/**
 *  \file          autocompletestack.c
 *  
 *  \brief         Implements routines to manipulate
 *                 autocomplete stack.
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
#include "cli.h"
#include "autocompletestack.h"
/***** Macros goes here *****/

/***** Typedefs and Enums goes here *****/

/***** Global Variables *****/

/***** Extern Variables *****/

/***** Function Forward References *****/

/***** Function Definitions *****/

/**
 *  \brief      initializeAutoCompl
 *              Initialize autocomplete Data structure
 *              and resets State Machine to IDLE.
 *
 *  \param[in]  AUTOCOMPL_STRUCT *autoComplStruct
 *  
 *  \return     AUTOCOMPL_RET_CODE
 */
AUTOCOMPL_RET_CODE initializeAutoCompl(AUTOCOMPL_STRUCT *autoComplStruct)
{
    /* Catch NULL Pointer Exception */
    if(autoComplStruct == NULL)
    {
      return AUTOCOMPL_NULL_ERR;
    }
    autoComplStruct->totalHits = 0;
    autoComplStruct->matchIndex = 0;
    autoComplStruct->autoComplState = AUTO_IDLE;

    return AUTOCOMPL_SUCCESS; 
}

/**
 *  \brief      buildMatchList
 *              Builds match list from the command table
 *              using the specfied search string.
 *
 *  \param[in]  AUTOCOMPL_STRUCT *autoComplStruct, char *searchStr, CMD_TABLE *cmdTable, int cmdTableSize
 *  
 *  \return     AUTOCOMPL_RET_CODE
 */
AUTOCOMPL_RET_CODE buildMatchList(AUTOCOMPL_STRUCT *autoComplStruct, char *searchStr, CMD_TABLE *cmdTable, int cmdTableSize)
{
    int index = 0;

    /* Reset internals */
    autoComplStruct->totalHits = 0;
    autoComplStruct->matchIndex = 0;

    for(index = 0; index < cmdTableSize; index++)
    {
        if(mystrstr(searchStr, cmdTable[index].cmdName) == TRUE)
        {
            /* Add hit to List */
            memcpy(autoComplStruct->matchList[autoComplStruct->totalHits++]
                , cmdTable[index].cmdName, mystrlen(cmdTable[index].cmdName) +1);
        }
    }

    if(autoComplStruct->totalHits == 0)
    {
        return AUTOCOMPL_NOHITS;
    }

    return AUTOCOMPL_SUCCESS;
}

/**
 *  \brief      findMatch
 *              Finds commands from the command table matching the 
 *              specified search string.
 *
 *  \param[in]  AUTOCOMPL_STRUCT *autoComplStruct, char *searchStr, CMD_TABLE *cmdTable, int cmdTableSize
 *  
 *  \return     AUTOCOMPL_RET_CODE
 */
AUTOCOMPL_RET_CODE findMatch(AUTOCOMPL_STRUCT *autoComplStruct, char *searchStr, CMD_TABLE *cmdTable, int cmdTableSize)
{
    AUTOCOMPL_RET_CODE retCode;

    /* Rebuild match list */
    if(autoComplStruct->autoComplState == AUTO_IDLE)
    {
        if((retCode = buildMatchList(autoComplStruct, searchStr, cmdTable, cmdTableSize)) != AUTOCOMPL_SUCCESS)
        {
            return retCode;
        }

        autoComplStruct->autoComplState = AUTO_ACTIVE;
        /* Add the seach string to the end of Match List */
        memcpy(autoComplStruct->matchList[autoComplStruct->totalHits++]
            , searchStr, mystrlen(searchStr) +1);

    }

    /* Copy Hit to search string */
    memcpy(searchStr, autoComplStruct->matchList[autoComplStruct->matchIndex]
            , mystrlen(autoComplStruct->matchList[autoComplStruct->matchIndex]) + 1);
    /* Increment to next index */
    autoComplStruct->matchIndex = (++autoComplStruct->matchIndex) % autoComplStruct->totalHits;     

    return AUTOCOMPL_SUCCESS;
}

