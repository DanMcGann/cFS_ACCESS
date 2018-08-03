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
*	File: sem_tables_test.c
*
*	Purpose: This file provides the implementation for all SEM tables tests
*
*	Author: Daniel McGann
*
******************************************************************************/
#include "sem_test_utils.h"
#include "sem_tables_test.h"

#include "sem_app.h"
#include "sem_tables.h"
#include "sem_tbldefs.h"
#include "sem_cmds.h"
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


/****************************************************************************************
** Local Functions and Data
****************************************************************************************/

/* Sat State Format Table **************************************************************/
typedef struct {
	int32 DS;
	int32 ES;
} SA_TEST_SatelliteState_t;

SA_TEST_SatelliteState_t SA_TEST_SatState;

FRMT_TBL_t* SatFrmtTbl;
FRMT_TBL_Entry_t SatFrmtTbl_e[2] = {
		{0, 0, 4, 0x1900},
		{1, 4, 4, 0x1901}
};
/***************************************************************************************/

/* SatPlan Format Table **************************************************************/
typedef struct {
	CFE_TIME_SysTime_t st;
	int32 actid;
} SEM_TEST_SatPlan_t;

SEM_TEST_SatPlan_t SEM_TEST_SatPlan;

FRMT_TBL_t* PlanFrmtTbl;
FRMT_TBL_Entry_t PlanFrmtTbl_e[8] = {
		{0, 0, 8, SEM_PFT_START_TIME_FLAG},
		{1, 8, 4, SEM_PFT_ACTION_ID_FLAG}
};


typedef struct {
	CFE_TIME_SysTime_t st;
	int32 actid;
	boolean isOverride;
} SEM_TEST_SatPlanOverride_t;


FRMT_TBL_t* PlanFrmtTblOverride;
FRMT_TBL_Entry_t PlanFrmtTblOverride_e[8] = {
		{0, 0, 8, SEM_PFT_START_TIME_FLAG},
		{1, 8, 4, SEM_PFT_ACTION_ID_FLAG},
		{2, 12, 1, SEM_PFT_OVERRIDE_FLAG}
};

/***************************************************************************************/

/* Action Table **************************************************************/

/* Check Function Pointers */
boolean SafeEs(void * data) {
	int32 a;
	a = *((int32*)data);

	return a > 25;
}

boolean SafeDs(void * data) {
	int32 a;
	a = *((int32*)data);

	return a > 10;
}


/* DS Checker */
SATCHECK_Checker_t safeDsChecker = {
		&SafeDs,	//Data storage precheck function
		{1},		//Sat State field of DS Data
		1			//Number of input parameters to SafeDs
};

SATCHECK_Checker_t safeEsChecker = {
		&SafeEs,	//Data storage precheck function
		{2},		//Sat State field of DS Data
		1			//Number of input parameters to SafeDs
};


/* action 1 */
SEM_ActionTblEntry_t action1; /* = {
		1, 					//Action ID
		RTS,				//Action initializer Type
		{{1}},				//Action Initializer Data
		0,					//Action Duration
		{{&SafeDs, {1}, 1}, {&SafeEs, {2}, 2}},	//Action PreChecks
		{{&SafeDs, {1}, 1}, {&SafeEs, {2}, 2}}	//Action DurChecks

};
*/

/* action 2 */

char action2_msg[CFE_SB_CMD_HDR_SIZE];

SEM_ActionTblEntry_t action2; /* {
		2, 					//Action ID
		CMD,				//Action initializer Type
		action2_msg,		//Action Initializer Data
		100,				//Action Duration
		{{&SafeDs, {1}, 1}, {&SafeEs, {2}, 2}},	//Action PreChecks
		{{&SafeDs, {1}, 1}, {&SafeEs, {2}, 2}}	//Action DurChecks

};
*/


SEM_ActionTbl_t actionTbl; /* = { 1,
		{action1, action2}
};
*/
/***************************************************************************************/

/* Schedule Table **************************************************************/

SEM_ScheduleTbl_t schTbl= { 6,
		{
			{SEM_SCHEDULED, {0,0}, 	1},
			{SEM_SCHEDULED, {10,0}, 2},
			{SEM_SCHEDULED, {20,0}, 1},
			{SEM_SCHEDULED, {30,0}, 2},
			{SEM_SCHEDULED, {40,0}, 1},
			{SEM_SCHEDULED, {50,0}, 2}
		}
};

SEM_ScheduleTbl_t schTbl_b = { 6,
	{
		{SEM_SCHEDULED, {0,0}, 	1},
		{SEM_SCHEDULED, {10,0}, 2},
		{SEM_SCHEDULED, {20,0}, 1},
		{SEM_SCHEDULED, {30,0}, 2},
		{SEM_SCHEDULED, {40,0}, 1},
		{SEM_SCHEDULED, {50,0}, 2}
	}
};
/***************************************************************************************/


