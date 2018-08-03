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
*	File: gsds_tables_test.c
*
*	Purpose: This file provides the implementation for all GSDS tables tests
*
*	Author: Daniel McGann
*
******************************************************************************/
#include "gsds_test_utils.h"
#include "gsds_tables_test.h"

#include "gsds_app.h"
#include "gsds_table.h"
#include "gsds_cmd.h"
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
	int32 id;
	char name[10];
} GS_Data_t;

GS_Data_t gs1;
GS_Data_t gs2;
GS_Data_t gs3;
GS_Data_t gs4;
GS_Data_t gs5;

FRMT_TBL_t* gsFrmtTbl;
FRMT_TBL_Entry_t gsFrmtTbl_e[2] =
{
	{0, 0, 4, GSDS_ID_FLAG},
	{1, 4, 10, 8}
};

FRMT_TBL_t* gsFrmtTbl_NoIdFlag;
FRMT_TBL_Entry_t gsFrmtTbl_NoIdFlag_e[2] =
{
	{0, 0, 4, FRMT_TBL_NOFLAG},
	{1, 4, 10, FRMT_TBL_NOFLAG}
};

FRMT_TBL_t* gsFrmtTbl_inValidIdFieldSize;
FRMT_TBL_Entry_t gsFrmtTbl_inValidIdFieldSize_e[2] =
{
	{0, 0, -1, GSDS_ID_FLAG},
	{1, 4, 10, FRMT_TBL_NOFLAG}
};

GSDS_StateTbl_t exTbl1;
GSDS_StateTbl_t exTbl2;

//Data Init Function
void GSDS_TEST_TABLE_InitData(void ) {

	/* Setup Format Tables */
	gsFrmtTbl = malloc((sizeof(gsFrmtTbl) + FRMT_TBL_ENTRY_SIZE*2));
	memcpy(gsFrmtTbl->Entries, gsFrmtTbl_e, FRMT_TBL_ENTRY_SIZE*2);
	gsFrmtTbl->NumEntries = 2;

	gsFrmtTbl_NoIdFlag = malloc( (sizeof(gsFrmtTbl_NoIdFlag) + FRMT_TBL_ENTRY_SIZE*2));
	memcpy(gsFrmtTbl_NoIdFlag->Entries, gsFrmtTbl_NoIdFlag_e, FRMT_TBL_ENTRY_SIZE*2);
	gsFrmtTbl_NoIdFlag->NumEntries = 2;

	gsFrmtTbl_inValidIdFieldSize = malloc( (sizeof(gsFrmtTbl_inValidIdFieldSize) + FRMT_TBL_ENTRY_SIZE*2));
	memcpy(gsFrmtTbl_inValidIdFieldSize->Entries, gsFrmtTbl_inValidIdFieldSize_e, FRMT_TBL_ENTRY_SIZE*2);
	gsFrmtTbl_inValidIdFieldSize->NumEntries = 2;

	memset(&exTbl1, 0x00, sizeof(GSDS_StateTbl_t));

	//Create Ground Stations
	gs1.id = 1;
	strcpy(gs1.name, "gs1");
	gs2.id = 2;
	strcpy(gs2.name, "gs2");
	gs3.id = 3;
	strcpy(gs3.name, "gs3");
	gs4.id = 4;
	strcpy(gs4.name, "gs4");
	gs5.id = 5;
	strcpy(gs5.name, "gs2");

	//Setup example gs state table 1

	exTbl1.NumEntries = 4;
	exTbl1.Stations[0].IsUsed = TRUE;
	memcpy(exTbl1.Stations[0].Data, &gs1, 14);
	exTbl1.Stations[1].IsUsed = TRUE;
	memcpy(exTbl1.Stations[1].Data, &gs2, 14);
	exTbl1.Stations[2].IsUsed = TRUE;
	memcpy(exTbl1.Stations[2].Data, &gs3, 14);
	exTbl1.Stations[3].IsUsed = TRUE;
	memcpy(exTbl1.Stations[3].Data, &gs4, 14);

	//Setup example gs state table 2


	exTbl2.NumEntries = 5;
	exTbl2.Stations[0].IsUsed = TRUE;
	memcpy(exTbl2.Stations[0].Data, &gs1, 14);
	exTbl2.Stations[1].IsUsed = TRUE;
	memcpy(exTbl2.Stations[1].Data, &gs2, 14);
	exTbl2.Stations[2].IsUsed = TRUE;
	memcpy(exTbl2.Stations[2].Data, &gs3, 14);
	exTbl2.Stations[3].IsUsed = TRUE;
	memcpy(exTbl2.Stations[3].Data, &gs4, 14);
	exTbl2.Stations[3].IsUsed = TRUE;
	memcpy(exTbl2.Stations[3].Data, &gs5, 14);


	// Set up AppData


} /* end InitData */

