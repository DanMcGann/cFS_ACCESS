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
*	File: sa_tables_test.c
*
*	Purpose: This file provides the implementation for all SA tables tests
*
*	Author: Daniel McGann
*
******************************************************************************/
#include "sa_test_utils.h"
#include "sa_tables_test.h"

#include "sa_app.h"
#include "sa_tables.h"
#include "sa_cmds.h"
#include "access_lib.h"

#include "cfe_tbl.h"

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
#include <stdlib.h>

/*
 * Data
 */
typedef struct {
	int32 DS;
	int32 ES;
} SA_TEST_SatelliteState_t;

SA_TEST_SatelliteState_t SA_TEST_SatelliteState;

FRMT_TBL_t* SatStateTbl;
FRMT_TBL_Entry_t SatStateTbl_e[2] = {
		{0, 0, 4, 0x1900},
		{1, 4, 4, 0x1901}
};

void SA_TEST_initData(void) {
	SatStateTbl = malloc(sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE *2);
	memcpy(SatStateTbl->Entries, SatStateTbl_e, FRMT_TBL_ENTRY_SIZE *2);
	SatStateTbl->NumEntries = 2;

	SA_TEST_SatelliteState.DS = 1000;
	SA_TEST_SatelliteState.ES = 100;
}


int32 UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    g_SA_AppData.SatFrmtPtr = (void*)SatStateTbl;
    return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1 */

int32 UT_SA_TABLE_TEST_CFE_ES_GetPoolBuf (uint32** pointer, CFE_ES_MemHandle_t handle, uint32 size)
{
    *pointer = malloc(size);

    return size + 168;
} /* end UT_SA_TABLE_TEST_CFE_ES_GetPoolBuf */

int32 UT_SA_TABLE_TEST_CFE_ES_PutPoolBuf (CFE_ES_MemHandle_t handle, uint32* pointer)
{
    free(pointer);
    g_SA_AppData.SatStatePtr = NULL;
    return 168;
} /* end UT_SA_TABLE_TEST_CFE_ES_PutPoolBuf */

/******************************************************************************
* 	InitTable Tests
*/

void SA_TEST_InitTableNominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SA_InitFrmtTable();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end SA_TEST_InitTableNominal */