//Data Init Function
void SEM_TEST_TABLE_InitData(void ) {

	/* Setup Sat State Format Table */
	SatFrmtTbl = malloc(sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE *2);
	memcpy(SatFrmtTbl->Entries, SatFrmtTbl_e, FRMT_TBL_ENTRY_SIZE *2);
	SatFrmtTbl->NumEntries = 2;

	/* Setup Plan Format Table */
	PlanFrmtTbl = malloc(sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE *2);
	memcpy(PlanFrmtTbl->Entries, PlanFrmtTbl_e, FRMT_TBL_ENTRY_SIZE *2);
	PlanFrmtTbl->NumEntries = 2;

	/* Setup Plan Format Table with overrides*/
	PlanFrmtTblOverride = malloc(sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE *3);
	memcpy(PlanFrmtTblOverride->Entries, PlanFrmtTblOverride_e, FRMT_TBL_ENTRY_SIZE *3);
	PlanFrmtTblOverride->NumEntries = 3;

	SA_TEST_SatState.DS = 1000;
	SA_TEST_SatState.ES = 100;


	/* setup actions */
	action1.id = 1;
	action1.type = RTS;
	action1.init.Rts = 1;
	action1.duration = 0;
	action1.precheck[0] =  safeDsChecker;
	action1.precheck[1] = safeEsChecker;
	action1.durcheck[0] =  safeDsChecker;
	action1.durcheck[1] = safeEsChecker;

	CFE_SB_InitMsg((CFE_SB_MsgPtr_t)&action2_msg,
			0x1900,
			CFE_SB_CMD_HDR_SIZE,
			TRUE);
	CFE_SB_GenerateChecksum((CFE_SB_MsgPtr_t)&action2_msg);


	action2.id = 2;
	action2.type = CMD;
	memcpy(action2.init.Cmd, action2_msg, CFE_SB_CMD_HDR_SIZE);
	action2.duration = 100;
	action2.precheck[0] =  safeDsChecker;
	action2.precheck[1] = safeEsChecker;
	action2.durcheck[0] =  safeDsChecker;
	action2.durcheck[1] = safeEsChecker;

	/* Setup Action Table */
	CFE_PSP_MemSet(&actionTbl, 0x00, sizeof(actionTbl));
	actionTbl.Actions[0] = action1;
	actionTbl.Actions[1] = action2;
	actionTbl.NumEntries = 2;

	for (int i = 2; i < SEM_MAX_ACTIONS; i++) {
		actionTbl.Actions[i].id = SEM_UNUSED_ENTRY;
	}
	for (int i = 6; i < SEM_MAX_SCHEDULE_LEN; i++) {
		schTbl.Schedule[i].Action = SEM_UNUSED_ENTRY;
	}

	g_SEM_AppData.ScheduleTblHandle = 8;
	g_SEM_AppData.ActionTblHandle = 9;
	g_SEM_AppData.PlanFrmtTblHandle = 10;
	g_SEM_AppData.SatFrmtTblHandle = 11;

	memcpy(&schTbl, &schTbl_b, sizeof(schTbl_b));


} /* end InitData */

int32 UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
	if (TblHandle == g_SEM_AppData.ActionTblHandle) {
		g_SEM_AppData.ActionTblPtr = &actionTbl;
	}
	else if (TblHandle == g_SEM_AppData.ScheduleTblHandle) {
		g_SEM_AppData.ScheduleTblPtr = &schTbl;
	}
	else if (TblHandle == g_SEM_AppData.PlanFrmtTblHandle) {
		g_SEM_AppData.PlanFrmtTblPtr = PlanFrmtTbl;
	}
	else if (TblHandle == g_SEM_AppData.SatFrmtTblHandle){
		g_SEM_AppData.SatFrmtTblPtr = SatFrmtTbl;
	}

	return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1 */

int32 UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook2 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
	if (TblHandle == g_SEM_AppData.ActionTblHandle) {
		g_SEM_AppData.ActionTblPtr = &actionTbl;
	}
	else if (TblHandle == g_SEM_AppData.ScheduleTblHandle) {
		g_SEM_AppData.ScheduleTblPtr = &schTbl;
	}
	else if (TblHandle == g_SEM_AppData.PlanFrmtTblHandle) {
		g_SEM_AppData.PlanFrmtTblPtr = PlanFrmtTblOverride;
	}
	else if (TblHandle == g_SEM_AppData.SatFrmtTblHandle){
		g_SEM_AppData.SatFrmtTblPtr = SatFrmtTbl;
	}

	return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook2 */

CFE_TIME_SysTime_t UT_SEM_TABLE_TEST_CFE_TIME_GetTimeHook(void) {
	CFE_TIME_SysTime_t time;

	time.Seconds = 10;
	time.Subseconds = 5000;

	return time;
} /* END UT_SEM_TABLE_TEST_CFE_TBL_GetTimeHook */



/****************************************************************************************
** SEM_InitTables Tests
****************************************************************************************/
void SEM_TEST_InitTables_Nominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_SHARE_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitTables();

	/* Verify Results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end SEM_TEST_InitTables_Nominal */


void SEM_TEST_InitTables_RegisterErr(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_SHARE_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, 0x99, 1);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitTables();

	/* Verify Results */
	UtAssert_True (Result == SEM_INIT_TBL_ERR_EID, "Result == SEM_INIT_TBL_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

	 UtAssert_True
	    (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
	   		"SEM: Table Init Failed - SEM requires tables for functionality - STOP"),
	        "SEM: Table Init Failed - SEM requires tables for functionality - STOP");
	 UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Unable to register Action Table, Error = 0x00000099"),
			"SEM: Unable to register Action Table, Error = 0x00000099");
} /* end SEM_TEST_InitTables_RegisterErr */

void SEM_TEST_InitTables_ShareErr(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_SHARE_INDEX, 0x99, 1);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitTables();

	/* Verify Results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	 UtAssert_True
	    (Ut_CFE_EVS_EventSent(59, CFE_EVS_ERROR,
	   		"SEM: Could Not Share Satellite State Format Table | Error: 0x00000099"),
	        "SEM: Could Not Share Satellite State Format Table | Error: 0x00000099");
} /* end SEM_TEST_InitTables_ShareErr */

/*
 * Init Action Table
 */
void SEM_TEST_InitActionTable_Nominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitActionTable();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end SEM_TEST_InitTableNominal */