/* Function Hooks */
int32 UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
	if (TblPtr == (void**)&g_GSDS_AppData.StatePtr) {
		g_GSDS_AppData.StatePtr = &exTbl1;
	}
	else if (TblPtr == (void**)&g_GSDS_AppData.GsFrmtPtr) {
		g_GSDS_AppData.GsFrmtPtr = (FRMT_TBL_t*) gsFrmtTbl;
	}

	return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1 */

int32 UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook2 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
	if (TblPtr == (void**)&g_GSDS_AppData.StatePtr) {
		g_GSDS_AppData.StatePtr = &exTbl1;
	}
	else if (TblPtr == (void**)&g_GSDS_AppData.GsFrmtPtr) {
		g_GSDS_AppData.GsFrmtPtr = (FRMT_TBL_t*) gsFrmtTbl_NoIdFlag;
	}

	return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook2 */


int32 UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook3 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
	if (TblPtr == (void**)&g_GSDS_AppData.StatePtr) {
		g_GSDS_AppData.StatePtr = &exTbl1;
	}
	else if (TblPtr == (void**)&g_GSDS_AppData.GsFrmtPtr) {
		g_GSDS_AppData.GsFrmtPtr = (FRMT_TBL_t*) gsFrmtTbl_inValidIdFieldSize;
	}

	return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook3 */

int32 UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook4 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
	if (TblPtr == (void**)&g_GSDS_AppData.StatePtr) {
		g_GSDS_AppData.StatePtr = &exTbl2;
	}
	else if (TblPtr == (void**)&g_GSDS_AppData.GsFrmtPtr) {
		g_GSDS_AppData.GsFrmtPtr = (FRMT_TBL_t*) gsFrmtTbl;
	}

	return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook4 */


/* TESTS */

/****************************************************************************************
** GSDS_InitTable Tests
****************************************************************************************/
void GSDS_TEST_InitTable_Nominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitTable();

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end GSDS_TEST_InitTable_Nominal */

void GSDS_TEST_InitTable_CDSRecover(void) {
	int32 Result;

	/* To enter test case */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);

	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitTable();

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(GSDS_TBLINIT_INF_EID, CFE_EVS_INFORMATION,
        		"GSDS: GS State Table Recovered from CDS"),
        		"GSDS: GS State Table Recovered from CDS");
} /* end GSDS_TEST_InitTable_CDSRecover */


void GSDS_TEST_InitTable_RegErr(void) {
	int32 Result;
	/* To enter test case */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, 0x99, 1);

	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitTable();

	/* Verify Results */
	UtAssert_True (Result == 0x00000099, "Result == 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
        		"GSDS: Unable to Register GS State Table | Error: 0x00000099"),
        		"GSDS: Unable to Register GS State Table | Error: 0x00000099");

} /* end GSDS_TEST_InitTable_RegErr */

