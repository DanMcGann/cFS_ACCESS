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
*	File: sa_cmds_test.c
*
*	Purpose: This file provides the implementation for all SA Cmds tests
*
*	Author: Daniel McGann
*
******************************************************************************/
#include "sa_test_utils.h"
#include "sa_cmds_test.h"

#include "sa_app.h"
#include "sa_tables.h"
#include "sa_cmds.h"
#include "sa_private_types.h"
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
} SA_CMD_TEST_SatelliteState_t;

typedef struct {
	 uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];
	 SA_CMD_TEST_SatelliteState_t data;
} SA_TEST_SatelliteStateMsg_t;

SA_CMD_TEST_SatelliteState_t SA_CMD_TEST_SatelliteState;
FRMT_TBL_t* CmdSatStateTbl;
FRMT_TBL_Entry_t CmdSatStateTbl_e[2] = {
		{0, 0, 4, 0x1900},
		{1, 4, 4, 0x1901}
};

void SA_CMD_TEST_initData(void) {
	CmdSatStateTbl = malloc((sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE *2));
	memcpy(CmdSatStateTbl->Entries,CmdSatStateTbl_e,  FRMT_TBL_ENTRY_SIZE *2);
	CmdSatStateTbl->NumEntries = 2;

	SA_CMD_TEST_SatelliteState.DS = 1000;
	SA_CMD_TEST_SatelliteState.ES = 100;

	g_SA_AppData.SatFrmtEntries = 2;
}

int32 UT_SA_CMD_TEST_CFE_TBL_GetAddressHook1 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    g_SA_AppData.SatFrmtPtr = (void*)CmdSatStateTbl;
    return CFE_SUCCESS;
} /* end UT_SA_TABLE_TEST_CFE_TBL_GetAddressHook1 */

int32 UT_SA_CMD_TEST_CFE_ES_GetPoolBuf (uint32** pointer, CFE_ES_MemHandle_t handle, uint32 size)
{
    *pointer = malloc(size);
    g_SA_AppData.SatStatePtr = (char* )pointer;

    memset(*pointer, 0x00, size);

    return size + 168;
} /* end UT_SA_TABLE_TEST_CFE_ES_GetPoolBuf */

int32 UT_SA_CMD_TEST_CFE_ES_PutPoolBuf (CFE_ES_MemHandle_t handle, uint32* pointer)
{
    free(pointer);
    g_SA_AppData.SatStatePtr = NULL;
    return 168;
} /* end UT_SA_TABLE_TEST_CFE_ES_PutPoolBuf */

int32 UT_SA_CMD_CMD_CFE_SB_SendMsg(CFE_SB_MsgPtr_t MsgPtr) {

	SA_CMD_TEST_SatelliteState_t * data = (SA_CMD_TEST_SatelliteState_t*) (CFE_SB_GetUserData(MsgPtr));


	CFE_EVS_SendEvent(1, CFE_EVS_INFORMATION,
			"OutData Sent: SA_TEST_SatelliteState_t.DS = %d, SA_TEST_SatelliteState_t.ES = %d",
			data->DS, data->ES);

	return CFE_SUCCESS;
}