void SEM_TEST_InitActionTable_CDS(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitActionTable();

	/* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_RECOVERED_TBL, "Result == CFE_TBL_INFO_RECOVERED_TBL");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_CDS_EID, CFE_EVS_INFORMATION,
        		"SEM: Action Table recovered from CDS"),
        		"SEM: Action Table recovered from CDS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SEM_TEST_InitActionTable_CDS */

void SEM_TEST_InitActionTable_RegErr(void)
{
    int32 Result;

    /* Set to generate first instance of error message SEM_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, SEM_INIT_TBL_ERR_EID, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = SEM_InitActionTable();

    /* Verify results */
    UtAssert_True (Result == SEM_INIT_TBL_ERR_EID, "Result == SEM_INIT_TBL_ERR_EID");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
        	"SEM: Unable to register Action Table, Error = 0x0000003B"),
        	"SEM: Unable to register Action Table, Error = 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SEM_TEST_InitActionTable_RegErr */


void SEM_TEST_InitActionTable_LoadErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	// Register type 1 table in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);


    Result = SEM_InitActionTable();
    /* Verify results */
    UtAssert_True (Result == 0x99, "Result == 0x99");

    UtAssert_True
            (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                    "SEM: Unable to load default Action Table: Filename = 'sem_action.tbl' | Error: 0x00000099"),
                    "SEM: Unable to load default Action Table: Filename = 'sem_action.tbl' | Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* SEM_TEST_InitActionTable_LoadErr */

/*
 * Init Schedule Table
 */
void SEM_TEST_InitScheduleTable_Nominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitScheduleTable();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end SEM_TEST_InitTableNominal */

void SEM_TEST_InitScheduleTable_CDS(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitScheduleTable();

	/* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_RECOVERED_TBL, "Result == CFE_TBL_INFO_RECOVERED_TBL");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_CDS_EID, CFE_EVS_INFORMATION,
        		"SEM: Schedule Table recovered from CDS"),
        		"SEM: Schedule Table recovered from CDS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SEM_TEST_InitScheduleTable_CDS */

void SEM_TEST_InitScheduleTable_RegErr(void)
{
    int32 Result;

    /* Set to generate first instance of error message SEM_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, SEM_INIT_TBL_ERR_EID, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = SEM_InitScheduleTable();

    /* Verify results */
    UtAssert_True (Result == SEM_INIT_TBL_ERR_EID, "Result == SEM_INIT_TBL_ERR_EID");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
        	"SEM: Unable to register Schedule Table, Error = 0x0000003B"),
        	"SEM: Unable to register Schedule Table, Error = 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SEM_TEST_InitScheduleTable_RegErr */


void SEM_TEST_InitScheduleTable_LoadErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	// Register type 1 table in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);



    Result = SEM_InitScheduleTable();
    /* Verify results */
    UtAssert_True (Result == 0x99, "Result == 0x99");

    UtAssert_True
            (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                    "SEM: Unable to load default Schedule Table: Filename = 'sem_schedule.tbl' | Error: 0x00000099"),
                    "SEM: Unable to load default Schedule Table: Filename = 'sem_schedule.tbl' | Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* SEM_TEST_InitScheduleTable_LoadErr */


/*
 * Init PlanFrmt Table
 */
void SEM_TEST_InitPlanFrmtTable_Nominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitPlanFrmtTable();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end SEM_TEST_InitTableNominal */

void SEM_TEST_InitPlanFrmtTable_CDS(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = SEM_InitPlanFrmtTable();

	/* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_RECOVERED_TBL, "Result == CFE_TBL_INFO_RECOVERED_TBL");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_CDS_EID, CFE_EVS_INFORMATION,
        		"SEM: Plan Format Table recovered from CDS"),
        		"SEM: Plan Format Table recovered from CDS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SEM_TEST_InitPlanFrmtTable_CDS */

void SEM_TEST_InitPlanFrmtTable_RegErr(void)
{
    int32 Result;

    /* Set to generate first instance of error message SEM_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, SEM_INIT_TBL_ERR_EID, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = SEM_InitPlanFrmtTable();

    /* Verify results */
    UtAssert_True (Result == SEM_INIT_TBL_ERR_EID, "Result == SEM_INIT_TBL_ERR_EID");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
        	"SEM: Unable to register Plan Format Table, Error = 0x0000003B"),
        	"SEM: Unable to register Plan Format Table, Error = 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end SEM_TEST_InitPlanFrmtTable_RegErr */


void SEM_TEST_InitPlanFrmtTable_LoadErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	// Register type 1 table in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);



    Result = SEM_InitPlanFrmtTable();
    /* Verify results */
    UtAssert_True (Result == 0x99, "Result == 0x99");

    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Unable to load default Plan Format Table: Filename = 'sem_planfrmt.tbl' | Error: 0x00000099"),
			"SEM: Unable to load default Plan Format Table: Filename = 'sem_planfrmt.tbl' | Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* SEM_TEST_InitPlanFrmtTable_LoadErr */


/****************************************************************************************
** SEM_Manage/ATables Tests
****************************************************************************************/

void SEM_TABLES_ManageTables_Nominal(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


    Result = SEM_ManageTables();
    /* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SEM_TABLES_ManageTables_Nominal*/

void SEM_TABLES_ManageTables_ActionManageErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, 0x99, 1);

    Result = SEM_ManageTables();
    /* Verify results */

    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_MANAGE_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Action Table Management Failed | Error: 0x00000099"),
			"SEM: Action Table Management Failed | Error: 0x00000099");


	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SEM_TABLES_ManageTables_ActionManageErr*/

void SEM_TABLES_ManageTables_ScheduleValidate(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);

    Result = SEM_ManageTables();
    /* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_VALIDATION_PENDING, "Result == CFE_TBL_INFO_VALIDATION_PENDING");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SEM_TABLES_ManageTables_ScheduleValidate*/

void SEM_TABLES_ManageTables_ScheduleUpdate_Nominal(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);

    g_SEM_AppData.ExeScheduleFlag = FALSE;

    Result = SEM_ManageTables();
    /* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_UPDATE_PENDING, "Result == CFE_TBL_INFO_UPDATE_PENDING");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SEM_TABLES_ManageTables_ScheduleUpdate*/

void SEM_TABLES_ManageTables_ScheduleUpdate_Err(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);

    g_SEM_AppData.ExeScheduleFlag = TRUE;

    Result = SEM_ManageTables();
    /* Verify results */

    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_INVALID_SCHLOAD_ERR_EID, CFE_EVS_ERROR,
			"SEM: Schedule Load attempt while current schedule is executing - Blocking Load"),
			"SEM: Schedule Load attempt while current schedule is executing - Blocking Load");

	UtAssert_True (Result == SEM_INVALID_SCHLOAD_ERR_EID, "Result == SEM_INVALID_SCHLOAD_ERR_EID");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SEM_TABLES_ManageTables_ScheduleUpdate_Err*/