void GSDS_TEST_InitTable_LoadErr(void) {
	int32 Result;

	/* Set to enter test case */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitTable();

	/* Verify Results */
	UtAssert_True (Result == 0x00000099, "Result == 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
        		"Unable to load default GS State Table: Filename = '/cf/gsstate.tbl', Error = 0x00000099"),
        		"Unable to load default GS State Table: Filename = '/cf/gsstate.tbl', Error = 0x00000099");

} /* end GSDS_TEST_InitTable_RegErr */

/****************************************************************************************
** GSDS_InitFrmtTable Tests
****************************************************************************************/
void GSDS_TEST_InitFrmtTable_Nominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitFrmtTable();

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end GSDS_TEST_InitFrmtTable_Nominal */

void GSDS_TEST_InitFrmtTable_CDSRecover(void) {
	int32 Result;

	/* To enter test case */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);

	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitFrmtTable();

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(GSDS_TBLINIT_INF_EID, CFE_EVS_INFORMATION,
        		"GSDS: GS Format Table Recovered from CDS"),
        		"GSDS: GS Format Table Recovered from CDS");
} /* end GSDS_TEST_InitFrmtTable_CDSRecover */


void GSDS_TEST_InitFrmtTable_RegErr(void) {
	int32 Result;
	/* To enter test case */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, 0x99, 1);

	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitFrmtTable();

	/* Verify Results */
	UtAssert_True (Result == 0x00000099, "Result == 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
        		"GSDS: Unable to Register GS Format Table | Error: 0x00000099"),
        		"GSDS: Unable to Register GS Format Table | Error: 0x00000099");

} /* end GSDS_TEST_InitFrmtTable_RegErr */

void GSDS_TEST_InitFrmtTable_LoadErr(void) {
	int32 Result;

	/* Set to enter test case */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_InitFrmtTable();

	/* Verify Results */
	UtAssert_True (Result == 0x00000099, "Result == 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
        		"Unable to load default GS Format Table: Filename = '/cf/gsfrmt.tbl', Error = 0x00000099"),
        		"Unable to load default GS Format Table: Filename = '/cf/gsfrmt.tbl', Error = 0x00000099");

} /* end GSDS_TEST_InitFrmtTable_LoadErr */

/****************************************************************************************
** GSDS_VerifyStateTable Tests
****************************************************************************************/
void GSDS_TEST_VerifyStateTable_Nominal(void) {
	int32 Result;

	GSDS_TEST_TABLE_InitData();

	Result = GSDS_VerifyStateTable((void*) &exTbl1);

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_VERIY_TBL_INF_EID, CFE_EVS_INFORMATION,
			"GSDS: State Table verified | Good: 4, Bad 0"),
			"GSDS: State Table verified | Good: 4, Bad 0");

} /* end GSDS_TEST_VerifyStateTable_Nominal */

void GSDS_TEST_VerifyStateTable_NumEntriesErr(void) {
	int32 Result;

	GSDS_StateTbl_t errTbl;
	memset(&errTbl, 0x00, sizeof(GSDS_StateTbl_t));

	//Create Ground Stations
	gs1.id = 1;
	strcpy(gs1.name, "gs1");
	gs1.id = 2;
	strcpy(gs1.name, "gs2");
	gs1.id = 3;
	strcpy(gs1.name, "gs3");
	gs1.id = 4;
	strcpy(gs1.name, "gs4");

	//Setup example gs state table 1
	memset(exTbl1.Stations, 0, 256);

	errTbl.NumEntries = 2; //Error in Table
	errTbl.Stations[0].IsUsed = TRUE;
	memcpy(errTbl.Stations[0].Data, &gs1, 14);
	errTbl.Stations[1].IsUsed = TRUE;
	memcpy(errTbl.Stations[1].Data, &gs2, 14);
	errTbl.Stations[2].IsUsed = TRUE;
	memcpy(errTbl.Stations[2].Data, &gs3, 14);
	errTbl.Stations[3].IsUsed = TRUE;
	memcpy(errTbl.Stations[3].Data, &gs4, 14);

	Result = GSDS_VerifyStateTable((void*) &errTbl);


	/* Verify Results */
	UtAssert_True (Result == GSDS_VERIY_TBL_ERR_EID, "Result == GSDS_VERIY_TBL_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_VERIY_TBL_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Error State Table NumEntries incorrect | Expected: 2, Actual: 4"),
			"GSDS: Error State Table NumEntries incorrect | Expected: 2, Actual: 4");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_VERIY_TBL_INF_EID, CFE_EVS_INFORMATION,
			"GSDS: State Table verified | Good: 4, Bad 0"),
			"GSDS: State Table verified | Good: 4, Bad 0");

} /* end GSDS_TEST_VerifyStateTable_NumEntriesErr */

