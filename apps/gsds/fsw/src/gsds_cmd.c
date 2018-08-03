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
** File Name:  gsds_cmd.c
**
** Title:  Definitions for GSDS cmds
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This file provides the Function Definitions for all GSDS Commands
** 	data and functions
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

/*
 * Includes
 */
#include "gsds_app.h"
#include "gsds_cmd.h"


/****************************************************************************************
* 	GSDS_GsReqIdCmd
****************************************************************************************/
void GSDS_GsReqIdCmd(CFE_SB_MsgPtr_t MsgPtr) {

	if (!GSDS_VerifyCmdLength(MsgPtr, sizeof(GSDS_GsReqIdCmd_t))) {
		CFE_EVS_SendEvent(GSDS_CMD_ERR_EID, CFE_EVS_ERROR,
				"GSDS: GsReqIdCmd failed Error: Invalid Cmd Length");
		g_GSDS_AppData.HkTlm.usCmdErrCnt++;
		return;
	}


	/* Report message recieved */
	CFE_EVS_SendEvent(GSDS_CMD_INF_EID, CFE_EVS_INFORMATION,
				"GSDS: GsReqIdCmd Received (0x%08X)", GSDS_GSREQID_CC);
	g_GSDS_AppData.HkTlm.usCmdCnt++;


	/* Set up Local Data */
	GSDS_GsReqIdCmd_t* msg = (GSDS_GsReqIdCmd_t*) MsgPtr;
	GSDS_GsDataOut_t out_msg;
	int32 status;

	/* Initialize out message */
	CFE_SB_InitMsg(&out_msg, GSDS_OUT_DATA_MID, sizeof(GSDS_GsDataOut_t), TRUE);

	/* Get Ground Station With Id */
	status = GSDS_GetGsId(msg->id, out_msg.GsData);

	/* Set the number of GS entries being sent */
	out_msg.NumGs = (status == CFE_SUCCESS) ? 1 : 0;

	/* Send out the response */
	status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&out_msg);

	if (status != CFE_SUCCESS) {
		CFE_EVS_SendEvent(GSDS_CMD_ERR_EID, CFE_EVS_ERROR,
				"GSDS: GsReqIdCmd failed to send out data Error: 0x%08X", status);
		g_GSDS_AppData.HkTlm.usCmdErrCnt++;
	}

} /* GSDS_GsReqIdCmd */

/****************************************************************************************
* 	GSDS_GsReqIdCmd
****************************************************************************************/
void GSDS_GsReqMatchCmd(CFE_SB_MsgPtr_t MsgPtr) {

	if (!GSDS_VerifyCmdLength(MsgPtr, sizeof(GSDS_GsReqMatchCmd_t))) {
		CFE_EVS_SendEvent(GSDS_CMD_ERR_EID, CFE_EVS_ERROR,
				"GSDS: GsReqIdCmd failed Error: Invalid Cmd Length");
		g_GSDS_AppData.HkTlm.usCmdErrCnt++;
		return;
	}

	/* Set up Local Data */
	GSDS_GsReqMatchCmd_t* msg = (GSDS_GsReqMatchCmd_t*) MsgPtr;
	GSDS_GsDataOut_t out_msg;
	int32 status;

	/* Initialize out message */
	CFE_SB_InitMsg(&out_msg, GSDS_OUT_DATA_MID, sizeof(GSDS_GsDataOut_t), TRUE);

	/* Get Ground Stations that match the desired parameter */
	status = GSDS_GetGsAllMatch(msg->flag,
								msg->equal,
								CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&out_msg));

	/* Set the number of GS entries being sent */
	out_msg.NumGs = status;

	/* Send out the response */
	status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&out_msg);

	if (status != CFE_SUCCESS) {
		CFE_EVS_SendEvent(GSDS_CMD_ERR_EID, CFE_EVS_ERROR,
				"GSDS: GsReqMatchCmd failed to send out data Error: 0x%08X", status);
	}

} /* end GSDS_GsReqMatchCmd */

/****************************************************************************************
* 	GSDS_GsReqIdCmd
****************************************************************************************/
void GSDS_AddGsCmd(CFE_SB_MsgPtr_t MsgPtr) {

	if (!GSDS_VerifyCmdLength(MsgPtr, sizeof(GSDS_GsReqMatchCmd_t))) {
		CFE_EVS_SendEvent(GSDS_CMD_ERR_EID, CFE_EVS_ERROR,
				"GSDS: GSDS_AddGsCmd failed Error: Invalid Cmd Length");
		g_GSDS_AppData.HkTlm.usCmdErrCnt++;
		return;
	}

	/* Set up Local Data */
	GSDS_AddGsCmd_t* msg = (GSDS_AddGsCmd_t*) MsgPtr;

	GSDS_AddGs(msg->GsData);
	g_GSDS_AppData.HkTlm.usCmdCnt++;
} /* end GSDS_AddGsCmd */


void GSDS_EditGsCmd(CFE_SB_MsgPtr_t MsgPtr) {
	//TODO Complete Edit Gs
}/* end GSDS_EditGsCmd */
/***************/
/* End of File */
/***************/
