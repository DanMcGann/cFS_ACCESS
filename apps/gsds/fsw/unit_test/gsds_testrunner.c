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
 /*************************************************************************
 ** File: gsds_testrunner.c
 **
 ** Purpose:
 **   This file contains the unit test runner for the gsds Application
 **
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "uttest.h"
#include "gsds_tables_test.h"
#include "gsds_cmds_test.h"
#include "gsds_test_utils.h"
/*
 * Function Definitions
 */

int main(void)
{
    GSDS_TABLES_AddTestCases();
    //GSDS_APP_AddTestCases();
    GSDS_CMDS_AddTestCases();
    return(UtTest_Run());
} /* end main */


/************************/
/*  End of File Comment */
/************************/