void GSDS_TEST_VerifyStateTable_IsUsedErr(void) {
	int32 Result;

	GSDS_StateTbl_t errTbl;
	memset(&errTbl, 0x00, sizeof(GSDS_StateTbl_t));

	//Create Ground Stations
	gs1.id = 1;
	strcpy(gs1.name, "gs1");
	gs1.id = 2;
	strcpy(gs1.name, "gs2");
	gs1.id = 3;
	strcpy(gs1.name, "gs3");
	gs1.id = 4;
	strcpy(gs1.name, "gs4");

	//Setup example gs state table 1
	memset(exTbl1.Stations, 0, 256);

	errTbl.NumEntries = 4; //Error in Table
	errTbl.Stations[0].IsUsed = 8;
	memcpy(errTbl.Stations[0].Data, &gs1, 14);
	errTbl.Stations[1].IsUsed = TRUE;
	memcpy(errTbl.Stations[1].Data, &gs2, 14);
	errTbl.Stations[2].IsUsed = TRUE;
	memcpy(errTbl.Stations[2].Data, &gs3, 14);
	errTbl.Stations[3].IsUsed = TRUE;
	memcpy(errTbl.Stations[3].Data, &gs4, 14);

	Result = GSDS_VerifyStateTable((void*) &errTbl);


	/* Verify Results */
	UtAssert_True (Result == GSDS_VERIY_TBL_ERR_EID, "Result == GSDS_VERIY_TBL_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_VERIY_TBL_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Error State Table not verifiable | Good: 3, Bad 1"),
			"GSDS: Error State Table not verifiable | Good: 3, Bad 1");

} /* end GSDS_TEST_VerifyStateTable_IsUsedErr */

/****************************************************************************************
** GSDS_TEST_VerifyFrmtTableNominal Tests
****************************************************************************************/
void GSDS_TEST_VerifyFrmtTableNominal(void) {
	int32 Result;

	Result = GSDS_VerifyFrmtTable(gsFrmtTbl);

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end GSDS_TEST_VerifyFrmtTableNominal */

/****************************************************************************************
** GSDS_GrabPtrs Tests
****************************************************************************************/
void GSDS_TEST_GrabPtrs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = GSDS_GrabPtrs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_MemCmp(g_GSDS_AppData.StatePtr, &exTbl1, sizeof(exTbl1), "g_GSDS_AppData.StatePtr == exTbl1");
	UtAssert_MemCmp(g_GSDS_AppData.GsFrmtPtr, gsFrmtTbl,
			(sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE * 2),
			"g_GSDS_AppData.GsFrmtPtr == gsFrmtTbl");


    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end GSDS_TEST_GrabPtrs_Nominal */

void GSDS_TEST_GrabPtrs_StateErr(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);

	Result = GSDS_GrabPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not grab pointers for GsState Table, Error: 0x00000099"),
			"GSDS: Could not grab pointers for GsState Table, Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end GSDS_TEST_GrabPtrs_StateErr */

void GSDS_TEST_GrabPtrs_FrmtErr(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 2);

	Result = GSDS_GrabPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not grab pointers for GsFrmt Table, Error: 0x00000099"),
			"GSDS: Could not grab pointers for GsFrmt Table, Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end GSDS_TEST_GrabPtrs_FrmtErr */

/****************************************************************************************
** GSDS_DropPtrs Tests
****************************************************************************************/
void GSDS_TEST_DropPtrs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_RELEASEADDRESS_INDEX);

	Result = GSDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end GSDS_TEST_DropPtrs_Nominal */

