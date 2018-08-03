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
*	File: conds_table_test.c
*
*	Purpose: This file provides the implementation for all conds tables tests
*
*	Author: Daniel McGann
*
******************************************************************************/
#include "conds_test_utils.h"
#include "conds_table_test.h"

#include "conds_app.h"
#include "conds_table.h"
#include "conds_cmds.h"
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
 * Data to use
 */
CONDS_ConStateEntry_t ConStateTbl[2];
FRMT_TBL_t Type0;
FRMT_TBL_t Type1;
FRMT_TBL_Entry_t Type0_e[1];
FRMT_TBL_Entry_t Type1_e[1];

int32 UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    if (*TblPtr == g_CONDS_AppData.ConStatePtr) {
    	g_CONDS_AppData.ConStatePtr = (void*)&ConStateTbl;
    }
    else if (*TblPtr == g_CONDS_AppData.ConStatePtr[0].FrmtPtr) {
    	g_CONDS_AppData.ConStatePtr[0].FrmtPtr = (void*)&Type0;
    }
    else {
    	g_CONDS_AppData.ConStatePtr[1].FrmtPtr = (void*)&Type1;
    }

    return CFE_SUCCESS;
} /* end UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook1 */

int32 UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf (uint32** pointer, CFE_ES_MemHandle_t handle, uint32 size)
{
    *pointer = malloc(size);

    return size + 168;
} /* end UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf */

int32 UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf (CFE_ES_MemHandle_t handle, uint32* pointer)
{
    free(pointer);
    if(pointer == g_CONDS_AppData.ConStatePtr[0].DataPtr) {
    	g_CONDS_AppData.ConStatePtr[0].DataPtr = NULL;
    }
    else {
    	g_CONDS_AppData.ConStatePtr[1].DataPtr = NULL;
    }
    return 1;
} /* end UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf */



void UT_CONDS_InitData (void) {
	//Setup ConStateTable
	ConStateTbl[0].DataPtr = (uint32*)NULL;
	strcpy(ConStateTbl[0].FrmtFilename, "/cf/type0.tbl");
	strcpy(ConStateTbl[0].FrmtName, "TYPE0");
	ConStateTbl[0].FrmtPtr = NULL;
	ConStateTbl[0].MsgId = 0x1900;

	ConStateTbl[1].DataPtr = (uint32*)NULL;
	strcpy(ConStateTbl[1].FrmtFilename, "/cf/type1.tbl");
	strcpy(ConStateTbl[1].FrmtName, "TYPE1");
	ConStateTbl[1].FrmtPtr = NULL;
	ConStateTbl[1].MsgId = 0x1910;

	//Set up Type0 Table
	Type0_e[0].FieldId = 0;
	Type0_e[0].Flag = -999;
	Type0_e[0].Offset = 0;
	Type0_e[0].Size = 5;

	memcpy (Type0.Entries, Type0_e, FRMT_TBL_ENTRY_SIZE);
	Type0.NumEntries = 1;

	//Set up Type0 Table
	Type1_e[0].FieldId = 0;
	Type1_e[0].Flag = -999;
	Type1_e[0].Offset = 0;
	Type1_e[0].Size = 12;

	memcpy (Type1.Entries, Type1_e, FRMT_TBL_ENTRY_SIZE);
	Type1.NumEntries = 1;

	//Reset Counters
	g_CONDS_AppData.HkTlm.FrmtTblLoads = 0;

} /* end InitData() */




/******************************************************************************
* 	InitTable Tests
*/
void CONDS_TEST_InitTableNominal(void) {
	int32 Result;
	/* Set to prevent unintended error messages */
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	//Prevents erros during the Grab Pointers
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

    Result = CONDS_InitTables();
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end CONDS_TEST_InitTableNominal */



void CONDS_TEST_InitTableCDSLoad(void) {
	int32 Result;

    /* Set to generate both of the two error messages DS_INIT_TBL_CDS_EID  */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = CONDS_InitTables();


    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG, "Constellation State Table recovered from CDS"),
        		"Constellation State Table recovered from CDS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
} /* end CONDS_TEST_InitTableCDSLoad */



void CONDS_TEST_InitTableRegisterErr(void)
{
    int32 Result;

    /* Set to generate first instance of error message CONDS_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CONDS_INIT_TBL_ERR_EID, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = CONDS_InitTables();

    /* Verify results */
    UtAssert_True (Result == CONDS_INIT_TBL_ERR_EID, "Result == CONDS_INIT_TBL_ERR_EID");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR, "Unable to Register Constellation State Table, Error = 0x0000003B"),
        "Unable to Register Constellation State Table, Error = 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_InitTableRegisterErr */


