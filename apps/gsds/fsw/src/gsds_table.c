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
** File Name:  gsds_table.c
**
** Title:  Definitions for GSDS Tables
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This file provides the Function Definitions for all GSDS table
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
#include "gsds_app.h"
#include "gsds_table.h"
#include "gsds_events.h"
#include "cfs_utils.h"

/****************************************************************************************
** GSDS_InitTable
****************************************************************************************/
int32 GSDS_InitTable() {
	int32 Status = CFE_SUCCESS;
	uint16 TableRegisterFlags = CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
	uint8 NeedToLoad = FALSE;

	#if (GSDS_MAKE_TABLES_CRITICAL == 1)
	TableRegisterFlags |= CFE_TBL_OPT_CRITICAL;
	#endif

	Status = CFE_TBL_Register(&g_GSDS_AppData.StateHandle,
				GSDS_STATE_TBL_NAME,
				sizeof(GSDS_StateTbl_t),
				TableRegisterFlags,
				(CFE_TBL_CallbackFuncPtr_t) GSDS_VerifyStateTable);


	if (Status == CFE_TBL_INFO_RECOVERED_TBL) {

		// Table was recovered from the critical data store
		CFE_EVS_SendEvent(GSDS_TBLINIT_INF_EID, CFE_EVS_INFORMATION,
			"GSDS: GS State Table Recovered from CDS");
		Status = CFE_SUCCESS;

	}
	else if (Status == CFE_SUCCESS) {

		//Table registered -> Load table
		NeedToLoad = TRUE;

	}
	else {

		//Table registration Failed
		CFE_EVS_SendEvent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Unable to Register GS State Table | Error: 0x%08X", Status);

	}

	/* Load the GS State Table if it is needed */
	int32 LoadStatus;

	if (NeedToLoad) {

		LoadStatus = CFE_TBL_Load(g_GSDS_AppData.StateHandle,
				CFE_TBL_SRC_FILE, GSDS_STATE_FILENAME);

		if (LoadStatus != CFE_SUCCESS) {
        	CFE_EVS_SendEvent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
        			"Unable to load default GS State Table: Filename = '%s', Error = 0x%08X",
					GSDS_STATE_FILENAME, (unsigned int)LoadStatus);

        	return LoadStatus;

		}
	}

	return Status;
} /* end GSDS_InitTable */


/****************************************************************************************
** GSDS_InitFrmtTable
****************************************************************************************/
int32 GSDS_InitFrmtTable() {
	int32 Status = CFE_SUCCESS;
	uint16 TableRegisterFlags = CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
	uint8 NeedToLoad = FALSE;


	#if (GSDS_MAKE_TABLES_CRITICAL == 1)
	TableRegisterFlags |= CFE_TBL_OPT_CRITICAL;
	#endif

	Status = CFE_TBL_Register(&g_GSDS_AppData.GsFrmtHandle,
				GSDS_GSFRMT_TBL_NAME,
				sizeof(GSDS_StateTbl_t),
				TableRegisterFlags,
				(CFE_TBL_CallbackFuncPtr_t) GSDS_VerifyFrmtTable);


	if (Status == CFE_TBL_INFO_RECOVERED_TBL) {

		// Table was recovered from the critical data store
		CFE_EVS_SendEvent(GSDS_TBLINIT_INF_EID, CFE_EVS_INFORMATION,
			"GSDS: GS Format Table Recovered from CDS");
		Status = CFE_SUCCESS;

	}
	else if (Status == CFE_SUCCESS) {

		//Table registered -> Load table
		NeedToLoad = TRUE;

	}
	else {

		//Table registration Failed
		CFE_EVS_SendEvent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Unable to Register GS Format Table | Error: 0x%08X", Status);

	}

	/* Load the GS Format Table if it is needed */
	int32 LoadStatus;

	if (NeedToLoad) {

		LoadStatus = CFE_TBL_Load(g_GSDS_AppData.GsFrmtHandle,
				CFE_TBL_SRC_FILE, GSDS_GSFRMT_FILENAME);

		if (LoadStatus != CFE_SUCCESS) {
        	CFE_EVS_SendEvent(GSDS_TBLINIT_ERR_EID, CFE_EVS_ERROR,
        			"Unable to load default GS Format Table: Filename = '%s', Error = 0x%08X",
					GSDS_GSFRMT_FILENAME, (unsigned int)LoadStatus);
        	return LoadStatus;

		}
	}

	if (NeedToLoad) {
		return LoadStatus;
	}
	else {
		return Status;
	}
} /* end GSDS_InitFrmtTable */

