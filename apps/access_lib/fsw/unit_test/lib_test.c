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
/************************************************************************************************
*   File: lib_test.c
*
*   Purose: Provides implementation of tests for the access library 
*
*   AUTHOR: Daniel McGann
*
************************************************************************************************/

/*
 * Includes
 */
#include "lib_test.h"
#include "access_lib.h"
#include "access_lib_test_utils.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include <sys/fcntl.h>
#include <unistd.h>



void ACCESS_LIB_TEST_LibInit(void) {
    int32 status;

    status = ACCESS_LibInit();

    UtAssert_True(status == CFE_SUCCESS, "LibInit Status == CFE_SUCCESS");
} /* End LibInit */

/*******************************************************************************
*   AddTestCases
*******************************************************************************/

void ACCESS_LIB_Lib_AddTestCases(void) {
    UtTest_Add(ACCESS_LIB_TEST_LibInit, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "LibInit");
} /* End AddTestCases */ 