void GSDS_TEST_DropPtrs_StateErr(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);

	Result = GSDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not drop pointers for GsState Table, Error: 0x00000099"),
			"GSDS: Could not drop pointers for GsState Table, Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end GSDS_TEST_DropPtrs_StateErr */

void GSDS_TEST_DropPtrs_FrmtErr(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 2);

	Result = GSDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not drop pointers for GsFrmt Table, Error: 0x00000099"),
			"GSDS: Could not drop pointers for GsFrmt Table, Error: 0x00000099");


    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end GSDS_TEST_DropPtrs_FrmtErr */

/****************************************************************************************
** GSDS_DropPtrs Tests
****************************************************************************************/
void GSDS_TEST_AddGs_Nominal(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	/* Setup Data */
	GSDS_TEST_TABLE_InitData();
	GS_Data_t newGs = {15, "DAVE"};

	/* Execute Test Function */
	Result = GSDS_AddGs((char*)&newGs);

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

	UtAssert_MemCmp(g_GSDS_AppData.StatePtr->Stations[4].Data, &newGs, sizeof(newGs), "g_GSDS_AppData.StatePtr->Stations[4].Data == newGs");
	UtAssert_True (g_GSDS_AppData.StatePtr->NumEntries == 5, "g_GSDS_AppData.StatePtr->NumEntries == 5");
	OS_printf("actual entries = %d", g_GSDS_AppData.StatePtr->NumEntries);
} /* end GSDS_TEST_AddGs_Nominal */

void GSDS_TEST_AddGs_TblFullErr(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	/* Setup Data */
	GSDS_TEST_TABLE_InitData();
	GS_Data_t newGs = {15, "DAVE"};
	GSDS_GrabPtrs();

	for (int i = 0; i < GSDS_MAX_GS; i++) {
		g_GSDS_AppData.StatePtr->Stations[i].IsUsed = TRUE;
	}

	/* Execute Test Function */
	Result = GSDS_AddGs((char*)&newGs);

	/* Verify Results */
	UtAssert_True (Result == GSDS_ADDGS_ERR_EID, "Result == GSDS_ADDGS_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_ADDGS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could Not Add Ground Station to GsState Table: Table Full"),
			"GSDS: Could Not Add Ground Station to GsState Table: Table Full");

} /* end GSDS_TEST_AddGs_TblFullErr */

/****************************************************************************************
** GSDS_GetGsId Tests
****************************************************************************************/
void GSDS_TEST_GetGsId_Nominal(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	/* Setup Data */
	GS_Data_t retGs;
	int id = 1;
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsId((char*)&id, (char*)&retGs);

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 10");

	UtAssert_MemCmp(&retGs, g_GSDS_AppData.StatePtr->Stations[0].Data, sizeof(GS_Data_t), "retGs == g_GSDS_AppData.StatePtr->Stations[0].Data");
	UtAssert_MemCmp(&retGs, &gs1, sizeof(GS_Data_t), "retGs == gs1");


	/* Get a Second one Nominally */

	id = 3;
	Result = GSDS_GetGsId((char*)&id, (char*)&retGs);

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

	UtAssert_MemCmp(&retGs, g_GSDS_AppData.StatePtr->Stations[2].Data, sizeof(GS_Data_t), "retGs == g_GSDS_AppData.StatePtr->Stations[2].Data");
	UtAssert_MemCmp(&retGs, &gs3, sizeof(GS_Data_t), "retGs == gs3");
} /* end GSDS_TEST_GetGsId_Nominal */