/******************************************************************************
* 	SA_SatStateDataReqCmd Tests
*/
void SA_TEST_SA_SatStateDataReqCmd_Nominal(void) {

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_CMD_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_SA_CMD_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_SA_CMD_TEST_CFE_ES_PutPoolBuf);
	Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &UT_SA_CMD_CMD_CFE_SB_SendMsg);

	SA_CMD_TEST_initData();

	SA_NoArgCmd_t cmdMsg;
	CFE_SB_InitMsg(&cmdMsg, 0x00, CFE_SB_CMD_HDR_SIZE, FALSE);
	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdMsg, 3);

	g_SA_AppData.SatStatePtr = (char*) &SA_CMD_TEST_SatelliteState;

	SA_SatStateDataReqCmd((CFE_SB_MsgPtr_t)&cmdMsg);

	/* verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

    UtAssert_True
   	   (Ut_CFE_EVS_EventSent(SA_CMD_INF_EID, CFE_EVS_INFORMATION,
   		   "SA - Recvd SatStateDataReq cmd (0)"),
   		   "SA - Recvd SatStateDataReq cmd (0)");

    UtAssert_True
	   (Ut_CFE_EVS_EventSent(1, CFE_EVS_INFORMATION,
		   "OutData Sent: SA_TEST_SatelliteState_t.DS = 1000, SA_TEST_SatelliteState_t.ES = 100"),
		   "OutData Sent: SA_TEST_SatelliteState_t.DS = 1000, SA_TEST_SatelliteState_t.ES = 100");

    //No modifications to saved data
    UtAssert_True (((SA_CMD_TEST_SatelliteState_t*) g_SA_AppData.SatStatePtr)->DS == 1000,
    		"g_SA_AppData.SatStatePtr)->DS == 1000");
    UtAssert_True (((SA_CMD_TEST_SatelliteState_t*) g_SA_AppData.SatStatePtr)->ES == 100,
    		"g_SA_AppData.SatStatePtr)->ES == 100");

} /* SA_SatStateDataReqCmd */

void SA_TEST_SA_SatStateDataReqCmd_NoPtr(void) {

	Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 0x99, 1);


	SA_NoArgCmd_t cmdMsg;
	CFE_SB_InitMsg(&cmdMsg, 0x00, CFE_SB_CMD_HDR_SIZE, FALSE);
	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdMsg, 3);

	g_SA_AppData.SatStatePtr = (char*) &SA_CMD_TEST_SatelliteState;

	SA_SatStateDataReqCmd((CFE_SB_MsgPtr_t)&cmdMsg);
	UtAssert_True
		   (Ut_CFE_EVS_EventSent(SA_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			   "SA: Could not grab pointers for SatFrmt Table, Error: 0x00000099"),
			   "SA: Could not grab pointers for SatFrmt Table, Error: 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* SA_TEST_SA_SatStateDataReqCmd_NoPtr */

void SA_TEST_SA_SatStateDataReqCmd_InvalidMsg(void) {

	Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_SA_CMD_TEST_CFE_TBL_GetAddressHook1);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &UT_SA_CMD_TEST_CFE_ES_GetPoolBuf);
	Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &UT_SA_CMD_TEST_CFE_ES_PutPoolBuf);
	Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &UT_SA_CMD_CMD_CFE_SB_SendMsg);


	char cmdMsg[100];
	CFE_SB_InitMsg(&cmdMsg, 0x00, 100, FALSE);
	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdMsg, 3);

	g_SA_AppData.SatStatePtr = (char*) &SA_CMD_TEST_SatelliteState;

	SA_SatStateDataReqCmd((CFE_SB_MsgPtr_t)&cmdMsg);

	UtAssert_True
		   (Ut_CFE_EVS_EventSent(SA_MSGLEN_ERR_EID, CFE_EVS_ERROR,
               "SA - Rcvd invalid msgLen: msgId=0x00000000, cmdCode=0, msgLen=100, expectedLen=8"),
			   "SA - Rcvd invalid msgLen: msgId=0x00000000, cmdCode=0, msgLen=100, expectedLen=8");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* SA_TEST_SA_SatStateDataReqCmd_InvalidMsg */

/******************************************************************************
* 	SA_CMDS_AddTestCases
*/
void SA_CMDS_AddTestCases(void) {
    UtTest_Add(SA_TEST_SA_SatStateDataReqCmd_Nominal, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SatStateDataReqCmd_Nominal");
    UtTest_Add(SA_TEST_SA_SatStateDataReqCmd_NoPtr, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SatStateDataReqCmd_NoPtr");
    UtTest_Add(SA_TEST_SA_SatStateDataReqCmd_InvalidMsg, SA_Test_Setup, SA_Test_TearDown, "SA_TEST_SA_SatStateDataReqCmd_InvalidMsg");


} /* end SA_CMDS_AddTestCases */