void CONDS_TEST_InitTableLoadErr(void)
{
    int32 Result;

    /* Set to generate error message error message */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 123, 1);

    /* Execute the function being tested */
    Result = CONDS_InitTables();

    /* Verify results */
    UtAssert_True (Result == 123, "Result == 123");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                "Unable to load default Constellation State table from File: /cf/conds_state.tbl, Error: 0x0000007B"),
                "Unable to load default Constellation State table from File: /cf/conds_state.tbl, Error: 0x0000007B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_InitTableLoadErr */

/******************************************************************************
* 	InitFrmtTable Tests
*/
void CONDS_TEST_InitFrmtTablesNominal(void) {
	int32 Result;

	UT_CONDS_InitData();

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	// Register both format tables correctly
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);


    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


    Result = CONDS_InitFrmtTables();

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "ConState Table saved properly");

    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* CONDS_TEST_InitFrmtTablesNominal */



void CONDS_TEST_InitFrmtTablesRegister0Err(void) {
	int32 Result;

	UT_CONDS_InitData();

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	// Register type 0 tablel in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CONDS_INIT_TBL_ERR_EID, 1);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


    Result = CONDS_InitFrmtTables();

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "ConState Table saved properly");

    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
            (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                    "Unable to Register Frmt Tbl #0 | Error: 0x0000003B"),
                    "Unable to Register Frmt Tbl #0 | Error: 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* CONDS_TEST_InitFrmtTablesRegister0Err */



void CONDS_TEST_InitFrmtTablesRegister1Err(void) {
	int32 Result;

	UT_CONDS_InitData();

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	// Register type 1 table in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CONDS_INIT_TBL_ERR_EID, 2);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


    Result = CONDS_InitFrmtTables();

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "ConState Table saved properly");

    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
            (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                    "Unable to Register Frmt Tbl #1 | Error: 0x0000003B"),
                    "Unable to Register Frmt Tbl #1 | Error: 0x0000003B");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    UtAssert_True (g_CONDS_AppData.FrmtTablesInit == FALSE, "g_CONDS_AppData.FrmtTablesInit == FALSE");
} /* CONDS_TEST_InitFrmtTablesRegister1Err */




void CONDS_TEST_InitFrmtTablesCDS(void) {
	int32 Result;

	UT_CONDS_InitData();

	//Prevents erros during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	// Register both format tables correctly
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);


    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


    Result = CONDS_InitFrmtTables();


    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
                (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG,
                        "Load from CDS Frmt Tbl #0 |  File: /cf/type0.tbl"),
                        "Load from CDS Frmt Tbl #0 |  File: /cf/type0.tbl");

    UtAssert_True
                (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG,
                        "Load from CDS Frmt Tbl #1 |  File: /cf/type1.tbl"),
                        "Load from CDS Frmt Tbl #1 |  File: /cf/type1.tbl");


    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* CONDS_TEST_InitFrmtTablesCDS */



void CONDS_TEST_InitFrmtTablesLoadErr(void) {
	int32 Result;

	UT_CONDS_InitData();

	//Prevents errors during the Grab Pointers
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	// Register type 1 table in correct
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);

    //Load Both Tables correctly
    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 0x99, 1);


    Result = CONDS_InitFrmtTables();

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "ConState Table saved properly");

    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
            (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                    "Unable to load default Frmt Tbl #0 |  File: /cf/type0.tbl, Error: 0x00000099"),
                    "Unable to load default Frmt Tbl #0 |  File: /cf/type0.tbl, Error: 0x00000099");
    UtAssert_True
                (Ut_CFE_EVS_EventSent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                        "Unable to load default Frmt Tbl #1 |  File: /cf/type1.tbl, Error: 0x00000099"),
                        "Unable to load default Frmt Tbl #1 |  File: /cf/type1.tbl, Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* CONDS_TEST_InitFrmtTablesLoadErr */

/******************************************************************************
* 	GrabPtrs Tests
*/
void  CONDS_TEST_GrabPtrs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	UT_CONDS_InitData();

	Result = CONDS_GrabPtrs();

	/* Verify results */
	    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

		UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "*ConStatePtr = ConStateTable");
		UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr[0].FrmtPtr, &Type0, (sizeof(Type0) + (FRMT_TBL_ENTRY_SIZE * 1)), "*ConStatePtr[0].FrmtPtr = Type0");
		UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr[1].FrmtPtr, &Type1, (sizeof(Type1) + (FRMT_TBL_ENTRY_SIZE * 1)), "*ConStatePtr[1].FrmtPtr = Type1");

} /* end CONDS_TEST_GrabPtrs_Nominal */