void GSDS_TEST_GetGsId_NoIdFlag(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook2);

	/* Setup Data */
	GS_Data_t retGs;
	int id = 1;
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsId((char*)&id, (char*)&retGs);

	/* Verify Results */
	UtAssert_True (Result == -1, "Result == -1");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: No Format Table field with GSDS_ID_FLAG"),
			"GSDS: No Format Table field with GSDS_ID_FLAG");
} /* end GSDS_TEST_GetGsId_NoIdFlag */


void GSDS_TEST_GetGsId_FieldDataExtractonErr(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook3);

	/* Setup Data */
	GS_Data_t retGs;
	int id = 1;
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsId((char*)&id, (char*)&retGs);

	/* Verify Results */
	UtAssert_True (Result == -1, "Result == -1");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could Extract Ground Station Identification Data | Error: -1"),
			"GSDS: Could Extract Ground Station Identification Data | Error: -1");
} /* end GSDS_TEST_GetGsId_FieldDataExtractonErr */

void GSDS_TEST_GetGsId_NoGsFoundErr(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	/* Setup Data */
	GS_Data_t retGs;
	int id = 6;
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsId((char*)&id, (char*)&retGs);

	/* Verify Results */
	UtAssert_True (Result == GSDS_GETGGSDATA_ERR_EID, "Result == GSDS_GETGGSDATA_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: No Ground Station found with the given Id"),
			"GSDS: No Ground Station found with the given Id");
} /* end GSDS_TEST_GetGsId_NoGsFoundErr */

/****************************************************************************************
** Test GSDS_GetGsAllMatch()
****************************************************************************************/
void GSDS_TEST_GetGsAllMatch_Nominal(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook4);

	/* Setup Data */
	GS_Data_t retGs[2];
	char id[10] = "gs2";
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsAllMatch(8, (char*)id, (char*)retGs);

	/* Verify Results */
	UtAssert_True (Result == 2, "Result == 2");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

	UtAssert_MemCmp(&retGs[0], &gs2, sizeof(GS_Data_t), "retGs[0] == gs2");
	UtAssert_MemCmp(&retGs[1], &gs5, sizeof(GS_Data_t), "retGs[1] == gs5");
} /* end GSDS_TEST_GetGsAllMatch_Nominal */

void GSDS_TEST_GetGsAllMatch_FieldDataExtractonErr(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook3);

	/* Setup Data */
	GS_Data_t retGs;
	int id = 1;
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsAllMatch(1 ,(char*)&id, (char*)&retGs);

	/* Verify Results */
	UtAssert_True (Result == 0, "Result == 0");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 4, "Ut_CFE_EVS_GetEventQueueDepth() == 4");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could Extract Ground Station Identification Data from GS entry 0 | Error -1"),
			"GSDS: Could Extract Ground Station Identification Data from GS entry 0 | Error -1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could Extract Ground Station Identification Data from GS entry 1 | Error -1"),
			"GSDS: Could Extract Ground Station Identification Data from GS entry 1 | Error -1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could Extract Ground Station Identification Data from GS entry 2 | Error -1"),
			"GSDS: Could Extract Ground Station Identification Data from GS entry 2 | Error -1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could Extract Ground Station Identification Data from GS entry 3 | Error -1"),
			"GSDS: Could Extract Ground Station Identification Data from GS entry 3 | Error -1");

} /* end GSDS_TEST_GetGsAllMatch_FieldDataExtractonErr */

void GSDS_TEST_GetGsAllMatch_NoneFound(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook4);

	/* Setup Data */
	GS_Data_t retGs[2];
	char id[10] = "GS DAVE";
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsAllMatch(8, (char*)id, (char*)retGs);

	/* Verify Results */
	UtAssert_True (Result == 0, "Result == 0");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end GSDS_TEST_GetGsAllMatch_NoneFound */

