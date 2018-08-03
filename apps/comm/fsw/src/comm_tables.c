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
** File Name:  comm_tables.c
**
** Title:  Source File for COMM Application table functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define functional implementation for the COMM application table functions,
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
#include "comm_tables.h"
#include "comm_app.h"
#include "cfs_utils.h"
#include "limits.h"

/****************************************************************************************
 * COMM_InitTables();
****************************************************************************************/
int32 COMM_InitTables() {
	int32 StatusD = CFE_SUCCESS;
	int32 StatusX = CFE_SUCCESS;
	uint16 TableRegisterFlags = CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
	uint8 NeedToLoadD = FALSE;
	uint8 NeedToLoadX = FALSE;

	/* DOWN LINK TABLE */
	StatusD = CFE_TBL_Register(&g_COMM_AppData.DLinkHandle,
				COMM_DLINK_TBL_NAME,
				sizeof(COMM_LinkFile_t) * COMM_MAX_DLINK_FILES,
				TableRegisterFlags,
				(CFE_TBL_CallbackFuncPtr_t) COMM_VerifyDLinkTbl);


	if (StatusD == CFE_SUCCESS) {

		//Table registered -> Load table
		NeedToLoadD = TRUE;

	}
	else {

		//Table registration Failed
		CFE_EVS_SendEvent(COMM_TBLINIT_ERR_EID, CFE_EVS_ERROR,
			"COMM: Unable to Register COMM DLink Table | Error: 0x%08X", StatusD);

	}

	/* CROSS LINK TABLE */
	StatusX = CFE_TBL_Register(&g_COMM_AppData.XLinkHandle,
				COMM_XLINK_TBL_NAME,
				sizeof(COMM_LinkFile_t) * COMM_MAX_XLINK_FILES,
				TableRegisterFlags,
				(CFE_TBL_CallbackFuncPtr_t) COMM_VerifyXLinkTbl);


	if (StatusX == CFE_SUCCESS) {

		//Table registered -> Load table
		NeedToLoadX = TRUE;

	}
	else {

		//Table registration Failed
		CFE_EVS_SendEvent(COMM_TBLINIT_ERR_EID, CFE_EVS_ERROR,
			"COMM: Unable to Register COMM XLink Table | Error: 0x%08X", StatusD);

	}


	/* Load the COMM Tables if needed */
	int32 LoadStatus;

	if (NeedToLoadD) {

		LoadStatus = CFE_TBL_Load(g_COMM_AppData.DLinkHandle,
				CFE_TBL_SRC_FILE, COMM_DLINK_FILENAME);

		if (LoadStatus != CFE_SUCCESS) {
			CFE_EVS_SendEvent(COMM_TBLINIT_ERR_EID, CFE_EVS_ERROR,
					"COMM: Unable to load default COMM DLink Table: Filename = '%s', Error = 0x%08X",
					COMM_DLINK_FILENAME, (unsigned int)LoadStatus);
		}
	}

	if (NeedToLoadX) {

		LoadStatus = CFE_TBL_Load(g_COMM_AppData.XLinkHandle,
				CFE_TBL_SRC_FILE, COMM_XLINK_FILENAME);

		if (LoadStatus != CFE_SUCCESS) {
			CFE_EVS_SendEvent(COMM_TBLINIT_ERR_EID, CFE_EVS_ERROR,
					"COMM: Unable to load default COMM DLink Table: Filename = '%s', Error = 0x%08X",
					COMM_DLINK_FILENAME, (unsigned int)LoadStatus);
		}
	}


	if (StatusD != CFE_SUCCESS) {
		return StatusD;
	}
	else if (StatusX != CFE_SUCCESS) {
		return StatusX;
	}
	else {
		return LoadStatus;
	}
} /* end COMM_InitTables*/

/****************************************************************************************
 * COMM_VerifyDLinkTbl
****************************************************************************************/
int32 COMM_VerifyDLinkTbl(void* TblPtr) {
	return COMM_VerifyLinkTbl((COMM_LinkFile_t*)TblPtr, COMM_MAX_DLINK_FILES);
} /* end COMM_VerifyDLinkTbl*/

/****************************************************************************************
 * COMM_VerifyXLinkTbl
****************************************************************************************/
int32 COMM_VerifyXLinkTbl(void* TblPtr) {
	return COMM_VerifyLinkTbl((COMM_LinkFile_t*)TblPtr, COMM_MAX_XLINK_FILES);
} /* end COMM_VerifyXLinkTbl*/