void SEM_TABLES_ManageTables_ScheduleDump(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);

    g_SEM_AppData.ExeScheduleFlag = FALSE;

    Result = SEM_ManageTables();
    /* Verify results */
	UtAssert_True (Result == CFE_TBL_INFO_DUMP_PENDING, "Result == CFE_TBL_INFO_DUMP_PENDING");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SEM_TABLES_ManageTables_ScheduleDump*/

void SEM_TABLES_ManageTables_ScheduleErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, 0x99, 1);

    g_SEM_AppData.ExeScheduleFlag = FALSE;

    Result = SEM_ManageTables();
    /* Verify results */

    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_INVALID_SCHLOAD_ERR_EID, CFE_EVS_ERROR,
			"SEM: Table Management has failed | Error: 0x00000099"),
			"SEM: Table Management has failed | Error: 0x00000099");

	UtAssert_True (Result == 0x99, "Result == 0x99");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SEM_TABLES_ManageTables_ScheduleErr*/


/****************************************************************************************
** SEM_VerifyActionTable
****************************************************************************************/

void SEM_TABLES_VerifyActionTable_Nominal(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_VerifyActionTable(&actionTbl);

	/* Verify results */
    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_INF_EID, CFE_EVS_ERROR,
			"SEM: Action Table Verification Success | Good: 2, Bad: 0"),
			"SEM: Action Table Verification Success | Good: 2, Bad: 0");

	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* END SEM_TABLES_VerifyActionTable_Nominal */

void SEM_TABLES_VerifyActionTable_NumEntriesErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	actionTbl.NumEntries = 5;

	Result = SEM_VerifyActionTable(&actionTbl);

	/* Verify results */
    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_INFORMATION,
			"SEM: Action Table Verification num entires does not match number of used entries | Non-Fatal Error"),
			"SEM: Action Table Verification num entires does not match number of used entries | Non-Fatal Error");

    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_INF_EID, CFE_EVS_ERROR,
			"SEM: Action Table Verification Success | Good: 2, Bad: 0"),
			"SEM: Action Table Verification Success | Good: 2, Bad: 0");

	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* END SEM_TABLES_VerifyActionTable_NumEntriesErr */

void SEM_TABLES_VerifyActionTable_BadCmd(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	actionTbl.NumEntries = 3;
	actionTbl.Actions[2].id = 3;

	Result = SEM_VerifyActionTable(&actionTbl);

	/* Verify results */
    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Action Table Verification Failed | Good: 2, Bad: 1"),
			"SEM: Action Table Verification Failed | Good: 2, Bad: 1");

	UtAssert_True (Result == SEM_VERIFY_TBL_ERR_EID, "Result == SEM_VERIFY_TBL_ERR_EID");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* END SEM_TABLES_VerifyActionTable_BadCmd */

/****************************************************************************************
** SEM_VerifyScheduleTable
****************************************************************************************/

void SEM_TABLES_VerifyScheduleTable_Nominal(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();


	Result = SEM_VerifyScheduleTable(&schTbl);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Schedule Table Verification Success | Good: 6, Bad: 0"),
			"SEM: Schedule Table Verification Success | Good: 6, Bad: 0");
} /* END SEM_TABLES_VerifyScheduleTable_Nominal */

void SEM_TABLES_VerifyScheduleTable_NumEntriesErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	schTbl.NumEntries = 10;

	Result = SEM_VerifyScheduleTable(&schTbl);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

	UtAssert_True
			(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_INFORMATION,
				"SEM: Schedule Table Verification num entires does not match number of used entries | Non-Fatal Error"),
				"SEM: Schedule Table Verification num entires does not match number of used entries | Non-Fatal Error");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Schedule Table Verification Success | Good: 6, Bad: 0"),
			"SEM: Schedule Table Verification Success | Good: 6, Bad: 0");
} /* END SEM_TABLES_VerifyScheduleTable_NumEntriesErr */


void SEM_TABLES_VerifyScheduleTable_InvalidActionErr(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	SEM_ScheduleTbl_t sch;
	memcpy(&sch, &schTbl, sizeof(schTbl));

	sch.Schedule[2].Action = 5;

	Result = SEM_VerifyScheduleTable(&sch);

	/* Verify results */
	UtAssert_True (Result == SEM_VERIFY_TBL_ERR_EID, "Result == SEM_VERIFY_TBL_ERR_EID");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Schedule Table Verification Failed | 1 Invalid Actions Scheduled"),
			"SEM: Schedule Table Verification Failed | 1 Invalid Actions Scheduled");
} /* END SEM_TABLES_VerifyScheduleTable_InvalidActionErr */


