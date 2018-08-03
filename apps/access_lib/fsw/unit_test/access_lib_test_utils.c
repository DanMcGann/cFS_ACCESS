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
/*******************************************************************************
*   File: access_lib_test_utils.c
*
*   Purpose: Proveds detup and teardown functions for access_lib test enviro
*
*   AUTHOR: Daniel McGann 3/22/2018
*
*******************************************************************************/

/*
 * Includes
 */

#include "access_lib_test_utils.h"
#include "access_lib.h"
/*
 * Function Definitions
 */

void ACCESS_LIB_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
   
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
} /* end DS_Test_Setup */

void ACCESS_LIB_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end DS_Test_TearDown */


/************************/
/*  End of File Comment */

