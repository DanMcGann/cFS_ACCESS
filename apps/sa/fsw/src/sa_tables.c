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
** File Name:  sa_table.c
**
** Title:  Definitions for sa Tables
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This file provides the Function Definitions for all sa table
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

#include "sa_tables.h"
#include "sa_app.h"
#include "access_lib.h"

/******************************************************************************
 * SA_InitFrmtTable
 */
int32 SA_InitFrmtTable(void) {
	int32 Status = CFE_SUCCESS;
	boolean NeedToLoad = FALSE;
	uint16 TableRegisterFlags = CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;

	/* Register the Format Table */
	Status = CFE_TBL_Register(&g_SA_AppData.SatFrmtHandle,
			SA_SATFRMT_TBLNAME,
			(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * SA_SATFRMT_MAX_LEN)),
			TableRegisterFlags,
			(CFE_TBL_CallbackFuncPtr_t) SA_VerifySatFrmtTable);

	if (Status == CFE_TBL_INFO_RECOVERED_TBL) {

			// Table was recovered from the critical data store
			CFE_EVS_SendEvent(SA_INIT_TBL_ERR_EID, CFE_EVS_INFORMATION,
				"SA: Sat Frmt Table Recovered from CDS - SHOULD NOT OCCUR");

	}
	else if (Status == CFE_SUCCESS) {

		//Table registered -> Load table
		NeedToLoad = TRUE;

	}
	else {

		//Table registration Failed
		CFE_EVS_SendEvent(SA_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SA: Unable to Register Sat Frmt Table | Error: 0x%08X", Status);

	}

	/* Load the Sat Frmt Table if it is needed */
	int32 LoadStatus;
	if (NeedToLoad) {

		LoadStatus = CFE_TBL_Load(g_SA_AppData.SatFrmtHandle,
				CFE_TBL_SRC_FILE, SA_SATFRMT_FILENAME);

		if (LoadStatus != CFE_SUCCESS) {
			CFE_EVS_SendEvent(SA_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
					"SA: Unable to load default SatFrmt Table: Filename = '%s', Error = 0x%08X",
					SA_SATFRMT_FILENAME, (unsigned int)LoadStatus);

		}
	}

	if(Status == CFE_SUCCESS) {
		if(LoadStatus != CFE_SUCCESS) {
			OS_printf("LOAD STATUS ERROR\n\n");
		}
		return LoadStatus;
	}
	else {
		if(Status != CFE_SUCCESS) {
			OS_printf("Register STATUS ERROR\n\n");
		}
		return Status;
	}
} /* end SA_InitFrmtTable */

/******************************************************************************
 * SA_VerifySatFrmtTable
 */
int32 SA_VerifySatFrmtTable(void* TblPtr) {
	int32 Status;

	Status = FRMT_TBL_Verify(TblPtr);
	if (Status != CFE_SUCCESS) {
		g_SA_AppData.HkTlm.SatFrmtTblVerifyErrCnt++;
		return SA_VERIFY_TBL_ERR_EID;
	}

	return CFE_SUCCESS;
}

/******************************************************************************
 * SA_GrabPtrs
 */
int32 SA_GrabPtrs(void) {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for SatFrmt Table
	Status = CFE_TBL_GetAddress((void**)&g_SA_AppData.SatFrmtPtr,
			g_SA_AppData.SatFrmtHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(SA_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could not grab pointers for SatFrmt Table, Error: 0x%08X",
			Status);
		return Status;
	}
	return CFE_SUCCESS;
} /* end SA_GrabPtrs */

/******************************************************************************
 * SA_GrabPtrs
 */
int32 SA_DropPtrs(void) {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for SatFrmt Table
	Status = CFE_TBL_ReleaseAddress(g_SA_AppData.SatFrmtHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(SA_DROPPTRS_ERR_EID, CFE_EVS_ERROR,
			"SA: Could not drop pointers for SatFrmt Table, Error: 0x%08X",
			Status);
		return Status;
	}

	g_SA_AppData.SatFrmtPtr = NULL;
	return Status;
} /* end SA_DropPtrs */

/******************************************************************************
 * SA_AlllocMem
 */
int32 SA_AllocMem(void) {
	int32 Status = CFE_SUCCESS;


	Status = SA_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	int32 size = FRMT_TBL_GetTotalSize(g_SA_AppData.SatFrmtPtr);

	Status = CFE_ES_GetPoolBuf((uint32**)&g_SA_AppData.SatStatePtr,
			g_SA_AppData.MemPoolHandle,
			size);


	if (!(Status >= size)) {
		CFE_EVS_SendEvent(SA_ALLOCMEM_ERR_EID, CFE_EVS_ERROR,
				"SA: Could Not Allocate Memory Pool Space | Error: 0x%08X Size: %d",
				(unsigned int) Status, size);
		return Status;
	}

	return CFE_SUCCESS;
} /* end SA_AllocMem */

/******************************************************************************
 * SA_FreeMem
 */
int32 SA_FreeMem(void) {
	int32 Status = CFE_SUCCESS;

	Status = SA_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	int32 size = FRMT_TBL_GetTotalSize(g_SA_AppData.SatFrmtPtr);

	Status = CFE_ES_PutPoolBuf(g_SA_AppData.MemPoolHandle,
			(uint32 *)g_SA_AppData.SatStatePtr);

	if (!(Status >= size)) {
		CFE_EVS_SendEvent(SA_FREEMEM_ERR_EID, CFE_EVS_ERROR,
				"SA: Could Not Free Memory Pool Space | Error: 0x%08X Size: %d",
				(unsigned int) Status, size);
		return Status;
	}

	return CFE_SUCCESS;
} /* end SA_FreeMem */

/******************************************************************************
 * SA_IsValidMsgIdFlag
 */
boolean SA_IsValidMsgIdFlag(int32 MsgId) {
	int32 Status = CFE_SUCCESS;

	Status = SA_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	int32 field = FRMT_TBL_GetFlaggedFieldId(g_SA_AppData.SatFrmtPtr, MsgId);

	return (field >= 0);
} /* end SA_IsValidMsgIdFlag*/

/******************************************************************************
 * SA_SubSatStateMsgs
 */
int32 SA_SubSatStateMsgs(void) {
	int32 Status;
	int32 bad = 0;

	Status = SA_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	for (int i = 0; i < g_SA_AppData.SatFrmtEntries; i ++) {
		Status = CFE_SB_Subscribe(g_SA_AppData.SatFrmtPtr->Entries[i].Flag,
				g_SA_AppData.StatePipeId);

		if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
				"SA: Could Not Subscribe to Msg: 0x%08X | Error: 0x%08X ",
				(unsigned int) g_SA_AppData.SatFrmtPtr->Entries[i].Flag,
				(unsigned int) Status);
			bad++;
		}

	}

	return bad;
}

/******************************************************************************
 * SA_UnSubSatStateMsgs
 */
int32 SA_UnSubSatStateMsgs(void) {
	int32 Status;
	int32 bad = 0;

	Status = SA_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	for (int i = 0; i < g_SA_AppData.SatFrmtEntries; i ++) {
		Status = CFE_SB_Unsubscribe(g_SA_AppData.SatFrmtPtr->Entries[i].Flag,
				g_SA_AppData.StatePipeId);

		if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(SA_SUBMSGS_ERR_EID, CFE_EVS_ERROR,
				"SA: Could Not Unsubscribe to Msg: 0x%08X | Error: 0x%08X ",
				(unsigned int) g_SA_AppData.SatFrmtPtr->Entries[i].Flag,
				(unsigned int) Status);
			bad++;
		}

	}

	return bad;
}


