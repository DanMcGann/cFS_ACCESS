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
*	File: conds_cmds.c
*
*	Purpose: This file provides implementation for all conds command responce
*		functions.
*
*	Author: Daniel McGann
******************************************************************************/
#include "conds_cmds.h"
#include "conds_msg.h"
#include "cfe.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_CmdNoop() - NOOP command                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CONDS_NoopCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(CONDS_NoArgCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
    	g_CONDS_AppData.HkTlm.usCmdErrCnt++;

        CFE_EVS_SendEvent(CONDS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "CONDS - Recvd Invalid NOOP command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else
    {
        /*
        ** Do nothing except display "aliveness" event...
        */
        g_CONDS_AppData.HkTlm.usCmdCnt++;

        CFE_EVS_SendEvent(CONDS_CMD_INF_EID, CFE_EVS_INFORMATION,
        		"CONDS - Recvd NOOP cmd (%d)", CFE_SB_GetCmdCode(MessagePtr));
    }

    return;

} /* End of DS_CmdNoop() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_ResetCmd() - reset hk telemetry counters command             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CONDS_ResetCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(CONDS_NoArgCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
    	g_CONDS_AppData.HkTlm.usCmdErrCnt++;

        CFE_EVS_SendEvent(CONDS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "CONDS - Recvd Invalid RESET command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else
    {
        /*
        ** Reset application command counters...
        */
    	g_CONDS_AppData.HkTlm.usCmdCnt = 0;
    	g_CONDS_AppData.HkTlm.usCmdErrCnt = 0;
    	g_CONDS_AppData.HkTlm.ConStateTblLoads = 0;
    	g_CONDS_AppData.HkTlm.FrmtTblLoads = 0;
    	CFE_EVS_SendEvent(CONDS_CMD_INF_EID, CFE_EVS_INFORMATION,
    	"CONDS - Recvd RESET cmd (%d)", CFE_SB_GetCmdCode(MessagePtr));
    }

    return;

} /* End of CONDS_CmdReset() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_CmdReset() - Send Requested data Command				   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CONDS_DataReqCmd(CFE_SB_Msg_t * MsgPtr) {

	uint16 ActualLength = CFE_SB_GetTotalMsgLength(MsgPtr);
	uint16 ExpectedLength = sizeof(CONDS_DataReqCmd_t);

	if (ExpectedLength != ActualLength)
	{
		/*
		** Invalid command packet length...
		*/
		g_CONDS_AppData.HkTlm.usCmdErrCnt++;

		CFE_EVS_SendEvent(CONDS_DATAREQCMD_ERR_EID, CFE_EVS_ERROR,
						 "Invalid DATA REQ command length: expected = %d, actual = %d",
						  ExpectedLength, ActualLength);
		return;
	}

	g_CONDS_AppData.HkTlm.usCmdCnt++;


	//Command Received is good to Proceed
	for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i ++) {
		int eq = strcmp((g_CONDS_AppData.ConStatePtr)[i].FrmtName,
				((CONDS_DataReqCmd_t * ) MsgPtr)->dataName);

		//Are the Strings Equal
		if (eq == 0) {
			//Create a SoftwareBus Msg
			CFE_SB_Msg_t * out_msg = NULL;

			//Get size of the data to send
			int Size = FRMT_TBL_GetTotalSize((FRMT_TBL_t *)(g_CONDS_AppData.ConStatePtr)[i].FrmtPtr);

			//Initialize a software bus message
			CFE_SB_InitMsg(out_msg,
					((CONDS_DataReqCmd_t * ) MsgPtr)->destId,
					Size,
					TRUE);

			//Fill Message with data
			memcpy((g_CONDS_AppData.ConStatePtr)[i].DataPtr,
					(void*)CFE_SB_GetUserData(out_msg), Size);

			//Send this Message through the Software bus
			int32 Status = CFE_SB_SendMsg(MsgPtr);

			//Was there an error
			if(Status != CFE_SUCCESS) {
				CFE_EVS_SendEvent(CONDS_DATAREQCMD_ERR_EID, CFE_EVS_ERROR,
					"Data Req Message Failed to Send | Error: 0x%08X",
					Status);
			}
		}
	}//End For Loop

	CFE_EVS_SendEvent(CONDS_CMD_INF_EID, CFE_EVS_INFORMATION,
					 "CONDS - Recvd DATAREQ cmd for ConState Data: %s",
					 ((CONDS_DataReqCmd_t * ) MsgPtr)->dataName);

} /* End DataReq */


