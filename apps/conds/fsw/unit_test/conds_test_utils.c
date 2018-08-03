/*********************************************************************************
*DISTRIBUTION STATEMENT A. Approved for public release. Distribution is unlimited.
*
*This material is based upon work supported under Air Force Contract No. FA8721-05-C-0002
*and/or FA8702-15-D-0001. Any opinions, findings, conclusions or recommendations
*expressed in this material are those of the author(s) and do not necessarily reflect the
*views of the U.S. Air Force.
*
*© 2018 Massachusetts Institute of Technology.
*
*The software/firmware is provided to you on an As-Is basis
*
*Delivered to the U.S. Government with Unlimited Rights, as defined in DFARS Part
*252.227-7013 or 7014 (Feb 2014). Notwithstanding any copyright notice, U.S. Government
*rights in this work are defined by DFARS 252.227-7013 or DFARS 252.227-7014 as detailed
*above. Use of this work other than as specifically authorized by the U.S. Government may
*violate any copyrights that exist in this work.
*********************************************************************************/ 
/******************************************************************************
* File:L conds_test_utils.c
*
*	Purpose: This files provides the implementation of testing utility functions
*	for the conds application.
*
*  Created on: Mar 30, 2018
*      Author: Daniel McGann
******************************************************************************/

/*
 * Includes
 */
#include "conds_test_utils.h"
#include "conds_app.h"


/*
 * Function Definitions
 */

void CONDS_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&g_CONDS_AppData, 0, sizeof(CONDS_AppData_t));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
} /* end CONDS_Test_Setup */

void CONDS_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end CONDS_Test_TearDown */

/************************/
/*  End of File Comment */
/************************/