void  CONDS_TEST_GrabPtrs_ConStateErr(void) {
	int32 Result;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);
    UT_CONDS_InitData();

	Result = CONDS_GrabPtrs();

	/* Verify results */
	    UtAssert_True (Result == 0x99, "Result == 0x99");
		UtAssert_True
			(Ut_CFE_EVS_EventSent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
				"CONDS: Could not grab pointers for ConState Table, Error: 0x00000099"),
				"CONDS: Could not grab pointers for ConState Table, Error: 0x00000099");
		UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_GrabPtrs_ConStateErr */



void  CONDS_TEST_GrabPtrs_Frmt0Err(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 2);
	UT_CONDS_InitData();
	Result = CONDS_GrabPtrs();

	/* Verify results */
	    UtAssert_True (Result == 0x99, "Result == 0x99");
		UtAssert_True
			(Ut_CFE_EVS_EventSent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
				"CONDS: Could not grab pointers for Frmt Table, Index: 0  Error: 0x00000099"),
				"CONDS: Could not grab pointers for Frmt Table, Index: 0  Error: 0x00000099");

		UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_GrabPtrs_Frmt0Err */


void  CONDS_TEST_GrabPtrs_Frmt1Err(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 3);
	UT_CONDS_InitData();

	Result = CONDS_GrabPtrs();

	/* Verify results */
    UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not grab pointers for Frmt Table, Index: 1  Error: 0x00000099"),
			"CONDS: Could not grab pointers for Frmt Table, Index: 1  Error: 0x00000099");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_GrabPtrs_Frmt1Err */


/******************************************************************************
* 	DropPtrs Tests
*/
void  CONDS_TEST_DropPtrs_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	UT_CONDS_InitData();

	CONDS_GrabPtrs(); //Already Tested

	Result = CONDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True(g_CONDS_AppData.ConStatePtr == NULL, "g_CONDS_AppData.ConStatePtr == NULL");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_DropPtrs_Nominal */


void  CONDS_TEST_DropPtrs_Frmt0Err(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 1);
	UT_CONDS_InitData();

	CONDS_GrabPtrs(); //Already Tested

	Result = CONDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "*ConStatePtr = ConStateTable");
	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr[0].FrmtPtr, &Type0, sizeof(Type0), "*ConStatePtr[0].FrmtPtr = Type0");
	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr[1].FrmtPtr, &Type1, sizeof(Type1), "*ConStatePtr[1].FrmtPtr = Type1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not release pointer for Frmt Table, Index: 0  Error: 0x00000099"),
			"CONDS: Could not release pointer for Frmt Table, Index: 0  Error: 0x00000099");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_DropPtrs_Frmt0Err */


void  CONDS_TEST_DropPtrs_Frmt1Err(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 2);
	UT_CONDS_InitData();

	CONDS_GrabPtrs(); //Already Tested

	Result = CONDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "*ConStatePtr = ConStateTable");
	UtAssert_True(g_CONDS_AppData.ConStatePtr[0].FrmtPtr == NULL, "g_CONDS_AppData.ConStatePtr[0].FrmtPtr == NULL");
	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr[1].FrmtPtr, &Type1, sizeof(Type1), "*ConStatePtr[1].FrmtPtr = Type1");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not release pointer for Frmt Table, Index: 1  Error: 0x00000099"),
			"CONDS: Could not release pointer for Frmt Table, Index: 1  Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_DropPtrs_Frmt1Err */

void  CONDS_TEST_DropPtrs_ConStateErr(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, 0x99, 3);
	UT_CONDS_InitData();

	CONDS_GrabPtrs(); //Already Tested

	Result = CONDS_DropPtrs();

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");

	UtAssert_MemCmp(g_CONDS_AppData.ConStatePtr, &ConStateTbl, sizeof(ConStateTbl), "*ConStatePtr = ConStateTable");
	UtAssert_True(g_CONDS_AppData.ConStatePtr[0].FrmtPtr == NULL, "g_CONDS_AppData.ConStatePtr[0].FrmtPtr == NULL");
	UtAssert_True(g_CONDS_AppData.ConStatePtr[1].FrmtPtr == NULL, "g_CONDS_AppData.ConStatePtr[1].FrmtPtr == NULL");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not release pointer for ConState Table, Error: 0x00000099"),
			"CONDS: Could not release pointer for ConState Table, Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_DropPtrs_ConStateErr */