/****************************************************************************************
** SEM_VerifyPlanFrmtTable
**************************************************************************************/

void SEM_TABLES_VerifyPlanFrmtTbl_Nominal(void) {
	int32 Result;

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_VerifyPlanFrmtTable(&PlanFrmtTbl);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SEM_TABLES_VerifyPlanFrmtTbl */

void SEM_TABLES_VerifyPlanFrmtTbl_NoST(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	PlanFrmtTbl->Entries[0].Flag = 1234;

	Result = SEM_VerifyPlanFrmtTable((void*)PlanFrmtTbl);

	PlanFrmtTbl->Entries[0].Flag = SEM_PFT_START_TIME_FLAG;

	/* Verify results */
	UtAssert_True (Result == SEM_VERIFY_TBL_ERR_EID, "Result == SEM_VERIFY_TBL_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Plan Format Table does not contain entry with REQUIRED] SEM_PFT_START_TIME_FLAG"),
			"SEM: Plan Format Table does not contain entry with REQUIRED] SEM_PFT_START_TIME_FLAG");

} /* end SEM_TABLES_VerifyPlanFrmtTbl_NoST */

void SEM_TABLES_VerifyPlanFrmtTbl_NoAct(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	PlanFrmtTbl->Entries[1].Flag = 1234;

	Result = SEM_VerifyPlanFrmtTable((void*)PlanFrmtTbl);

	PlanFrmtTbl->Entries[1].Flag = SEM_PFT_ACTION_ID_FLAG;

	/* Verify results */
	UtAssert_True (Result == SEM_VERIFY_TBL_ERR_EID, "Result == SEM_VERIFY_TBL_ERR_EID");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Plan Format Table does not contain entry with REQUIRED] SEM_PFT_ACTION_ID_FLAG"),
			"SEM: Plan Format Table does not contain entry with REQUIRED] SEM_PFT_ACTION_ID_FLAG");

} /* end SEM_TABLES_VerifyPlanFrmtTbl_NoAct */

/****************************************************************************************
** SEM_GrabschedulePlanPtr
**************************************************************************************/
void SEM_TABLES_GrabScheduleTblPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabScheduleTblPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.ScheduleTblPtr == &schTbl, "g_SEM_AppData.ScheduleTblPtr == &schTbl");
} /* END SEM_TABLES_GrabScheduleTblPtr_Nominal */

void SEM_TABLES_GrabScheduleTblPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabScheduleTblPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Schedule Table | Error: 0x00000099"),
			"SEM: Grab Pointers Failed for Schedule Table | Error: 0x00000099");

} /* END SEM_TABLES_GrabScheduleTblPtr_Err */

/****************************************************************************************
** SEM_GrabActionPlanPtr
**************************************************************************************/
void SEM_TABLES_GrabActionTblPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabActionTblPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.ActionTblPtr == &actionTbl, "g_SEM_AppData.ActionTblPtr == &actionTbl");
} /* END SEM_TABLES_GrabActionTblPtr_Nominal */

void SEM_TABLES_GrabActionTblPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabActionTblPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Action Table | Error: 0x00000099"),
			"SEM: Grab Pointers Failed for Action Table | Error: 0x00000099");

} /* END SEM_TABLES_GrabActionTblPtr_Err */

/****************************************************************************************
** SEM_GrabPlanFrmtPlanPtr
**************************************************************************************/
void SEM_TABLES_GrabPlanFrmtTblPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabPlanFrmtTblPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.PlanFrmtTblPtr == PlanFrmtTbl, "g_SEM_AppData.PlanFrmtTblPtr == PlanFrmtTbl");
} /* END SEM_TABLES_GrabPlanFrmtTblPtr_Nominal */

void SEM_TABLES_GrabPlanFrmtTblPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabPlanFrmtTblPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Plan Format Table | Error: 0x00000099"),
			"SEM: Grab Pointers Failed for PlanFormat Table | Error: 0x00000099");

} /* END SEM_TABLES_GrabPlanFrmtTblPtr_Err */

/****************************************************************************************
** SEM_GrabSatStateFrmtPlanPtr
**************************************************************************************/
void SEM_TABLES_SEM_GrabSatStateFrmtPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabSatStateFrmtPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.SatFrmtTblPtr == SatFrmtTbl, "g_SEM_AppData.SatStateFrmtTblPtr == SatFrmtTbl");
} /* END SEM_TABLES_GrabSatStateFrmtTblPtr_Nominal */

void SEM_TABLES_SEM_GrabSatStateFrmtPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_GrabSatStateFrmtPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Sat State Format Table | Error: 0x00000099"),
			"SEM: Grab Pointers Failed for Sat State Format Table | Error: 0x00000099");

} /* END SEM_TABLES_GrabSatStateFrmtTblPtr_Err */


/****************************************************************************************
** SEM_DropschedulePlanPtr
**************************************************************************************/
void SEM_TABLES_DropScheduleTblPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, CFE_SUCCESS, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropScheduleTblPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.ScheduleTblPtr == NULL, "g_SEM_AppData.ScheduleTblPtr == NULL");
} /* END SEM_TABLES_DropScheduleTblPtr_Nominal */

void SEM_TABLES_DropScheduleTblPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropScheduleTblPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Schedule Table | Error: 0x00000099"),
			"SEM: Drop Pointers Failed for Schedule Table | Error: 0x00000099");

} /* END SEM_TABLES_DropScheduleTblPtr_Err */

