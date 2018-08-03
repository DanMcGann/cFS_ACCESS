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
/*=======================================================================================
** File Name:  comm_cmds.c
**
** Title:  Source  File for COMM Application command functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define implementations for the COMM application command functions.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#include "comm_cmds.h"
#include "comm_exec.h"
#include "comm_app.h"
#include "comm_msg.h"
#include "comm_platform_cfg.h"

/****************************************************************************************
*
* COMM Command Functions - Standard
*
****************************************************************************************/

/****************************************************************************************
*   No Operation Command
****************************************************************************************/
void COMM_NoopCmd(CFE_SB_MsgPtr_t MsgPtr) {
	if (COMM_VerifyCmdLength(MsgPtr, sizeof(COMM_NoArgCmd_t))) {
		/* Update command counter and report reception*/
		g_COMM_AppData.HkTlm.usCmdCnt++;
		CFE_EVS_SendEvent(COMM_CMD_INF_EID, CFE_EVS_INFORMATION,
			"COMM: Noop Command received (%d)", CFE_SB_GetCmdCode(MsgPtr));

		/* React to Command*/

		/*
		 * No Operations to perform
		 */

	}
	else {
		/* Report and Record error */
		CFE_EVS_SendEvent(COMM_CMD_ERR_EID, CFE_EVS_ERROR,
			"COMM: Received invalid Noop Command");
		g_COMM_AppData.HkTlm.usCmdErrCnt++;
	}

} /* end COMM_NoopCmd*/

/****************************************************************************************
*   Reset Counters Command
****************************************************************************************/
void COMM_RstCmd(CFE_SB_MsgPtr_t MsgPtr) {
	if (COMM_VerifyCmdLength(MsgPtr, sizeof(COMM_NoArgCmd_t))) {
		/* Update command counter and report reception*/
		g_COMM_AppData.HkTlm.usCmdCnt++;
		CFE_EVS_SendEvent(COMM_CMD_INF_EID, CFE_EVS_INFORMATION,
			"COMM: Reset Counters Command received (%d)", CFE_SB_GetCmdCode(MsgPtr));

		/* React to Command*/

        g_COMM_AppData.HkTlm.usCmdCnt = 0;
        g_COMM_AppData.HkTlm.usCmdErrCnt = 0;


	}
	else {
		/* Report and Record error */
		CFE_EVS_SendEvent(COMM_CMD_ERR_EID, CFE_EVS_ERROR,
			"COMM: Received invalid Reset Counters Command");
		g_COMM_AppData.HkTlm.usCmdErrCnt++;
	}

}/* end COMM_RstCmd*/

/****************************************************************************************
*   Register File Command
****************************************************************************************/
void COMM_RegisterFileCmd(CFE_SB_MsgPtr_t MsgPtr) {
	if (COMM_VerifyCmdLength(MsgPtr, sizeof(COMM_RegisterFileCmd_t))) {
		/* Update command counter and report reception*/
		g_COMM_AppData.HkTlm.usCmdCnt++;
		CFE_EVS_SendEvent(COMM_CMD_INF_EID, CFE_EVS_INFORMATION,
			"COMM: Register File Command received (%d)", CFE_SB_GetCmdCode(MsgPtr));

		/* React to Command*/
		COMM_RegisterFileCmd_t* ptr = (COMM_RegisterFileCmd_t*) MsgPtr;

		COMM_AddFile(ptr->LinkType,
				ptr->Retain,
				ptr->DelOnLink,
				ptr->Priority,
				ptr->File);

	}
	else {
		/* Report and Record error */
		CFE_EVS_SendEvent(COMM_CMD_ERR_EID, CFE_EVS_ERROR,
			"COMM: Received invalid Register File Command");
		g_COMM_AppData.HkTlm.usCmdErrCnt++;
	}

}/* endCOMM_RegisterFileCmd*/

/****************************************************************************************
*   Start Link Command
****************************************************************************************/
void COMM_StartLinkCmd(CFE_SB_MsgPtr_t MsgPtr) {
	if (COMM_VerifyCmdLength(MsgPtr, sizeof(COMM_StartCmd_t))) {
		/* Update command counter and report reception*/
		g_COMM_AppData.HkTlm.usCmdCnt++;
		CFE_EVS_SendEvent(COMM_CMD_INF_EID, CFE_EVS_INFORMATION,
			"COMM: Start Link Command received (%d)", CFE_SB_GetCmdCode(MsgPtr));

		/* React to Command*/

		COMM_StartCmd_t* ptr = (COMM_StartCmd_t*) MsgPtr;
		COMM_InitLink(ptr->LinkType, ptr->LinkDur);

	}
	else {
		/* Report and Record error */
		CFE_EVS_SendEvent(COMM_CMD_ERR_EID, CFE_EVS_ERROR,
			"COMM: Received invalid Start Link Command");
		g_COMM_AppData.HkTlm.usCmdErrCnt++;
	}

}/* end COMM_StartLinkCmd*/

/****************************************************************************************
*   Stop Link Command
****************************************************************************************/
void COMM_StopLinkCmd(CFE_SB_MsgPtr_t MsgPtr) {
	if (COMM_VerifyCmdLength(MsgPtr, sizeof(COMM_NoArgCmd_t))) {
		/* Update command counter and report reception*/
		g_COMM_AppData.HkTlm.usCmdCnt++;
		CFE_EVS_SendEvent(COMM_CMD_INF_EID, CFE_EVS_INFORMATION,
			"COMM: Stop Link Command received (%d)", CFE_SB_GetCmdCode(MsgPtr));

		/* React to Command*/
		COMM_STOP_LINK_FUNC();

	}
	else {
		/* Report and Record error */
		CFE_EVS_SendEvent(COMM_CMD_ERR_EID, CFE_EVS_ERROR,
			"COMM: Received invalid Stop Link Command");
		g_COMM_AppData.HkTlm.usCmdErrCnt++;
	}

}/* end COMM_StopLinkCmd*/

/****************************************************************************************
*   Ack Link Command
****************************************************************************************/
void COMM_AckLinkCmd(CFE_SB_MsgPtr_t MsgPtr) {
//TODO cODE aCKlINK
}/* end COMM_AckLinkCmd*/