/****************************************************************************************
 * COMM_VerifyLinkTbl
****************************************************************************************/
int32 COMM_VerifyLinkTbl(COMM_LinkFile_t* TblPtr, int32 Length) {
	int32 Status = CFE_SUCCESS;
	int32 good = 0;
	int32 bad = 0;

	/* For each item in the table*/
	for (int i = 0; i < Length; i++) {

		/* If this entry is used*/
		if (TblPtr[i].Used) {

			/* Check that the file listed is valid */
			if (CFS_IsValidFilename(TblPtr[i].File, OS_MAX_PATH_LEN)) {
				good ++;
			}
			else {
				bad++;
			}
		}
		/* This entry is not used - Do nothing */
	}
	return Status;
} /* end COMM_VerifyLinkTbl*/


/****************************************************************************************
 * COMM_GrabDLinkPtrs
****************************************************************************************/
int32 COMM_GrabDLinkPtr() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for DLink Table
	Status = CFE_TBL_GetAddress((void**)&g_COMM_AppData.DLinkPtr,
								g_COMM_AppData.DLinkHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(COMM_GRABPTR_ERR_EID, CFE_EVS_ERROR,
			"COMM: Could not grab pointers for DLink Table, Error: 0x%08X",
			Status);
	}

	return Status;
} /* end COMM_GrabDLinkPtrs*/


/****************************************************************************************
 * COMM_DropDLinkPtrs
****************************************************************************************/
int32 COMM_DropDLinkPtr() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for GsState Table
	Status = CFE_TBL_ReleaseAddress(g_COMM_AppData.DLinkHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(COMM_GRABPTR_ERR_EID, CFE_EVS_ERROR,
			"COMM: Could not drop pointers for DLink Table, Error: 0x%08X",
			Status);
	}
	return Status;
} /* end COMM_DropDLinkPtrs*/

/****************************************************************************************
 * COMM_GrabXLinkPtrs
****************************************************************************************/
int32 COMM_GrabXLinkPtr() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for DLink Table
	Status = CFE_TBL_GetAddress((void**)&g_COMM_AppData.XLinkPtr,
								g_COMM_AppData.XLinkHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(COMM_GRABPTR_ERR_EID, CFE_EVS_ERROR,
			"COMM: Could not grab pointers for XLink Table, Error: 0x%08X",
			Status);
	}

	return Status;
} /* end COMM_GrabXLinkPtrs*/


/****************************************************************************************
 * COMM_DropXLinkPtrs
****************************************************************************************/
int32 COMM_DropXLinkPtr() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for GsState Table
	Status = CFE_TBL_ReleaseAddress(g_COMM_AppData.XLinkHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(COMM_GRABPTR_ERR_EID, CFE_EVS_ERROR,
			"COMM: Could not drop pointers for XLink Table, Error: 0x%08X",
			Status);
	}
	return Status;
} /* end COMM_DropXLinkPtrs*/

/****************************************************************************************
 * COMM_GrabPtrs
****************************************************************************************/
int32 COMM_GrabPtr(COMM_LinkType_t Link, COMM_LinkFile_t ** tbl, int32* len) {
	int32 Status;
	if (Link == DLINK) {
		Status = COMM_GrabDLinkPtr();
		if (Status != CFE_SUCCESS) {
			return Status;
		}

		*tbl = g_COMM_AppData.DLinkPtr;
		*len = COMM_MAX_DLINK_FILES;

	}
	else if (Link == XLINK) {
		Status = COMM_GrabXLinkPtr();
		if (Status != CFE_SUCCESS) {
			return Status;
		}

		*tbl = g_COMM_AppData.XLinkPtr;
		*len = COMM_MAX_XLINK_FILES;
	}

	return CFE_SUCCESS;
}


/****************************************************************************************
*
* COMM Table Functions - Specific
*
****************************************************************************************/