/****************************************************************************************
** SEM_DropActionPlanPtr
**************************************************************************************/
void SEM_TABLES_DropActionTblPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, CFE_SUCCESS, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropActionTblPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.ActionTblPtr == NULL, "g_SEM_AppData.ActionTblPtr == NULL");
} /* END SEM_TABLES_DropActionTblPtr_Nominal */

void SEM_TABLES_DropActionTblPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropActionTblPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Action Table | Error: 0x00000099"),
			"SEM: Drop Pointers Failed for Action Table | Error: 0x00000099");

} /* END SEM_TABLES_DropActionTblPtr_Err */

/****************************************************************************************
** SEM_DropPlanFrmtPlanPtr
**************************************************************************************/
void SEM_TABLES_DropPlanFrmtTblPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, CFE_SUCCESS, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropPlanFrmtTblPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.PlanFrmtTblPtr == NULL, "g_SEM_AppData.PlanFrmtTblPtr == NULL");
} /* END SEM_TABLES_DropPlanFrmtTblPtr_Nominal */

void SEM_TABLES_DropPlanFrmtTblPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropPlanFrmtTblPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Plan Format Table | Error: 0x00000099"),
			"SEM: Drop Pointers Failed for PlanFormat Table | Error: 0x00000099");

} /* END SEM_TABLES_DropPlanFrmtTblPtr_Err */

/****************************************************************************************
** SEM_DropSatStateFrmtPlanPtr
**************************************************************************************/
void SEM_TABLES_SEM_DropSatStateFrmtPtr_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, CFE_SUCCESS, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropSatStateFrmtPtr();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
	UtAssert_True (g_SEM_AppData.SatFrmtTblPtr == NULL, "g_SEM_AppData.SatStateFrmtTblPtr == NULL");
} /* END SEM_TABLES_DropSatStateFrmtTblPtr_Nominal */

void SEM_TABLES_SEM_DropSatStateFrmtPtr_Err(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);
	SEM_TEST_TABLE_InitData();

	Result = SEM_DropSatStateFrmtPtr();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Sat State Format Table | Error: 0x00000099"),
			"SEM: Drop Pointers Failed for Sat State Format Table | Error: 0x00000099");

} /* END SEM_TABLES_DropSatStateFrmtTblPtr_Err */


/****************************************************************************************
** SEM_IsValidActionId
**************************************************************************************/
void SEM_TABLES_SEM_IsValidActionId_All(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	UtAssert_True (SEM_IsValidActionId(1) == TRUE, "SEM_IsValidActionId(1) == TRUE");
	UtAssert_True (SEM_IsValidActionId(2) == TRUE, "SEM_IsValidActionId(2) == TRUE");
	UtAssert_True (SEM_IsValidActionId(3) == FALSE, "SEM_IsValidActionId(1) == FALSE");
	UtAssert_True (SEM_IsValidActionId(654) == FALSE, "SEM_IsValidActionId(654) == FALSE");
	UtAssert_True (SEM_IsValidActionId(0) == FALSE, "SEM_IsValidActionId(0) == FALSE");
	UtAssert_True (SEM_IsValidActionId(-1324) == FALSE, "SEM_IsValidActionId(-1324) == FALSE");

} /* END SEM_TABLES_SEM_IsValidActionId_All */


/****************************************************************************************
** SEM_IsValidActionId
**************************************************************************************/
void SEM_TABLES_SEM_IsOverridePlan_Nominal(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	SEM_TEST_SatPlan_t plan1[3] = {
			{{1,0}, 1},
			{{1,0}, 2},
			{{1,0}, 1},
	};

	UtAssert_True (SEM_IsOverridePlan((void*)plan1, sizeof(plan1)) == FALSE, "SEM_IsOverridePlan((void*)&plan1, sizeof(plan1)) == FALSE");
} /* END SEM_TABLES_SEM_IsOverridePlan_Nominal */

void SEM_TABLES_SEM_IsOverridePlan_Override(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook2);
	SEM_TEST_TABLE_InitData();

	SEM_TEST_SatPlanOverride_t plan1[3] = {
			{{1,0}, 1, TRUE},
			{{1,0}, 2, FALSE},
			{{1,0}, 1, FALSE},
	};

	SEM_TEST_SatPlanOverride_t plan2[3] = {
			{{1,0}, 1, FALSE},
			{{1,0}, 2, FALSE},
			{{1,0}, 1, FALSE},
	};

	SEM_TEST_SatPlanOverride_t plan3[3] = {
			{{1,0}, 1, FALSE},
			{{1,0}, 2, TRUE},
			{{1,0}, 1, FALSE},
	};

	UtAssert_True (SEM_IsOverridePlan((void*)plan1, sizeof(plan1)) == TRUE, "SEM_IsOverridePlan((void*)&plan1, sizeof(plan1)) == TRUE");
	UtAssert_True (SEM_IsOverridePlan((void*)plan2, sizeof(plan2)) == FALSE, "SEM_IsOverridePlan((void*)&plan2, sizeof(plan2)) == FALSE");
	UtAssert_True (SEM_IsOverridePlan((void*)plan3, sizeof(plan3)) == FALSE, "SEM_IsOverridePlan((void*)&plan3, sizeof(plan3)) == FALSE");
} /* END SEM_TABLES_SEM_IsOverridePlan_Override */