/****************************************************************************************
** GSDS_VerifyStateTable
****************************************************************************************/
int32 GSDS_VerifyStateTable(void* TblPtr) {
	int32 Status = CFE_SUCCESS;

	int32 numUsed = 0;
	int32 good = 0;
	int32 bad = 0;

	GSDS_StateTbl_t * tbl = (GSDS_StateTbl_t *) TblPtr;

	//Verify the GS State Table
	for (int i = 0; i < GSDS_MAX_GS; i++) {
		//Is this entry used
		if (tbl->Stations[i].IsUsed == TRUE) {
			//Increment the number used
			numUsed++;

			//Checks for GS State Table

			//Check the is Used Flag
			if (tbl->Stations[i].IsUsed != 1 && tbl->Stations[i].IsUsed != 0) {
				//IsUsed Plag is not properly a boolean
				bad++;
			}
			else {
				good++;
			}
		}
	}

	//Check that the numUsed matches the actual number used
	if (numUsed != tbl->NumEntries) {
		CFE_EVS_SendEvent(GSDS_VERIY_TBL_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Error State Table NumEntries incorrect | Expected: %d, Actual: %d",
			tbl->NumEntries, numUsed);
	}

	//Report on table verification
	if (bad != 0) {
		CFE_EVS_SendEvent(GSDS_VERIY_TBL_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Error State Table not verifiable | Good: %d, Bad %d",
			good, bad);
		g_GSDS_AppData.HkTlm.StateTableVerifyErrCnt++;
		Status = GSDS_VERIY_TBL_ERR_EID;
	}
	else {
		CFE_EVS_SendEvent(GSDS_VERIY_TBL_INF_EID, CFE_EVS_INFORMATION,
			"GSDS: State Table verified | Good: %d, Bad %d",
			good, bad);
		Status = CFE_SUCCESS;
	}

	return Status;
} /* end GSDS_VerifyStateTable */

/****************************************************************************************
** GSDS_VerifyFrmtTable
****************************************************************************************/
int32 GSDS_VerifyFrmtTable(void* TblPtr) {
	int32 Status;

	Status = FRMT_TBL_Verify(TblPtr);
	if (Status != CFE_SUCCESS) {
		g_GSDS_AppData.HkTlm.FrmtTableVerifyErrCnt++;
		return GSDS_VERIY_TBL_ERR_EID;
	}

	return CFE_SUCCESS;
} /* end GSDS_VerifyFrmtTable*/

/****************************************************************************************
** GSDS_GrabPtrs
****************************************************************************************/
int32 GSDS_GrabPtrs() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for GsState Table
	Status = CFE_TBL_GetAddress((void**)&g_GSDS_AppData.StatePtr,
								g_GSDS_AppData.StateHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not grab pointers for GsState Table, Error: 0x%08X",
			Status);
		return Status;
	}

	//Get Pointers for the GsFrmt Table
	Status = CFE_TBL_GetAddress((void**)&g_GSDS_AppData.GsFrmtPtr,
								g_GSDS_AppData.GsFrmtHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not grab pointers for GsFrmt Table, Error: 0x%08X",
			Status);
		return Status;
	}

	return Status;
} /* End GSDS_GrabPtrs */

/****************************************************************************************
** GSDS_DropPtrs
****************************************************************************************/
int32 GSDS_DropPtrs() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for GsState Table
	Status = CFE_TBL_ReleaseAddress(g_GSDS_AppData.StateHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not drop pointers for GsState Table, Error: 0x%08X",
			Status);
		return Status;
	}

	//Get Pointers for the GsFrmt Table
	Status = CFE_TBL_ReleaseAddress(g_GSDS_AppData.GsFrmtHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(GSDS_GRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"GSDS: Could not drop pointers for GsFrmt Table, Error: 0x%08X",
			Status);
		return Status;
	}

	return Status;
} /* End GSDS_DropPtrs */

/****************************************************************************************
** GSDS_AddGs()
****************************************************************************************/
int32 GSDS_AddGs(char* Data) {
	int32 Status = CFE_SUCCESS;

	Status = GSDS_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	Status = GSDS_ADDGS_ERR_EID;
	// Find Open Position
	for (int i = 0; i < GSDS_MAX_GS; i ++) {
		if (g_GSDS_AppData.StatePtr->Stations[i].IsUsed == FALSE) {
			//Place a Ground Station in this Location
			g_GSDS_AppData.StatePtr->Stations[i].IsUsed = TRUE;

			//Get ground station data size
			int32 Size = FRMT_TBL_GetTotalSize(g_GSDS_AppData.GsFrmtPtr);
			//clear buffer
			CFE_PSP_MemSet(g_GSDS_AppData.StatePtr->Stations[i].Data,
						   0x00,
						   GSDS_MAX_DATA_LEN);
			//copy data into buffer
			CFE_PSP_MemCpy(g_GSDS_AppData.StatePtr->Stations[i].Data,
					       Data,
					       Size);
			//Increment num entries
			g_GSDS_AppData.StatePtr->NumEntries++;
			return CFE_SUCCESS;
		}
	}

	if (Status != CFE_SUCCESS) {
		CFE_EVS_SendEvent(GSDS_ADDGS_ERR_EID, CFE_EVS_ERROR,
				"GSDS: Could Not Add Ground Station to GsState Table: Table Full");
	}
	return Status;
} /* end AddGs */