/****************************************************************************************
 * COMM_AddFile
****************************************************************************************/
int32 COMM_AddFile(COMM_LinkType_t Link, boolean Retain, boolean DelOnLink, uint8 Priority, char File[OS_MAX_PATH_LEN]) {
	COMM_LinkFile_t * tbl;
	int32 len;

	/* Information on the lowest priority entry in the table */
	int32 lp = INT_MAX;
	int32 lp_index = -1;

	if (COMM_GrabPtr(Link, &tbl, &len) != CFE_SUCCESS) {
		return COMM_GRABPTR_ERR_EID;
	}


	/* Iterate over the table */
	for (int i = 0; i < len; i ++) {

		/* This entry is unused */
		if (!(tbl[i].Used)) {

			/* Replace the existing entry with the given entry */
			tbl[i].Used = TRUE;
			tbl[i].RetainFlag = Retain;
			tbl[i].DeleteOnLink = DelOnLink;
			tbl[i].Priority = Priority;
			tbl[i].LinkAttempts = 0;
			CFE_PSP_MemCpy(tbl[i].File, File, OS_MAX_PATH_LEN);

			COMM_DropXLinkPtrs();
			COMM_DropDLinkPtrs();
			return CFE_SUCCESS;
		}
		else {
			/* Update Lowest priority information */
			if (tbl[i].Priority < lp) {
				lp = tbl[i].Priority;
				lp_index= i;
			}

		}
	}

	/* There are no unused entries and this is a priority 1 file
	 * and there is a lower priority file in the queue */
	if (Priority == 1 && lp > 1 && lp_index >= 0) {

		CFE_EVS_SendEvent(COMM_INF_EID, CFE_EVS_INFORMATION,
			"COMM: File %s added to link(type: %d) queue by overwrite of priority %d file %s",
			File, Link, tbl[lp_index].Priority, tbl[lp_index].File);

		/*overwrite the lowest priority file*/
		tbl[lp_index].Used = TRUE;
		tbl[lp_index].RetainFlag = Retain;
		tbl[lp_index].DeleteOnLink = DelOnLink;
		tbl[lp_index].Priority = Priority;
		tbl[lp_index].LinkAttempts = 0;
		CFE_PSP_MemCpy(tbl[lp_index].File, File, OS_MAX_PATH_LEN);
		COMM_DropXLinkPtrs();
		COMM_DropDLinkPtrs();
		return CFE_SUCCESS;
	}
	/* File is not priority one and the queue is full */
	else {
		CFE_EVS_SendEvent(COMM_ADDFILE_ERR_EID, CFE_EVS_INFORMATION,
			"COMM: Unable to add file %s to link queue of type %d : Queue Full",
			File, Link);
		COMM_DropXLinkPtrs();
		COMM_DropDLinkPtrs();
		return COMM_ADDFILE_ERR_EID;
	}
}

/****************************************************************************************
 * COMM_GetFileEntry
****************************************************************************************/
int32 COMM_GetFileEntry(COMM_LinkType_t Link, char File[OS_MAX_PATH_LEN]) {
	COMM_LinkFile_t * tbl;
	int32 len;

	if (COMM_GrabPtr(Link, &tbl, &len) != CFE_SUCCESS) {
		return COMM_GRABPTR_ERR_EID;
	}

	/* Search table for the specified filename */
	for (int i = 0; i < len; i++) {

		/* This entry is used */
		if (tbl[i].Used) {
			if (memcmp(File, tbl[i].File, OS_MAX_PATH_LEN) == 0) {
				return i;
			}
		}
	}
	COMM_DropXLinkPtrs();
	COMM_DropDLinkPtrs();
	return COMM_NO_ENTRY;
} /* end COMM_GetEntry */

/****************************************************************************************
 * COMM_GetFileName
****************************************************************************************/
int32 COMM_GetFileName(COMM_LinkType_t Link, int32 Index, char RetFile[OS_MAX_PATH_LEN]) {
	int32 Status;
	COMM_LinkFile_t * tbl;
	int32 len;

	if (COMM_GrabPtr(Link, &tbl, &len) != CFE_SUCCESS) {
		return COMM_GRABPTR_ERR_EID;
	}

	/* The index is a valid index */
	if(Index >= 0 && Index < len && tbl[Index].Used) {
		CFE_PSP_MemCpy(RetFile, tbl[Index].File, OS_MAX_PATH_LEN);
		Status = CFE_SUCCESS;
	}
	/* The idex is out of bounds or not used report error */
	else {
		CFE_EVS_SendEvent(COMM_ERR_EID, CFE_EVS_ERROR,
				"COMM: Could not retrieve entry %d for Link Table of type %d",
				Index, Link);
		Status = COMM_ERR_EID;

	}

	COMM_DropXLinkPtrs();
	COMM_DropDLinkPtrs();
	return Status;
} /* end COMM_GetFile */

/****************************************************************************************
 * COMM_UpdateEntry
****************************************************************************************/
int32 COMM_UpdateEntry(COMM_LinkType_t Link, int32 Index) {
	int32 Status;
	COMM_LinkFile_t * tbl;
	int32 len;

	if (COMM_GrabPtr(Link, &tbl, &len) != CFE_SUCCESS) {
		return COMM_GRABPTR_ERR_EID;
	}

	/* The index is a valid index */
	if(Index >= 0 && Index < len && tbl[Index].Used) {
		tbl[Index].LinkAttempts++;
		Status = CFE_SUCCESS;
	}
	/* The idex is out of bounds or not used report error */
	else {
		CFE_EVS_SendEvent(COMM_ERR_EID, CFE_EVS_ERROR,
				"COMM: Could not Update entry %d for Link Table of type %d",
				Index, Link);
		Status = COMM_ERR_EID;

	}

	COMM_DropXLinkPtrs();
	COMM_DropDLinkPtrs();
	return Status;
} /* end COMM_UpdateEntry */