/****************************************************************************************
** SEM_GetScheduledAction
**************************************************************************************/
void SEM_TEST_GetScheduleAction_All(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX, &UT_SEM_TABLE_TEST_CFE_TIME_GetTimeHook);


	SEM_TEST_TABLE_InitData();

	/*	CFE_GetTime returns {10, 5000}
	*
	*
	*	SEM_ScheduleTbl_t schTbl = { 6,
	*	{
	*			{SEM_SCHEDULED, {0,0}, 	1},
	*			{SEM_SCHEDULED, {10,0}, 2},
	*			{SEM_SCHEDULED, {20,0}, 1},
	*			{SEM_SCHEDULED, {30,0}, 2},
	*			{SEM_SCHEDULED, {40,0}, 1},
	*			{SEM_SCHEDULED, {50,0}, 2}
	*		}
	*	};
	*/

	g_SEM_AppData.NextActionIndex = 1;
	UtAssert_True (SEM_GetScheduledAction() == 2, "SEM_GetScheduledAction() == 2");

	g_SEM_AppData.NextActionIndex = 2;
	UtAssert_True (SEM_GetScheduledAction() == SEM_NO_ACTION, "SEM_GetScheduledAction() == SEM_NO_ACTION");

	g_SEM_AppData.NextActionIndex = 1;
	schTbl.Schedule[1].StartTime.Seconds = 9;
	UtAssert_True (SEM_GetScheduledAction() == SEM_NO_ACTION, "SEM_GetScheduledAction() == SEM_NO_ACTION");

	g_SEM_AppData.NextActionIndex = 1;
	schTbl.Schedule[1].StartTime.Seconds = 11;
	UtAssert_True (SEM_GetScheduledAction() == SEM_NO_ACTION, "SEM_GetScheduledAction() == SEM_NO_ACTION");
	schTbl.Schedule[1].StartTime.Seconds = 10; //Reset

	g_SEM_AppData.NextActionIndex = 6;
	UtAssert_True (SEM_GetScheduledAction() == SEM_END_SCH, "SEM_GetScheduledAction() == SEM_END_SCH");


	/* cannot get the pointer */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
	g_SEM_AppData.NextActionIndex = 1;
	UtAssert_True (SEM_GetScheduledAction() == SEM_NO_ACTION, "SEM_GetScheduledAction() == SEM_NO_ACTION");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Schedule Table | Error: 0x00000099"),
			"SEM: Grab Pointers Failed for Schedule Table | Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* END SEM_TEST_GetScheduleAction_Nominal*/

/****************************************************************************************
** SEM_GetScheduledAction
**************************************************************************************/
void SEM_TABLES_GetAction_All(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	UtAssert_True (SEM_GetAction(1) == &(actionTbl.Actions[0]), "SEM_GetAction(1) == &(actionTbl.Actions[0])");
	UtAssert_True (SEM_GetAction(2) == &(actionTbl.Actions[1]), "SEM_GetAction(2) == &(actionTbl.Actions[1])");

	UtAssert_True (SEM_GetAction(3) == NULL, "SEM_GetAction(3) == NULL");
	UtAssert_True (SEM_GetAction(-11) == NULL, "SEM_GetAction(-11) == NULL");
	UtAssert_True (SEM_GetAction(123) == NULL, "SEM_GetAction(123) == NULL");

}

/****************************************************************************************
** SEM_SetSchStatus
**************************************************************************************/
void SEM_TABLES_SetSchStatus_All(void) {
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SEM_TABLE_TEST_CFE_TBL_GetAddressHook1);
	SEM_TEST_TABLE_InitData();

	SEM_SetSchStatus(0, SEM_UNUSED);
	UtAssert_True (schTbl.Schedule[0].Status == SEM_UNUSED, "schTbl.Schedule[0].Status == SEM_UNUSED");

	SEM_TEST_TABLE_InitData();

	SEM_SetSchStatus(1, SEM_EXECUTED);
	UtAssert_True (schTbl.Schedule[1].Status == SEM_EXECUTED, "schTbl.Schedule[1].Status == SEM_EXECUTED");

	SEM_TEST_TABLE_InitData();

	SEM_SetSchStatus(1, SEM_CHECKFAILED);
	UtAssert_True (schTbl.Schedule[1].Status == SEM_CHECKFAILED, "schTbl.Schedule[1].Status == SEM_CHECKFAILED");

	SEM_TEST_TABLE_InitData();

	SEM_SetSchStatus(1, SEM_SKIPPED);
	UtAssert_True (schTbl.Schedule[1].Status == SEM_SKIPPED, "schTbl.Schedule[1].Status == SEM_SKIPPED");

}