void GSDS_TEST_GetGsAllMatch_NoFlag(void) {
	int32 Result;

	/* Setup Hooks */
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_GSDS_TABLE_TEST_CFE_TBL_GetAddressHook4);

	/* Setup Data */
	GS_Data_t retGs[2];
	char id[10] = "GS DAVE";
	GSDS_TEST_TABLE_InitData();
	GSDS_GrabPtrs();

	/* Execute Test Function */
	Result = GSDS_GetGsAllMatch(9, (char*)id, (char*)retGs);

	/* Verify Results */
	UtAssert_True (Result == 0, "Result == 0");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
			"GSDS: No Format Table field with given flag"),
			"GSDS: No Format Table field with given flag");

} /* end GSDS_TEST_GetGsAllMatch_NoneFound */

/******************************************************************************
* 	Add Test Cases
*/
void GSDS_TABLES_AddTestCases(void) {
//    UtTest_Add(, GSDS_Test_Setup, GSDS_Test_TearDown, "");
	UtTest_Add(GSDS_TEST_InitTable_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitTable_Nominal");
	UtTest_Add(GSDS_TEST_InitTable_CDSRecover, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitTable_CDSRecover");
	UtTest_Add(GSDS_TEST_InitTable_RegErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitTable_RegErr");
	UtTest_Add(GSDS_TEST_InitTable_LoadErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitTable_LoadErr");
	UtTest_Add(GSDS_TEST_InitFrmtTable_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitTable_Nominal");
	UtTest_Add(GSDS_TEST_InitFrmtTable_CDSRecover, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitFrmtTable_CDSRecover");
	UtTest_Add(GSDS_TEST_InitFrmtTable_RegErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitFrmtTable_RegErr");
	UtTest_Add(GSDS_TEST_InitFrmtTable_LoadErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_InitFrmtTable_LoadErr");
	UtTest_Add(GSDS_TEST_VerifyStateTable_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_VerifyStateTable_Nominal");
	UtTest_Add(GSDS_TEST_VerifyStateTable_NumEntriesErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_VerifyStateTable_NumEntriesErr");
	UtTest_Add(GSDS_TEST_VerifyStateTable_IsUsedErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_VerifyStateTable_IsUsedErr");
	UtTest_Add(GSDS_TEST_VerifyFrmtTableNominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_VerifyFrmtTableNominal");
	UtTest_Add(GSDS_TEST_GrabPtrs_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GrabPtrs_Nominal");
	UtTest_Add(GSDS_TEST_GrabPtrs_StateErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GrabPtrs_StateErr");
	UtTest_Add(GSDS_TEST_GrabPtrs_FrmtErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GrabPtrs_FrmtErr");
	UtTest_Add(GSDS_TEST_DropPtrs_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_DropPtrs_Nominal");
	UtTest_Add(GSDS_TEST_DropPtrs_StateErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_DropPtrs_StateErr");
	UtTest_Add(GSDS_TEST_DropPtrs_FrmtErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_DropPtrs_FrmtErr");
	UtTest_Add(GSDS_TEST_AddGs_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_AddGs_Nominal");
	UtTest_Add(GSDS_TEST_AddGs_TblFullErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_AddGs_TblFullErr");
	UtTest_Add(GSDS_TEST_GetGsId_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsId_Nominal");
	UtTest_Add(GSDS_TEST_GetGsId_NoIdFlag, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsId_NoIdFlag");
	UtTest_Add(GSDS_TEST_GetGsId_FieldDataExtractonErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsId_FieldDataExtractonErr");
	UtTest_Add(GSDS_TEST_GetGsId_NoGsFoundErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsId_NoGsFoundErr");
	UtTest_Add(GSDS_TEST_GetGsAllMatch_Nominal, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsAllMatch_Nominal");
	UtTest_Add(GSDS_TEST_GetGsAllMatch_FieldDataExtractonErr, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsAllMatch_FieldDataExtractonErr");
	UtTest_Add(GSDS_TEST_GetGsAllMatch_NoneFound, GSDS_Test_Setup, GSDS_Test_TearDown, "GSDS_TEST_GetGsAllMatch_NoneFound");

} /* end GSDS_TABLES_AddTestCases */