/******************************************************************************
* 	SubscribeStateMsg Tests
*/
void CONDS_TEST_SubStateMsg_Nominal(void) {
	int32 Result;
	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = CONDS_SubscribeStateMsgs();

	/* Verify results */
		UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
		UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_SubStateMsg_Nominal */


void CONDS_TEST_SubStateMsg_Type0Err(void) {
	int32 Result;

	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, 0x99, 1);

	Result = CONDS_SubscribeStateMsgs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True
			(Ut_CFE_EVS_EventSent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
				"Unable to Subscribe to Constellation State message | MsgId: 0x00001900, Error: 0x00000099"),
				"Unable to Subscribe to Constellation State message | MsgId: 0x00001900, Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_SubStateMsg_Type0Err */

void CONDS_TEST_SubStateMsg_Type1Err(void) {
	int32 Result;

	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, 0x99, 2);

	Result = CONDS_SubscribeStateMsgs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True
			(Ut_CFE_EVS_EventSent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
				"Unable to Subscribe to Constellation State message | MsgId: 0x00001910, Error: 0x00000099"),
				"Unable to Subscribe to Constellation State message | MsgId: 0x00001910, Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_SubStateMsg_Type1Err */

/******************************************************************************
* 	UnsubscribeStateMsgs Tests
*/
/******************************************************************************
* 	SubscribeStateMsg Tests
*/
void CONDS_TEST_UnSubStateMsg_Nominal(void) {
	int32 Result;

	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	Result = CONDS_SubscribeStateMsgs();

	/* Verify results */
		UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
		UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_UnSubStateMsg_Nominal */


void CONDS_TEST_UnSubStateMsg_Type0Err(void) {
	int32 Result;

	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, 0x99, 1);

	Result = CONDS_UnSubscribeStateMsgs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True
			(Ut_CFE_EVS_EventSent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
				"Unable to Unsubscribe to Constellation State message | MsgId: 0x00001900, Error: 0x00000099"),
				"Unable to Unsubscribe to Constellation State message | MsgId: 0x00001900, Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_UnSubStateMsg_Type0Err */

void CONDS_TEST_UnSubStateMsg_Type1Err(void) {
	int32 Result;

	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, 0x99, 2);

	Result = CONDS_UnSubscribeStateMsgs();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True
			(Ut_CFE_EVS_EventSent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
				"Unable to Unsubscribe to Constellation State message | MsgId: 0x00001910, Error: 0x00000099"),
				"Unable to Unsubscribe to Constellation State message | MsgId: 0x00001910, Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end CONDS_TEST_UnSubStateMsg_Type1Err */


/******************************************************************************
* 	AllocateStateMemory Tests
*/
void CONDS_TEST_AllocStateMem_Nominal(void) {
	int32 Result;
	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	Result = CONDS_AllocateStateMemory();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (g_CONDS_AppData.ConStatePtr[0].DataPtr != NULL, "[0].DataPtr != NULL");
	UtAssert_True (g_CONDS_AppData.ConStatePtr[1].DataPtr != NULL, "[1].DataPtr != NULL");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_AllocStateMem_Nominal */

//ADDME Error Tests
/******************************************************************************
* 	FreeStateMemory Tests
*/
void CONDS_TEST_FreeStateMem_Nominal(void) {
	int32 Result;
	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	CONDS_AllocateStateMemory();
	Result = CONDS_FreeStateMemory();

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (g_CONDS_AppData.ConStatePtr[0].DataPtr == NULL, "[0].DataPtr == NULL");
	UtAssert_True (g_CONDS_AppData.ConStatePtr[1].DataPtr == NULL, "[1].DataPtr == NULL");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end CONDS_TEST_FreeStateMem_Nominal */

//ADDME Error Tests
/******************************************************************************
* 	VerifyConState Tests
*/
void CONDS_TEST_VerifyConState_Nominal(void) {
	int32 Result;
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);

	CONDS_GrabPtrs(); //Tested

	Result = CONDS_VerifyConState((void*)g_CONDS_AppData.ConStatePtr);
	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* CONDS_TEST_VerifyConState_Nominal */

void CONDS_TEST_VerifyConState_InvalidTblName(void) {
	int32 Result;
	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);

	CONDS_GrabPtrs(); //Tested

	strcpy(g_CONDS_AppData.ConStatePtr[0].FrmtName, "%^(&)*^");

	Result = CONDS_VerifyConState((void*)g_CONDS_AppData.ConStatePtr);
	/* Verify results */
	UtAssert_True (Result == CONDS_TABLE_VERIFY_ERR_EID, "Result == CONDS_TABLE_VERIFY_ERR_EID");
	UtAssert_True
				(Ut_CFE_EVS_EventSent(CONDS_TABLE_VERIFY_ERR_EID, CFE_EVS_ERROR,
					"Invalid Format Table Name | Index: 0"),
					"Invalid Format Table Name | Index: 0");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* CONDS_TEST_VerifyConState_InvalidTblName */

void CONDS_TEST_VerifyConState_InvalidTblName2(void) {
	int32 Result;
	UT_CONDS_InitData();	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);


	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);

	CONDS_GrabPtrs(); //Tested

	strcpy(g_CONDS_AppData.ConStatePtr[1].FrmtName, "hello world");

	Result = CONDS_VerifyConState((void*)g_CONDS_AppData.ConStatePtr);
	/* Verify results */
	UtAssert_True (Result == CONDS_TABLE_VERIFY_ERR_EID, "Result == CONDS_TABLE_VERIFY_ERR_EID");
	UtAssert_True
				(Ut_CFE_EVS_EventSent(CONDS_TABLE_VERIFY_ERR_EID, CFE_EVS_ERROR,
					"Invalid Format Table Name | Index: 1"),
					"Invalid Format Table Name | Index: 1");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* CONDS_TEST_VerifyConState_InvalidTblName2 */

void CONDS_TEST_VerifyConState_InvalidFileName(void) {
	int32 Result;
	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);

	CONDS_GrabPtrs(); //Tested

	strcpy(g_CONDS_AppData.ConStatePtr[0].FrmtFilename, "/cf/*asdf.tbl");

	Result = CONDS_VerifyConState((void*)g_CONDS_AppData.ConStatePtr);
	/* Verify results */
	UtAssert_True (Result == CONDS_TABLE_VERIFY_ERR_EID, "Result == CONDS_TABLE_VERIFY_ERR_EID");
	UtAssert_True
				(Ut_CFE_EVS_EventSent(CONDS_TABLE_VERIFY_ERR_EID, CFE_EVS_ERROR,
					"Invalid Format Table Filename | Index: 0"),
					"Invalid Format Table Filename | Index: 0");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* CONDS_TEST_VerifyConState_InvalidFileName */

void CONDS_TEST_VerifyConState_InvalidFileName2(void) {
	int32 Result;
	UT_CONDS_InitData();

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);

	CONDS_GrabPtrs(); //Tested

	strcpy(g_CONDS_AppData.ConStatePtr[1].FrmtFilename, "/cf/a sdf.tbl");

	Result = CONDS_VerifyConState((void*)g_CONDS_AppData.ConStatePtr);
	/* Verify results */
	UtAssert_True (Result == CONDS_TABLE_VERIFY_ERR_EID, "Result == CONDS_TABLE_VERIFY_ERR_EID");
	UtAssert_True
				(Ut_CFE_EVS_EventSent(CONDS_TABLE_VERIFY_ERR_EID, CFE_EVS_ERROR,
					"Invalid Format Table Filename | Index: 1"),
					"Invalid Format Table Filename | Index: 1");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* CONDS_TEST_VerifyConState_InvalidFileName2 */
/******************************************************************************
* 	TableManageFrmts Tests
*/

void CONDS_TEST_ManageFrmtTables_Nominal(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end CONDS_TEST_ManageFrmtTables_Nominal */

void CONDS_TEST_ManageFrmtTables_DumpNominal(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
			"CONDS: Format Table Dumped to shared buffer"),
			"CONDS: Format Table Dumped to shared buffer");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageFrmtTables_DumpNominal */

void CONDS_TEST_ManageFrmtTables_DumpErr(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_DUMPTOBUFFER_INDEX, 0x99, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Error during Format Table Dumped to shared buffer | Error: 0x00000099"),
			"CONDS: Error during Format Table Dumped to shared buffer | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageFrmtTables_DumpErr */

void CONDS_TEST_ManageFrmtTables_ValidateNominal(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end CONDS_TEST_ManageFrmtTables_ValidateNominal */

void CONDS_TEST_ManageFrmtTables_ValidateErr(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_VALIDATE_INDEX, 0x99, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/*/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Error during Format Table Validation | Error: 0x00000099"),
			"CONDS: Error during Format Table Validation | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageFrmtTables_ValidateErr */


void CONDS_TEST_ManageFrmtTables_UpdateNominal(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
	UtAssert_True (g_CONDS_AppData.HkTlm.FrmtTblLoads == 1, "HkTlm.FrmtTblLoads == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
			"CONDS: Format Table Updated"),
			"CONDS: Format Table Updated");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageFrmtTables_UpdateNominal */

void CONDS_TEST_ManageFrmtTables_UpdateErr(void) {
	int32 Result;

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_UPDATE_INDEX, 0x99, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
	Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();

	Result = CONDS_TableManageFrmt(g_CONDS_AppData.ConStatePtr[0].FrmtHandle);

	/* Verify results */
	UtAssert_True (Result == 0x99, "Result == 0x99");
	UtAssert_True (g_CONDS_AppData.HkTlm.FrmtTblLoads == 0, "HkTlm.FrmtTblLoads == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Error during Format Table Update | Error: 0x00000099"),
			"CONDS: Error during Format Table Update | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageFrmtTables_UpdateErr */

/******************************************************************************
* 	TableManageConState Tests
*/
void CONDS_TEST_ManageConState_Nominal(void) {


	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SUCCESS, 1);


	CONDS_TableManageConState();

	/* Verify results */

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end CONDS_TEST_ManageConState_Nominal */

void CONDS_TEST_ManageConState_DumpNominal(void) {

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);

	CONDS_TableManageConState();

	/* Verify results */
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
			"CONDS: ConState Table Dumped to shared buffer"),
			"CONDS: ConState Table Dumped to shared buffer");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageConState_DumpNominal */

void CONDS_TEST_ManageConState_DumpErr(void) {

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_DUMPTOBUFFER_INDEX, 0x99, 1);

	CONDS_TableManageConState();

	/* Verify results */
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Error during ConState Table Dumped to shared buffer | Error: 0x00000099"),
			"CONDS: Error during ConState Table Dumped to shared buffer | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageConState_DumpErr */

void CONDS_TEST_ManageConState_ValidateNominal(void) {

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);

	CONDS_TableManageConState();

	/* Verify results */
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end CONDS_TEST_ManageConState_ValidateNominal */

void CONDS_TEST_ManageConState_ValidateErr(void) {

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_VALIDATE_INDEX, 0x99, 1);

	CONDS_TableManageConState();

	/*/* Verify results */
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Error during ConState Table Validation | Error: 0x00000099"),
			"CONDS: Error during ConState Table Validation | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageConState_ValidateErr */


void CONDS_TEST_ManageConState_UpdateNominal(void) {

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


	CONDS_TableManageConState();

	/* Verify results */
	UtAssert_True (g_CONDS_AppData.HkTlm.ConStateTblLoads == 1, "HkTlm.FrmtTblLoads == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
			"CONDS: ConState Table Updated"),
			"CONDS: ConState Table Updated");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageConState_UpdateNominal */

void CONDS_TEST_ManageConState_UpdateErr(void) {

	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);
	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_UPDATE_INDEX, 0x99, 1);

	CONDS_TableManageConState();

	/* Verify results */
	UtAssert_True (g_CONDS_AppData.HkTlm.FrmtTblLoads == 0, "HkTlm.FrmtTblLoads == 1");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Error during ConState Table Update | Error: 0x00000099"),
			"CONDS: Error during ConState Table Update | Error: 0x00000099");
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end CONDS_TEST_ManageConState_UpdateErr */

/******************************************************************************
* 	IsValidMsgId Tests
*/
void CONDS_TEST_IsValidMsgId(void) {
	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);

	UtAssert_True(CONDS_IsValidStateMsgId((CFE_SB_MsgId_t)0x1900) == TRUE, "0x00 is Valid" );
	UtAssert_True(CONDS_IsValidStateMsgId((CFE_SB_MsgId_t)0x1910) == TRUE, "0x01 is Valid" );
	UtAssert_True(CONDS_IsValidStateMsgId((CFE_SB_MsgId_t)0x3465) == FALSE, "0x4365 is NOTValid" );
	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* CONDS_TEST_IsValidMsgId_True */

/******************************************************************************
* 	SaveStateData Tests
*/

typedef struct {
	uint8  ucTlmHeader[CFE_SB_CMD_HDR_SIZE];
	char data[8];
} TYPE0_t;

void CONDS_TEST_SaveStateData_Nominal(void) {
	int32 Result;
	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
	CONDS_InitTables();
	CONDS_InitFrmtTables();
	CONDS_FreeStateMemory();
	CONDS_AllocateStateMemory();

	// Create a State message Id
	TYPE0_t msg;
	CFE_SB_InitMsg(&msg, 0x1900, sizeof(TYPE0_t), FALSE);
	char data[5] = "Pies";
	memcpy(CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&msg), data, 5);

	Result = CONDS_SaveStateData((CFE_SB_MsgPtr_t)&msg);

	/* Verify results */
	UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

	UtAssert_MemCmp(CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&msg), g_CONDS_AppData.ConStatePtr[0].DataPtr, 8, "Msg Data == ConStatePtr[0].DataPtr");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

	CONDS_FreeStateMemory();
} /*CONDS_TEST_SaveStateData_Nominal */

void CONDS_TEST_SaveStateData_NullPtr(void) {
	int32 Result;
	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);


	CONDS_InitTables();
	CONDS_InitFrmtTables();
	CONDS_FreeStateMemory();

	// Create a State message Id
	TYPE0_t msg;
	CFE_SB_InitMsg(&msg, 0x1900, sizeof(TYPE0_t), FALSE);
	char data[5] = "Pies";
	memcpy(CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&msg), data, 5);

	Result = CONDS_SaveStateData((CFE_SB_MsgPtr_t)&msg);

	/* Verify results */
	UtAssert_True (Result == CONDS_SAVEDATA_ERR_EID, "Result == CONDS_SAVEDATA_ERR_EID");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Data Received for Pointer not Initialized | MsgId: 0x00001900 Error: 0x0000003E"),
			"CONDS: Data Received for Pointer not Initialized | MsgId: 0x00001900 Error: 0x0000003E");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Data Save Could not save data to 1 locations - See Error Log for Details"),
			"CONDS: Data Save Could not save data to 1 locations - See Error Log for Details");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /*CONDS_TEST_SaveStateData_NullPtr */

