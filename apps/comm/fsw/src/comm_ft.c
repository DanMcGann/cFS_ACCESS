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
** File Name:  comm_ft.c
**
** Title:  Source File for COMM Application file Transfer Functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  This header file contains Implementations for a set of functions that allow
** COMM to transfer files through commonly used file transfer applications. These
** functions can be used for the COMM Configuration SEND_FILE_FUNC parameter.
**
** NOTES: (Copied from comm_ft.h)
** If the mission is using a file transport application not supported "out of the box"
** mission specific functions can be declared in this file, and written in the
** the corresponding source file. OR the source code for the files can be included in
** comm/fsw/platform_cfg/comm_platform_cfg.h file.
**
** All SEND_FILE_FUNC Functions should have the signature
**
** int32 <func_name>(char File[OS_MAX_PATH_LEN]
**
** and should return  CFE_SUCCESS [0] on a successful operation and anything else to
** represent an error.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#include "comm_ft.h"
#include "comm_tables.h"

/****************************************************************************************
* \brief Commands CF Application to send the given file via CF
*
* \par Description: This functions interfaces with the CFS CF Application, commanding
* it to send out the given file under default settings, with acknowledgment.
*
* \returns
* 	\retstmt CFE_SUCCESS - If all operations function nominally
* \endreturn
****************************************************************************************/
#include "cf_msg.h"
#include "cf_msg.h"
#include "cf_msgids.h"

int32 COMM_CF_SendFile(char File[OS_MAX_PATH_LEN]) {
	int32 Status;

	/* create instance of playback command message */
	CF_PlaybackFileCmd_t msg;

	CFE_SB_InitMsg(&msg, CF_CMD_MID, sizeof(CF_PlaybackFileCmd_t), TRUE);

	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&msg, CF_PLAYBACK_FILE_CC);

	/* get File Priority */
	msg.Class = 2;
	msg.Channel = 0;
	msg.Priority = 1;
	msg.Preserve = 1;
	CFE_PSP_MemCpy(msg.SrcFilename, File, OS_MAX_PATH_LEN);
	CFE_PSP_MemCpy(msg.DstFilename, File, OS_MAX_PATH_LEN);


	Status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t) &msg);

	return Status;
} /* end COMM_CF_SendFile */

/****************************************************************************************
* Commands CF Application to stop all file transfers being processed
*
* Description: This functions interfaces with the CFS CF Application, commanding
* it to stop all file transactions currently taking place. and clears the CF queues to
* ensure that they are clear for the next link opportunity, which may not be with the same
* target and therefore cannot be retained.
*
****************************************************************************************/
int32 COMM_CF_StopTrans(void) {
	int32 Status;

	CF_PurgeQueueCmd_t msg;

	CFE_SB_InitMsg(&msg, CF_CMD_MID, sizeof(CF_PurgeQueueCmd_t), TRUE);
	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&msg, CF_PURGE_QUEUE_CC);


	/* Pending Queue Purge */
	msg.Type = 2;
	msg.Chan = 0;
	msg.Queue = 0;

	Status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t) &msg);

	if (Status != CFE_SUCCESS) {
		return Status;
	}

	/* Active Queue Purge */

	msg.Type = 2;
	msg.Chan = 0;
	msg.Queue = 1;

	Status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t) &msg);

	return Status;
} /* end COMM_CF_StopTrans */
