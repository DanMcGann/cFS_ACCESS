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
** File Name:  sa_cmds.c
**
** Title:  Definitions for sa cmdss
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This file provides the Function Definitions for all sa cmds
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
**   2018-04-18 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#include "sa_app.h"
#include "sa_cmds.h"
#include "sa_msg.h"
#include "sa_tables.h"
#include "sa_events.h"
#include "sa_utils.h"
#include "access_lib.h"

/*
 * SA_SatStateDataReqCmd
 */
void SA_SatStateDataReqCmd(CFE_SB_MsgPtr_t MsgPtr) {
	int32 Status = CFE_SUCCESS;

	Status = SA_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return;
	}

	if (SA_VerifyCmdLength(MsgPtr, sizeof(SA_NoArgCmd_t))) {
		//Determine the size of the state data
		int32 size = FRMT_TBL_GetTotalSize(g_SA_AppData.SatFrmtPtr);

		//setup buffer in which to create message
		char buffer[CFE_SB_TLM_HDR_SIZE + size];

		//Initialize the message - sets up msg header
		CFE_SB_InitMsg(buffer,
				SA_OUT_DATA_MID,
				(CFE_SB_TLM_HDR_SIZE + size),
				TRUE);

		//Copy data into message
		CFE_PSP_MemCpy(CFE_SB_GetUserData((CFE_SB_MsgPtr_t) buffer),
				g_SA_AppData.SatStatePtr,
				size);

		//Sends the message
		CFE_SB_SendMsg((CFE_SB_MsgPtr_t)buffer);

		g_SA_AppData.HkTlm.usCmdCnt++;
		CFE_EVS_SendEvent(SA_CMD_INF_EID, CFE_EVS_INFORMATION,
						  "SA - Recvd SatStateDataReq cmd (%d)",
						  CFE_SB_GetCmdCode(MsgPtr));
	}

} /* end SA_SatStateDataReqCmd */