/******************************************************************************
* 	Add Test Cases
*/
void SEM_TABLES_AddTestCases(void) {

	/* Table initialization */
	UtTest_Add(SEM_TEST_InitTables_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitTables_Nominal");
	UtTest_Add(SEM_TEST_InitTables_RegisterErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitTables_RegisterErr");
	UtTest_Add(SEM_TEST_InitTables_ShareErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitTables_ShareErr");

	UtTest_Add(SEM_TEST_InitActionTable_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitActionTable_Nominal");
	UtTest_Add(SEM_TEST_InitActionTable_CDS, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitActionTable_CDS");
	UtTest_Add(SEM_TEST_InitActionTable_RegErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitActionTable_RegErr");
	UtTest_Add(SEM_TEST_InitActionTable_LoadErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitActionTable_LoadErr");

	UtTest_Add(SEM_TEST_InitScheduleTable_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitScheduleTable_Nominal");
	UtTest_Add(SEM_TEST_InitScheduleTable_CDS, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitScheduleTable_CDS");
	UtTest_Add(SEM_TEST_InitScheduleTable_RegErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitScheduleTable_RegErr");
	UtTest_Add(SEM_TEST_InitScheduleTable_LoadErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitScheduleTable_LoadErr");

	UtTest_Add(SEM_TEST_InitPlanFrmtTable_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitPlanFrmtTable_Nominal");
	UtTest_Add(SEM_TEST_InitPlanFrmtTable_CDS, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitPlanFrmtTable_CDS");
	UtTest_Add(SEM_TEST_InitPlanFrmtTable_RegErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitPlanFrmtTable_RegErr");
	UtTest_Add(SEM_TEST_InitPlanFrmtTable_LoadErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_InitPlanFrmtTable_LoadErr");


	/* Table Management */
	UtTest_Add(SEM_TABLES_ManageTables_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_Nominal");
	UtTest_Add(SEM_TABLES_ManageTables_ActionManageErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_ActionManageErr");
	UtTest_Add(SEM_TABLES_ManageTables_ScheduleValidate, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_ScheduleValidate");
	UtTest_Add(SEM_TABLES_ManageTables_ScheduleUpdate_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_ScheduleUpdate_Nominal");
	UtTest_Add(SEM_TABLES_ManageTables_ScheduleUpdate_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_ScheduleUpdate_Err");
	UtTest_Add(SEM_TABLES_ManageTables_ScheduleDump, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_ScheduleDump");
	UtTest_Add(SEM_TABLES_ManageTables_ScheduleErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_ManageTables_ScheduleErr");


	/* table verification */
	UtTest_Add(SEM_TABLES_VerifyActionTable_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyActionTable_Nominal");
	UtTest_Add(SEM_TABLES_VerifyActionTable_NumEntriesErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyActionTable_NumEntriesErr");
	UtTest_Add(SEM_TABLES_VerifyActionTable_BadCmd, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyActionTable_BadCmd");

	UtTest_Add(SEM_TABLES_VerifyScheduleTable_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyScheduleTable_Nominal");
	UtTest_Add(SEM_TABLES_VerifyScheduleTable_NumEntriesErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyScheduleTable_NumEntriesErr");
	UtTest_Add(SEM_TABLES_VerifyScheduleTable_InvalidActionErr, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyScheduleTable_InvalidActionErr");

	UtTest_Add(SEM_TABLES_VerifyPlanFrmtTbl_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyPlanFrmtTbl_Nominal");
	UtTest_Add(SEM_TABLES_VerifyPlanFrmtTbl_NoST, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyPlanFrmtTbl_NoST");
	UtTest_Add(SEM_TABLES_VerifyPlanFrmtTbl_NoAct, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_VerifyPlanFrmtTbl_NoAct");

	/* grab pointer tests... */
	UtTest_Add(SEM_TABLES_GrabScheduleTblPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabScheduleTblPtr_Nominal");
	UtTest_Add(SEM_TABLES_GrabScheduleTblPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabScheduleTblPtr_Err");

	UtTest_Add(SEM_TABLES_GrabActionTblPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabActionTblPtr_Nominal");
	UtTest_Add(SEM_TABLES_GrabActionTblPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabActionTblPtr_Err");

	UtTest_Add(SEM_TABLES_GrabPlanFrmtTblPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabPlanFrmtTblPtr_Nominal");
	UtTest_Add(SEM_TABLES_GrabPlanFrmtTblPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabPlanFrmtTblPtr_Err");

	UtTest_Add(SEM_TABLES_SEM_GrabSatStateFrmtPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabSatStateFrmtPtr_Nominal");
	UtTest_Add(SEM_TABLES_SEM_GrabSatStateFrmtPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GrabSatStateFrmtPtr_Err");

	/* drop pointer tests... */
	UtTest_Add(SEM_TABLES_DropScheduleTblPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropScheduleTblPtr_Nominal");
	UtTest_Add(SEM_TABLES_DropScheduleTblPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropScheduleTblPtr_Err");

	UtTest_Add(SEM_TABLES_DropActionTblPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropActionTblPtr_Nominal");
	UtTest_Add(SEM_TABLES_DropActionTblPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropActionTblPtr_Err");

	UtTest_Add(SEM_TABLES_DropPlanFrmtTblPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropPlanFrmtTblPtr_Nominal");
	UtTest_Add(SEM_TABLES_DropPlanFrmtTblPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropPlanFrmtTblPtr_Err");

	UtTest_Add(SEM_TABLES_SEM_DropSatStateFrmtPtr_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropSatStateFrmtPtr_Nominal");
	UtTest_Add(SEM_TABLES_SEM_DropSatStateFrmtPtr_Err, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_DropSatStateFrmtPtr_Err");

	/* Is Valid action id tests */
	UtTest_Add(SEM_TABLES_SEM_IsValidActionId_All, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_SEM_IsValidActionId_All");

	/* IsOverridePlan */
	UtTest_Add(SEM_TABLES_SEM_IsOverridePlan_Nominal, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_SEM_IsOverridePlan_Nominal");
	UtTest_Add(SEM_TABLES_SEM_IsOverridePlan_Override, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_SEM_IsOverridePlan_Override");

	/* GetScheduledAction Tests */
	UtTest_Add(SEM_TEST_GetScheduleAction_All, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TEST_GetScheduleAction_All");

	/* Get ActionTblEntry */
	UtTest_Add(SEM_TABLES_GetAction_All, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_GetAction_All");

	/* Set Sch Status */
	UtTest_Add(SEM_TABLES_SetSchStatus_All, SEM_Test_Setup, SEM_Test_TearDown, "SEM_TABLES_SetSchStatus_All");


} /* end SEM_TABLES_AddTestCases */


/****************
** END OF FILE **
****************/