void SA_TEST_InitTableCDS(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SA_InitFrmtTable();

	/* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_RECOVERED_TBL, "Result == CFE_TBL_INFO_RECOVERED_TBL");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SA_INIT_TBL_ERR_EID, CFE_EVS_INFORMATION,
        		"SA: Sat Frmt Table Recovered from CDS - SHOULD NOT OCCUR"),
        		"SA: Sat Frmt Table Recovered from CDS - SHOULD NOT OCCUR");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SA_TEST_InitTableCDS */

void SA_TEST_InitTableRegisterErr(void)
{
    int32 Result;

    /* Set to generate first instance of error message SA_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, SA_INIT_TBL_ERR_EID, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = SA_InitFrmtTable();

    /* Verify results */
    UtAssert_True (Result == SA_INIT_TBL_ERR_EID, "Result == SA_INIT_TBL_ERR_EID");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SA_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
        		"SA: Unable to Register Sat Frmt Table | Error: 0x0000003B"),
        "SA: Unable to Register Sat Frmt Table | Error: 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SA_TEST_InitTableRegisterErr */

void SA_TEST_InitFrmtTablesLoadErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	// Register type 1 table in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);


    Result = SA_InitFrmtTable();
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
            (Ut_CFE_EVS_EventSent(SA_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                    "SA: Unable to load default SatFrmt Table: Filename = '/cf/sasatfrmt.tbl', Error = 0x00000099"),
                    "SA: Unable to load default SatFrmt Table: Filename = '/cf/sasatfrmt.tbl', Error = 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* SA_TEST_InitFrmtTablesLoadErr */


/******************************************************************************
* 	VerifyTable Tests
*/

void SA_TEST_VerifyFrmtTableNominal(void) {
	int32 Result;
	SA_TEST_initData();

	Result = SA_VerifySatFrmtTable(SatStateTbl);

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SA_TEST_VerifyFrmtTableNominal */

//ADDME Error tests for verify format table

/******************************************************************************
* 	GrabPtrs Tests
*/

void  SA_TEST_GrabPtrsNominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SA_TEST_initData();

	Result = SA_GrabPtrs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_MemCmp(g_SA_AppData.SatFrmtPtr, SatStateTbl,
			(sizeof(SatStateTbl) + FRMT_TBL_ENTRY_SIZE * 2),
			"g_SA_AppData.SatFrmtTbl = SatStateTbl");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_GrabPtrs_Nominal */

void  SA_TEST_GrabPtrsErr(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
	SA_TEST_initData();

	Result = SA_GrabPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could not grab pointers for SatFrmt Table, Error: 0x00000099"),
			"SA: Could not grab pointers for SatFrmt Table, Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_GrabPtrs_Nominal */

/******************************************************************************
* 	DropPtrs Tests
*/
void  SA_TEST_DropPtrs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, CFE_SUCCESS, 1);
	SA_TEST_initData();

	SA_GrabPtrs(); //Already Tested

	Result = SA_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True(g_SA_AppData.SatFrmtPtr == NULL, "g_SA_AppData.SatFrmtPtr == NULL");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_DropPtrs_Nominal */

void  SA_TEST_DropPtrs_Err(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);
	SA_TEST_initData();

	SA_GrabPtrs(); //Already Tested

	Result = SA_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");

	UtAssert_True(g_SA_AppData.SatFrmtPtr == SatStateTbl, "g_SA_AppData.SatFrmtPtr == SatStateTbl");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_DROPPTRS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could not drop pointers for SatFrmt Table, Error: 0x00000099"),
			"SA: Could not drop pointers for SatFrmt Table, Error: 0x00000099");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SA_TEST_DropPtrs_Err */

/******************************************************************************
* 	SA_AllocMem Tests
*/
void SA_TEST_AllocMem_Nominal(void) {
	int32 Result;
	SA_TEST_initData();
	g_SA_AppData.SatStatePtr = NULL;

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_SA_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_SA_TABLE_TEST_CFE_ES_PutPoolBuf);

	Result = SA_AllocMem();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (g_SA_AppData.SatStatePtr != NULL, "g_SA_AppData.SatStatePtr != NULL");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* END SA_TEST_AllocMem_Nominal */

void SA_TEST_AllocMem_Err(void) {
	int32 Result;
	SA_TEST_initData();
	g_SA_AppData.SatStatePtr = NULL;

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX, -1, 1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_SA_TABLE_TEST_CFE_ES_PutPoolBuf);

	Result = SA_AllocMem();

	/* Verify results */
	UtAssert_True (Result == -1, "Result == -1");

	UtAssert_True (g_SA_AppData.SatStatePtr == NULL, "g_SA_AppData.SatStatePtr == NULL");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_ALLOCMEM_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Allocate Memory Pool Space | Error: 0xFFFFFFFF"),
			"SA: Could Not Allocate Memory Pool Space | Error: 0xFFFFFFFF");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* END SA_TEST_AllocMem_Nominal */

/******************************************************************************
* 	SA_FreeMem Tests
*/
void SA_TEST_FreeMem_Nominal(void) {
	int32 Result;
	SA_TEST_initData();
	g_SA_AppData.SatStatePtr = NULL;

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_SA_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_SA_TABLE_TEST_CFE_ES_PutPoolBuf);

	SA_AllocMem(); //Tested

	Result = SA_FreeMem();
//TODO
	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (g_SA_AppData.SatStatePtr == NULL, "g_SA_AppData.SatStatePtr == NULL");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* END SA_TEST_AllocMem_Nominal */

void SA_TEST_FreeMem_Err(void) {
	int32 Result;
	SA_TEST_initData();
	g_SA_AppData.SatStatePtr = NULL;

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_SA_TABLE_TEST_CFE_ES_GetPoolBuf);

	SA_AllocMem(); // Tested

	Result = SA_FreeMem();

	/* Verify results */
	UtAssert_True (Result == -1, "Result == -1");

	UtAssert_True (g_SA_AppData.SatStatePtr != NULL, "g_SA_AppData.SatStatePtr == NULL");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_FREEMEM_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Free Memory Pool Space | Error: 0xFFFFFFFF Size: 8"),
			"SA: Could Not Free Memory Pool Space | Error: 0xFFFFFFFF Size: 8");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* END SA_TEST_AllocMem_Nominal */

/******************************************************************************
* 	SA_IsValidMsgIdFlag Tests
*/
void SA_TEST_SA_IsValidMsgIdFlag_All(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	g_SA_AppData.SatFrmtEntries = 2;

	/* Verify results */
	UtAssert_True (SA_IsValidMsgIdFlag(0x1900) == TRUE, "SA_IsValidMsgIdFlag(0x1900) == TRUE");
	UtAssert_True (SA_IsValidMsgIdFlag(0x1901) == TRUE, "SA_IsValidMsgIdFlag(0x1901) == TRUE");
	UtAssert_True (SA_IsValidMsgIdFlag(0x1903) == FALSE, "SA_IsValidMsgIdFlag(0x1903) == FALSE");
	UtAssert_True (SA_IsValidMsgIdFlag(1) == FALSE, "SA_IsValidMsgIdFlag(1) == FALSE");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

}

/******************************************************************************
* 	SA_SubSatStateMsgs Tests
*/
void SA_TEST_SA_SubSatStateMsgs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_SUBSCRIBE_INDEX);
	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_SubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

void SA_TEST_SA_SubSatStateMsgs_Err1(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, 0x99, 1);

	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_SubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == 1, "Result == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Subscribe to Msg: 0x00001900 | Error: 0x00000099"),
			"SA: Could Not Subscribe to Msg: 0x00001900 | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