/****************************************************************************************
** GSDS_GetGsId()
****************************************************************************************/
int32 GSDS_GetGsId(char* id, char* retptr) {
	int32 Status = CFE_SUCCESS;

	Status = GSDS_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	int32 field = FRMT_TBL_GetFlaggedFieldId(g_GSDS_AppData.GsFrmtPtr, GSDS_ID_FLAG);

	if (field != CFE_SUCCESS) {
		CFE_EVS_SendEvent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
				"GSDS: No Format Table field with GSDS_ID_FLAG");
		return field;
	}

	int32 size = FRMT_TBL_GetSizeOfField(g_GSDS_AppData.GsFrmtPtr, field);

	char identif[size];

	// Iterate over table
	for (int i = 0; i < GSDS_MAX_GS; i++) {
		if (g_GSDS_AppData.StatePtr->Stations[i].IsUsed == TRUE) {

			Status = FRMT_TBL_GetFieldData(g_GSDS_AppData.GsFrmtPtr,
					g_GSDS_AppData.StatePtr->Stations[i].Data,
					field,
					identif);

			if (Status != CFE_SUCCESS) {
				CFE_EVS_SendEvent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
						"GSDS: Could Extract Ground Station Identification Data | Error: %d",
						Status);
				return Status;
			}

			//Compare the memory based on the size
			if (memcmp(id, identif, size) == 0) {
				/* IF this is the gs with the given identifier copy this
					ground station into the desired location */

				int32 total_size = FRMT_TBL_GetTotalSize(g_GSDS_AppData.GsFrmtPtr);

				CFE_PSP_MemCpy(retptr,
						g_GSDS_AppData.StatePtr->Stations[i].Data,
						total_size);
				return CFE_SUCCESS;
			}
		}
	}
	CFE_EVS_SendEvent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
		"GSDS: No Ground Station found with the given Id");
	return GSDS_GETGGSDATA_ERR_EID;
} /* end GSDS_GetGsId */

/****************************************************************************************
** GSDS_GetGsAllMatch()
****************************************************************************************/
int32 GSDS_GetGsAllMatch(uint8 flag, char * equal, char* retptr) {
	int32 Status = CFE_SUCCESS;

	Status = GSDS_GrabPtrs();
	if (Status != CFE_SUCCESS) {
		return Status;
	}

	/* Set up local data */
	int32 field = FRMT_TBL_GetFlaggedFieldId(g_GSDS_AppData.GsFrmtPtr, flag);

	if (field < 0) {
		CFE_EVS_SendEvent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
				"GSDS: No Format Table field with given flag");
		return field;
	}

	int32 size = FRMT_TBL_GetSizeOfField(g_GSDS_AppData.GsFrmtPtr, field);

	int32 total_size = FRMT_TBL_GetTotalSize(g_GSDS_AppData.GsFrmtPtr);

	char identif[size];
	int32 offset = 0;
	int32 num = 0;

	// Iterate over table
	for (int i = 0; i < GSDS_MAX_GS; i++) {
		if (g_GSDS_AppData.StatePtr->Stations[i].IsUsed == TRUE) {

			Status = FRMT_TBL_GetFieldData(g_GSDS_AppData.GsFrmtPtr,
					g_GSDS_AppData.StatePtr->Stations[i].Data,
					field,
					identif);

			if (Status != CFE_SUCCESS) {
				CFE_EVS_SendEvent(GSDS_GETGGSDATA_ERR_EID, CFE_EVS_ERROR,
						"GSDS: Could Extract Ground Station Identification Data from GS entry %d | Error %d",
						i, Status);
			}

			//Compare the memory based on the size
			if (memcmp(equal, identif, size) == 0) {
				/* IF this is the gs with the given identifier copy this
					ground station into the desired location */
				CFE_PSP_MemCpy((retptr + offset),
						g_GSDS_AppData.StatePtr->Stations[i].Data,
						total_size);
				/* Update offset info */
				offset = offset + total_size;
				num++;
			}
		}
	}
	return num;
} /* end GSDS_GetGsAllMatch */