void CONDS_TEST_SaveStateData_MisSize(void) {
	int32 Result;
	UT_CONDS_InitData();
	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_CONDS_TABLE_TEST_CFE_TBL_GetAddressHook1); // @suppress("Symbol is not resolved")
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_CONDS_TABLE_TEST_CFE_ES_PutPoolBuf);

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

	CONDS_InitTables();
	CONDS_InitFrmtTables();
	CONDS_FreeStateMemory();
	CONDS_AllocateStateMemory();

	// Create a State message Id
	TYPE0_t msg;
	CFE_SB_InitMsg(&msg, 0x1900, sizeof(TYPE0_t) + 2, FALSE);
	char data[10] = "Pies";
	memcpy(CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&msg), data, 5);

	Result = CONDS_SaveStateData((CFE_SB_MsgPtr_t)&msg);

	/* Verify results */
	UtAssert_True (Result == CONDS_SAVEDATA_ERR_EID, "Result == CONDS_SAVEDATA_ERR_EID");
	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Save Data size mismatch Expect: 8 Actual: 10 | MsgId: 0x00001900 Error: 0x0000003E"),
			"CONDS: Save Data size mismatch Expect: 8 Actual: 10 | MsgId: 0x00001900 Error: 0x0000003E");

	UtAssert_True
		(Ut_CFE_EVS_EventSent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Data Save Could not save data to 1 locations - See Error Log for Details"),
			"CONDS: Data Save Could not save data to 1 locations - See Error Log for Details");

	UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

	CONDS_FreeStateMemory();
} /*CONDS_TEST_SaveStateData_MisSize */