void SA_TEST_SA_SubSatStateMsgs_Err2(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, 0x99, 2);

	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_SubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == 1, "Result == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Subscribe to Msg: 0x00001901 | Error: 0x00000099"),
			"SA: Could Not Subscribe to Msg: 0x00001901 | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

void SA_TEST_SA_SubSatStateMsgs_Err12(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, 0x99, 1);
	Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_SUBSCRIBE_INDEX);

	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_SubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == 2, "Result == 2");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Subscribe to Msg: 0x00001900 | Error: 0x00000099"),
			"SA: Could Not Subscribe to Msg: 0x00001900 | Error: 0x00000099");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Subscribe to Msg: 0x00001901 | Error: 0x00000099"),
			"SA: Could Not Subscribe to Msg: 0x00001901 | Error: 0x00000099");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

/******************************************************************************
* 	SA_UnSubSatStateMsgs Tests
*/
void SA_TEST_SA_UnSubSatStateMsgs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_UNSUBSCRIBE_INDEX);
	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_UnSubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

void SA_TEST_SA_UnSubSatStateMsgs_Err1(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, 0x99, 1);

	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_UnSubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == 1, "Result == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Unsubscribe to Msg: 0x00001900 | Error: 0x00000099"),
			"SA: Could Not Unsubscribe to Msg: 0x00001900 | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

void SA_TEST_SA_UnSubSatStateMsgs_Err2(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, 0x99, 2);

	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_UnSubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == 1, "Result == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Unsubscribe to Msg: 0x00001901 | Error: 0x00000099"),
			"SA: Could Not Unsubscribe to Msg: 0x00001901 | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

void SA_TEST_SA_UnSubSatStateMsgs_Err12(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, 0x99, 1);
	Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_UNSUBSCRIBE_INDEX);

	g_SA_AppData.SatFrmtEntries = 2;

	Result = SA_UnSubSatStateMsgs();

	/* Verify results */
	UtAssert_True (Result == 2, "Result == 2");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Unsubscribe to Msg: 0x00001900 | Error: 0x00000099"),
			"SA: Could Not Unsubscribe to Msg: 0x00001900 | Error: 0x00000099");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could Not Unsubscribe to Msg: 0x00001901 | Error: 0x00000099"),
			"SA: Could Not Unsubscribe to Msg: 0x00001901 | Error: 0x00000099");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SA_TEST_SA_SubSatStateMsgs_Nominal */

/******************************************************************************
* 	Add Test Cases
*/
void SA_TABLES_AddTestCases(void) {
    UtTest_Add(SA_TEST_InitTableNominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_InitTableNominal");
    UtTest_Add(SA_TEST_InitTableRegisterErr, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_InitTableRegisterErr");
    UtTest_Add(SA_TEST_InitFrmtTablesLoadErr, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_InitFrmtTablesLoadErr");
    UtTest_Add(SA_TEST_InitTableCDS, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_InitTableCDS");
    UtTest_Add(SA_TEST_VerifyFrmtTableNominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_VerifyFrmtTableNominal");
    UtTest_Add(SA_TEST_GrabPtrsNominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_GrabPtrsNominal");
    UtTest_Add(SA_TEST_GrabPtrsErr, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_GrabPtrsErr");
    UtTest_Add(SA_TEST_DropPtrs_Nominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_DropPtrs_Nominal");
    UtTest_Add(SA_TEST_DropPtrs_Err, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_DropPtrs_Err");
    UtTest_Add(SA_TEST_AllocMem_Nominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_AllocMem_Nominal");
    UtTest_Add(SA_TEST_AllocMem_Err, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_AllocMem_Err");
    UtTest_Add(SA_TEST_FreeMem_Nominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_FreeMem_Nominal");
    UtTest_Add(SA_TEST_FreeMem_Err, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_FreeMem_Err");
    UtTest_Add(SA_TEST_SA_IsValidMsgIdFlag_All, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_IsValidMsgIdFlag_All");
    UtTest_Add(SA_TEST_SA_SubSatStateMsgs_Nominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SubSatStateMsgs_Nominal");
    UtTest_Add(SA_TEST_SA_SubSatStateMsgs_Err1, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SubSatStateMsgs_Err1");
    UtTest_Add(SA_TEST_SA_SubSatStateMsgs_Err2, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SubSatStateMsgs_Err2");
    UtTest_Add(SA_TEST_SA_SubSatStateMsgs_Err12, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SubSatStateMsgs_Err12");
    UtTest_Add(SA_TEST_SA_UnSubSatStateMsgs_Nominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_UnSubSatStateMsgs_Nominal");
    UtTest_Add(SA_TEST_SA_UnSubSatStateMsgs_Err1, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_UnSubSatStateMsgs_Err1");
    UtTest_Add(SA_TEST_SA_UnSubSatStateMsgs_Err2, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_UnSubSatStateMsgs_Err2");
    UtTest_Add(SA_TEST_SA_UnSubSatStateMsgs_Err12, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_UnSubSatStateMsgs_Err12");

} /* end SA_TABLES_AddTestCases */