/****************************************************************************************
 * COMM_HandleAck
****************************************************************************************/
int32 COMM_HandleAck(COMM_LinkType_t Link, COMM_AckMsg_t* Ack) {
	int32 Status = CFE_SUCCESS;
	COMM_LinkFile_t * tbl;
	int32 len;

	if (COMM_GrabPtr(Link, &tbl, &len) != CFE_SUCCESS) {
		return COMM_GRABPTR_ERR_EID;
	}

	/* Was the link completly unsuccessful */
	if (Ack->NumFiles == 0) {
		if (Link == XLINK) {
			g_COMM_AppData.HkTlm.XLinkFailCnt++;
		}
		else if (Link == DLINK) {
			g_COMM_AppData.HkTlm.DLinkFailCnt++;
		}
	}
	/* This link managed to link atleast one file */
	else {
		g_COMM_AppData.HkTlm.LinkCnt++;
	}

	int32 entry;

	/* For each file reported in the Ack Message */
	for (int i = 0; i < Ack->NumFiles; i++) {
		entry = COMM_GetFileEntry(Link, Ack->Files[i]);

		if(entry < 0) {
			CFE_EVS_SendEvent(COMM_ACK_ERR_EID, CFE_EVS_ERROR,
					"COMM: Invalid file found in link acknowledgment | File: %s",
					Ack->Files[i]);
		}
		/* The file was found in the link table */
		else {

			/* This is a retained file */
			if (tbl[entry].RetainFlag) {
				/* reset the link counter */
				tbl[entry].LinkAttempts = 0;
			}
			/* THis is NOT a retain file */
			else {
				/* the file should be deleted */
				if (tbl[entry].DeleteOnLink) {
					OS_remove(tbl[entry].File);
				}

				/* clear entry data and set this entry as unused */
				CFE_PSP_MemSet((void*)&(tbl[entry]), 0x00, sizeof (COMM_LinkFile_t));
				tbl[entry].Used = FALSE;
			}
		}
	}


	/* for all remaining entries in the file table check if they are stale */

	for (int j = 0; j < len; j ++) {
		/* The entry is used */
		if (tbl[j].Used) {
			/* The link attempts is greater than the alloted links */
			if (tbl[j].LinkAttempts > tbl[j].MaxLinkAttempts) {
				/* the file should be deleted */
				if (tbl[entry].DeleteOnLink) {
					OS_remove(tbl[entry].File);
				}

				/* clear entry data and set this entry as unused */
				CFE_PSP_MemSet((void*)&(tbl[entry]), 0x00, sizeof (COMM_LinkFile_t));
				tbl[entry].Used = FALSE;

				g_COMM_AppData.HkTlm.LinkFileFailCnt++;
			}
			/* the file is not stale, ignore for now */
		}
	}

	COMM_DropXLinkPtrs();
	COMM_DropDLinkPtrs();
	return Status;
} /* end COMM_HandleAck */


/****************************************************************************************
 * COMM_PreLinkUpdate
****************************************************************************************/
int32 COMM_PreLinkUpdate(COMM_LinkType_t Link) {
	int32 Status = CFE_SUCCESS;
	COMM_LinkFile_t * tbl;
	int32 len;

	if (COMM_GrabPtr(Link, &tbl, &len) != CFE_SUCCESS) {
		return COMM_GRABPTR_ERR_EID;
	}

	/* Sort the link table by priority for execution */
	qsort((void*)tbl, len, sizeof(COMM_LinkFile_t), COMM_PriorityComp);

	COMM_DropXLinkPtrs();
	COMM_DropDLinkPtrs();
	return Status;
}

/****************************************************************************************
 * COMM_PriorityComp
 *  -1 - If entry A < entry B in regards to priority
 * 	0 - If entry A = entry B if regards to priority
 * 	+1 - If entry A > Entry B in regards to priority
****************************************************************************************/
int32 COMM_PriorityComp(const void* A, const void* B) {
	int32 Status;
	COMM_LinkFile_t * fileA = (COMM_LinkFile_t*) A;
    COMM_LinkFile_t * fileB = (COMM_LinkFile_t*) B;


    /* Both Entries are being used */
	if (fileA->Used && fileB->Used) {

		/* A has a more urgent priority than B */
		if (fileA->Priority < fileB->Priority) {
			/* A First */
			Status = -1;
		}
		/* B Has a more urgent Priority than A */
		else if (fileA->Priority > fileB->Priority) {
			/* B first */

			Status = 1;
		}
		/* A and B have equal Priorities */
		else {
			/* A == B */
			Status = 0;
		}

	}
	/* File A Is used B is not */
	else if (fileA->Used) {
		/* A First */
		Status = -1;
	}
	/* File B is used B is not */
	else if (fileB->Used) {
		/* B first */
		return 1;
	}
	/* Both Files are unused */
	else {
		/* A == B */
		Status = 0;
	}

	return Status;
}

/****************
** End of File **
****************/