/******************************************************************************
* 	ADD TESTCASES
*/
void CONDS_TABLE_AddTestCases(void) {
    UtTest_Add(CONDS_TEST_InitTableNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "InitTableNominal");
    UtTest_Add(CONDS_TEST_InitTableCDSLoad, CONDS_Test_Setup, CONDS_Test_TearDown, "InitTableCDSLoad");
    UtTest_Add(CONDS_TEST_InitTableRegisterErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitTableRegisterErr");
    UtTest_Add(CONDS_TEST_InitTableLoadErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitTableLoadErr");
    UtTest_Add(CONDS_TEST_InitFrmtTablesNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitFrmtTablesNominal");
    UtTest_Add(CONDS_TEST_InitFrmtTablesRegister0Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitFrmtTablesRegister0Err");
    UtTest_Add(CONDS_TEST_InitFrmtTablesRegister1Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitFrmtTablesRegister1Err");
    UtTest_Add(CONDS_TEST_InitFrmtTablesCDS, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitFrmtTablesCDS");
    UtTest_Add(CONDS_TEST_InitFrmtTablesLoadErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_InitFrmtTablesLoadErr");
    UtTest_Add(CONDS_TEST_GrabPtrs_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_GrabPtrs_Nominal");
    UtTest_Add(CONDS_TEST_GrabPtrs_ConStateErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_GrabPtrs_ConStateErr");
    UtTest_Add(CONDS_TEST_GrabPtrs_Frmt0Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_GrabPtrs_Frmt0Err");
    UtTest_Add(CONDS_TEST_GrabPtrs_Frmt1Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_GrabPtrs_Frmt1Err");
    UtTest_Add(CONDS_TEST_DropPtrs_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_DropPtrs_Nominal");
    UtTest_Add(CONDS_TEST_DropPtrs_Frmt0Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_DropPtrs_Frmt0Err");
    UtTest_Add(CONDS_TEST_DropPtrs_Frmt1Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_DropPtrs_Frmt1Err");
    UtTest_Add(CONDS_TEST_DropPtrs_ConStateErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_DropPtrs_ConStateErr");
    UtTest_Add(CONDS_TEST_SubStateMsg_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_SubStateMsg_Nominal");
    UtTest_Add(CONDS_TEST_SubStateMsg_Type0Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_SubStateMsg_Type0Err");
    UtTest_Add(CONDS_TEST_SubStateMsg_Type1Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_SubStateMsg_Type1Err");
    UtTest_Add(CONDS_TEST_UnSubStateMsg_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_UnSubStateMsg_Nominal");
    UtTest_Add(CONDS_TEST_UnSubStateMsg_Type0Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_UnSubStateMsg_Type0Err");
    UtTest_Add(CONDS_TEST_UnSubStateMsg_Type1Err, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_UnSubStateMsg_Type1Err");
    UtTest_Add(CONDS_TEST_AllocStateMem_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_AllocStateMem_Nominal");
    UtTest_Add(CONDS_TEST_FreeStateMem_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_FreeStateMem_Nominal");
    UtTest_Add(CONDS_TEST_VerifyConState_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_VerifyConState_Nominal");
    UtTest_Add(CONDS_TEST_VerifyConState_InvalidTblName, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_VerifyConState_InvalidTblName");
    UtTest_Add(CONDS_TEST_VerifyConState_InvalidTblName2, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_VerifyConState_InvalidTblName2");
    UtTest_Add(CONDS_TEST_VerifyConState_InvalidFileName, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_VerifyConState_InvalidFileName");
    UtTest_Add(CONDS_TEST_VerifyConState_InvalidFileName2, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_VerifyConState_InvalidFileName2");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_Nominal");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_DumpNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_DumpNominal");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_DumpErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_DumpErr");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_ValidateNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_ValidateNominal");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_ValidateErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_ValidateErr");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_UpdateNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_UpdateNominal");
    UtTest_Add(CONDS_TEST_ManageFrmtTables_UpdateErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageFrmtTables_UpdateErr");
    UtTest_Add(CONDS_TEST_ManageConState_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_Nominal");
    UtTest_Add(CONDS_TEST_ManageConState_DumpNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_DumpNominal");
    UtTest_Add(CONDS_TEST_ManageConState_DumpErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_DumpErr");
  	UtTest_Add(CONDS_TEST_ManageConState_ValidateNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_ValidateNominal");
  	UtTest_Add(CONDS_TEST_ManageConState_ValidateErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_ValidateErr");
  	UtTest_Add(CONDS_TEST_ManageConState_UpdateNominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_UpdateNominal");
  	UtTest_Add(CONDS_TEST_ManageConState_UpdateErr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_ManageConState_UpdateErr");
  	UtTest_Add(CONDS_TEST_IsValidMsgId, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_IsValidMsgId");
 	UtTest_Add(CONDS_TEST_SaveStateData_Nominal, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_SaveStateData_Nominal");
  	UtTest_Add(CONDS_TEST_SaveStateData_NullPtr, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_SaveStateData_NullPtr");
  	UtTest_Add(CONDS_TEST_SaveStateData_MisSize, CONDS_Test_Setup, CONDS_Test_TearDown, "CONDS_TEST_SaveStateData_MisSize");


} /* End AddTestCases */
