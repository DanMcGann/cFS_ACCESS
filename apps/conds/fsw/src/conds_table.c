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
/*******************************************************************************
*   File: conds_table.h
*
*   Purpose: This File provides the implementation of all CONDS table functions
*
*   Author: Daniel McGann
*
*******************************************************************************/
#include "cfe.h"

#include "conds_table.h"
#include "conds_events.h"
#include "conds_app.h"



#include "access_lib.h"
#include "cfs_utils.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_InitTables() - CONDS application table initialization     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_InitTables (void) {
    int32 Status;
    uint16 TableRegisterFlags = CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
    boolean NeedToLoad;    

    #if (CONDS_MAKE_TABLES_CRITICAL == 1)
    TableRegisterFlags |= CFE_TBL_OPT_CRITICAL;
    #endif

    /*
    **  Register the Constellation State table with cFE table services
    */
    Status = CFE_TBL_Register(&(g_CONDS_AppData.ConStateTblHandle),
                CONDS_CONSTATE_TBL_NAME,
                sizeof(CONDS_ConStateEntry_t[CONDS_CONSTATE_TYPE_CNT]),
                TableRegisterFlags,
                (CFE_TBL_CallbackFuncPtr_t) CONDS_VerifyConState);

    if(Status == CFE_TBL_INFO_RECOVERED_TBL) {
        //Table is registered and resorted from data
        NeedToLoad = FALSE;
        CFE_EVS_SendEvent(CONDS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG,
            "Constellation State Table recovered from CDS");
        return CFE_SUCCESS;
    }
    else if ( (Status == CFE_SUCCESS) ) {
        //Table registered and needs loading
        NeedToLoad = TRUE;
    }
    else {
        //Table registration failed
        CFE_EVS_SendEvent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
            "Unable to Register Constellation State Table, Error = 0x%08X", 
            (unsigned int) Status);
        return Status;
    }   //Done With Registration process

    
    /*
    **  Load the Constellation State Table
    */
    if (NeedToLoad) {
        Status = CFE_TBL_Load(g_CONDS_AppData.ConStateTblHandle,
            CFE_TBL_SRC_FILE, CONDS_CONSTATE_FILENAME);
        
        if (Status != CFE_SUCCESS) {
            CFE_EVS_SendEvent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
                "Unable to load default Constellation State table from File: %s, Error: 0x%08X",
                CONDS_CONSTATE_FILENAME, (unsigned int) Status);
            return Status;
        }
    } //Done with load process

    return Status;
} /* End InitTables */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_InitFrmtTables() - CONDS frmt table initialization        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_InitFrmtTables(void) {
	int32 Status;
	uint16 TableRegisterFlags = CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;

	#if (CONDS_MAKE_TABLES_CRITICAL == 1)
	TableRegisterFlags |= CFE_TBL_OPT_CRITICAL;
 	#endif

	//Get Pointer for ConState Table
	Status = CFE_TBL_GetAddress((void**) &g_CONDS_AppData.ConStatePtr,
									g_CONDS_AppData.ConStateTblHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not grab pointers for ConState Table, Error: 0x%08X",
			Status);
		return Status;
	}

	//If we have pointers Go through ConState Table
	for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {

		//Register the table
		Status = CFE_TBL_Register(&(g_CONDS_AppData.ConStatePtr)[i].FrmtHandle,
					(g_CONDS_AppData.ConStatePtr)[i].FrmtName,
					(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * CONDS_MAX_FRMT_SIZE)),
					TableRegisterFlags,
					(CFE_TBL_CallbackFuncPtr_t) NULL);


		//Check Registration Status
		if (Status == CFE_TBL_INFO_RECOVERED_TBL) {
			// No need for table load
	        CFE_EVS_SendEvent(CONDS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG,
	        		"Load from CDS Frmt Tbl #%d |  File: %s",
					i, (g_CONDS_AppData.ConStatePtr)[i].FrmtFilename);
		}
		else if (Status == CFE_SUCCESS) {

			//Load the table
			Status = CFE_TBL_Load((g_CONDS_AppData.ConStatePtr)[i].FrmtHandle,
					CFE_TBL_SRC_FILE, (g_CONDS_AppData.ConStatePtr)[i].FrmtFilename);

			if (Status != CFE_SUCCESS) {
				CFE_EVS_SendEvent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
					"Unable to load default Frmt Tbl #%d |  File: %s, Error: 0x%08X",
					i, (g_CONDS_AppData.ConStatePtr)[i].FrmtFilename, (unsigned int) Status);
				g_CONDS_AppData.FrmtTablesInit = FALSE;
			}
			else {
				g_CONDS_AppData.FrmtTablesInit = TRUE;
			}

		}
		else {

			//Format Table not registered
			CFE_EVS_SendEvent(CONDS_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
				"Unable to Register Frmt Tbl #%d | Error: 0x%08X",
				i, (unsigned int) Status);
			g_CONDS_AppData.FrmtTablesInit = FALSE;
		}
	}//End For Loop
    
    CFE_TBL_ReleaseAddress(g_CONDS_AppData.ConStateTblHandle);

    return CFE_SUCCESS;
} /* End InitFrmtTables() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_GrabPtrs - CONDS Gets all table pointers			       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_GrabPtrs() {
	int32 Status = CFE_SUCCESS;
	//Get Pointer for ConState Table
	Status = CFE_TBL_GetAddress((void**)&g_CONDS_AppData.ConStatePtr,
								g_CONDS_AppData.ConStateTblHandle);

	//Check Grab of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not grab pointers for ConState Table, Error: 0x%08X",
			Status);
		return Status;
	}
	else {
		Status = CFE_SUCCESS;
	}

	//Get Pointers for all Format Tables
	for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {

		Status = CFE_TBL_GetAddress((void**) &(g_CONDS_AppData.ConStatePtr)[i].FrmtPtr,
				g_CONDS_AppData.ConStatePtr[i].FrmtHandle);

		//Check Grab of Pointer was Successful
		if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
			CFE_EVS_SendEvent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
						"CONDS: Could not grab pointers for Frmt Table, Index: %d  Error: 0x%08X",
						i, Status);
			return Status;
		}
		else {
			Status = CFE_SUCCESS;
		}
	} // End For Loop
	return Status;
} /* End CONDS_GrabPtrs() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_DropPtrs - CONDS Drops all table pointers			       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_DropPtrs() {
	int32 Status = CFE_SUCCESS;


	//Release Pointers for all Format Tables
	for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {

		Status = CFE_TBL_ReleaseAddress(g_CONDS_AppData.ConStatePtr[i].FrmtHandle);

		//Check Release of Pointer was Successful
		if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
			CFE_EVS_SendEvent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
						"CONDS: Could not release pointer for Frmt Table, Index: %d  Error: 0x%08X",
						i, Status);
			return Status;
		}

		//Set save location to NULL Pointer
		(g_CONDS_AppData.ConStatePtr)[i].FrmtPtr = NULL;

	} // End For Loop


	//Release Pointer for ConState Table
	Status = CFE_TBL_ReleaseAddress(g_CONDS_AppData.ConStateTblHandle);

	//Check Release of Pointer was Successful
	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(CONDS_TBLGRABPTRS_ERR_EID, CFE_EVS_ERROR,
			"CONDS: Could not release pointer for ConState Table, Error: 0x%08X",
			Status);
		return Status;
	}

	//Set Save Location to Null
	g_CONDS_AppData.ConStatePtr = NULL;

	return Status;
} /* End CONDS_DropPtrs() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_SubscribeStateMsgs() - CONDS state msg subscription       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_SubscribeStateMsgs(void) {
	int32 Status;
    Status = CONDS_GrabPtrs();

    if (Status != CFE_SUCCESS) {
    	return Status;
    }

    // For each Constellation State Entry
    for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {

    	Status = CFE_SB_Subscribe((g_CONDS_AppData.ConStatePtr)[i].MsgId, g_CONDS_AppData.ConStatePipeId);

        if (Status != CFE_SUCCESS) {
            CFE_EVS_SendEvent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
                "Unable to Subscribe to Constellation State message | MsgId: 0x%08X, Error: 0x%08X",
				(g_CONDS_AppData.ConStatePtr)[i].MsgId, Status);
        }
    }
    return CFE_SUCCESS;
} /* End SubscribeStateMsgs */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_UnSubscribeStateMsgs() - CONDS state msg unsubscription   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_UnSubscribeStateMsgs(void) {
    int32 Status;

    //Make sure we have the table pointers
    Status = CONDS_GrabPtrs();
    
    if (Status != CFE_SUCCESS) {
    	return Status;
    }

    // For each Constellation State Entry 
    for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {

    	Status = CFE_SB_Unsubscribe((g_CONDS_AppData.ConStatePtr)[i].MsgId, g_CONDS_AppData.ConStatePipeId);

        if (Status != CFE_SUCCESS) {
            CFE_EVS_SendEvent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
                "Unable to Unsubscribe to Constellation State message | MsgId: 0x%08X, Error: 0x%08X",
                (g_CONDS_AppData.ConStatePtr)[i].MsgId, Status);
        }
    }
    return CFE_SUCCESS;
} /* End UnSubscribeStateMsgs */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_AllocateStateMemory() - CONDS state memory allocation     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CONDS_AllocateStateMemory(void) {
    int32 Status;

    //Make sure we have the table pointers
        Status = CONDS_GrabPtrs();

        if (Status != CFE_SUCCESS) {
        	return Status;
        }

    // For each Constellation State Entry 
    for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {
        //Got format table, allocate memory and point DataPtr to it
    	int32 Size = FRMT_TBL_GetTotalSize((FRMT_TBL_t*)(g_CONDS_AppData.ConStatePtr)[i].FrmtPtr);

    	Status = CFE_ES_GetPoolBuf(&(g_CONDS_AppData.ConStatePtr)[i].DataPtr,
    								g_CONDS_AppData.MemPoolHandle, Size);
    	if (!(Status >= Size)) {
    		CFE_EVS_SendEvent(CONDS_ALLOCMEM_ERR_EID, CFE_EVS_ERROR,
    				"GSDS: Could Not Allocate Memory Pool Space | Error: 0x%08X Size: %d",
					(unsigned int) Status, Size);
    		return Status;
    	}
    }
    return CFE_SUCCESS;
} /* CONDS_AllocateStateMemory() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_FreeStateMemory() - CONDS state memory deallocation       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * void CONDS_DataReqCmd(CFE_SB_Msg_t * MsgPtr);
void CONDS_DataReqCmd(CFE_SB_Msg_t * MsgPtr);* * * * * * * * * * * * */
int32 CONDS_FreeStateMemory(void) {
    int32 Status;

    //Make sure we have the table pointers
        Status = CONDS_GrabPtrs();

        if (Status != CFE_SUCCESS) {
        	return Status;
        }

    // For each Constellation State Entry
    for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {
    	//Free the data allocated for Constellation State Data
    	Status = CFE_ES_PutPoolBuf(g_CONDS_AppData.MemPoolHandle,
    			(g_CONDS_AppData.ConStatePtr)[i].DataPtr);
    	if (!(Status > 0)) {
    		CFE_EVS_SendEvent(CONDS_ALLOCMEM_ERR_EID, CFE_EVS_ERROR,
    				"Could Not Free Memory Pool Space | Error: 0x%08X",
					Status);
    		return Status;
    	}
    }
    return CFE_SUCCESS;
} /* CONDS_FreeStateMemory() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_VerifyConState() - CONDS state table Validation           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_VerifyConState(void* TblData) {
    CONDS_ConStateEntry_t* TblPtr = (CONDS_ConStateEntry_t *) TblData;
    int32 Status = CFE_SUCCESS;
    
    //For each entry in the Constellation State Table
    for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {


        //Verify Format Table Name
        if (!CFS_VerifyString(TblPtr[i].FrmtName, OS_MAX_API_NAME, TRUE, TRUE)) {

            CFE_EVS_SendEvent(CONDS_TABLE_VERIFY_ERR_EID, CFE_EVS_ERROR,
                "Invalid Format Table Name | Index: %d", i);
            Status = CONDS_TABLE_VERIFY_ERR_EID;
        }


        //Verify Format Table File Name
        if (!CFS_IsValidFilename(TblPtr[i].FrmtFilename, OS_MAX_PATH_LEN)) {

            CFE_EVS_SendEvent(CONDS_TABLE_VERIFY_ERR_EID, CFE_EVS_ERROR,
                "Invalid Format Table Filename | Index: %d", i);
            Status = CONDS_TABLE_VERIFY_ERR_EID;
        }

        if (g_CONDS_AppData.FrmtTablesInit) {
			//Manage the Format Table
			int32 frmt_status = CONDS_TableManageFrmt(TblPtr[i].FrmtHandle);


			if (frmt_status == CFE_TBL_INFO_UPDATE_PENDING) {

				Status = CFE_TBL_GetAddress((void**) &TblPtr[i].FrmtPtr, TblPtr[i].FrmtHandle);

				if (Status != CFE_SUCCESS || Status != CFE_TBL_INFO_UPDATED) {

					CFE_EVS_SendEvent(CONDS_SUB_STATEMSGS_ERR_EID, CFE_EVS_ERROR,
							"Unable to Access Frmt Table #%d | Error: 0x%08X",
							i, Status);
				}

				// An Update was performed, free memory and allocate new data
				Status = CFE_ES_PutPoolBuf(g_CONDS_AppData.MemPoolHandle,
						TblPtr[i].DataPtr);

				//Get size of new data to store
				int32 size = FRMT_TBL_GetTotalSize((FRMT_TBL_t*) TblPtr[i].FrmtPtr);

				//Allocate memory for new Format Table
				Status = CFE_ES_GetPoolBuf(&TblPtr[i].DataPtr,
											g_CONDS_AppData.MemPoolHandle, size);

				//clear any garbage data in region
				memset(TblPtr[i].DataPtr, 0x00, size);

			}
		}
    } /* End Loop */

    return Status;
} /* End VerifyConState() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_TableManageFrmt() - CONDS frmt table Validation           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_TableManageFrmt(CFE_TBL_Handle_t TblHandle) {
    int32 Status;

    Status = CFE_TBL_GetStatus(TblHandle);
    
    if (Status == CFE_TBL_INFO_DUMP_PENDING) {
        
        // Dump This Particular Format Table
        Status = CFE_TBL_DumpToBuffer(TblHandle);

        if (Status != CFE_SUCCESS) {
        	CFE_EVS_SendEvent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
        			"CONDS: Error during Format Table Dumped to shared buffer | Error: 0x%08X",
					(unsigned int) Status);
        }
        else {
        	CFE_EVS_SendEvent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
        			"CONDS: Format Table Dumped to shared buffer");
        }
        return Status;
        
    }
    else if (Status == CFE_TBL_INFO_VALIDATION_PENDING) {

        //Set flag that validation was complete
        Status = CFE_TBL_Validate(TblHandle);
        if (Status != CFE_SUCCESS) {
        	CFE_EVS_SendEvent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
        			"CONDS: Error during Format Table Validation | Error: 0x%08X",
        			(unsigned int) Status);
        }
        return Status;
    } 
    else if (Status == CFE_TBL_INFO_UPDATE_PENDING) {
    	//Update this Format Table
    	CONDS_DropPtrs();
    	Status = CFE_TBL_Update(TblHandle);
        CONDS_GrabPtrs();

        if (Status != CFE_SUCCESS) {
        	CFE_EVS_SendEvent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
        			"CONDS: Error during Format Table Update | Error: 0x%08X",
					(unsigned int) Status);
        }
        else {
        	CFE_EVS_SendEvent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
        		"CONDS: Format Table Updated");
        	g_CONDS_AppData.HkTlm.FrmtTblLoads++;
        }
        return Status;
    }
    return CFE_SUCCESS;
} /* End of ManageConState() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_TableManageConState() - CONDS ConState table Validation   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CONDS_TableManageConState() {
    int32 Status;

    Status = CFE_TBL_GetStatus(2);//g_CONDS_AppData.ConStateTblHandle);
    

    if (Status == CFE_TBL_INFO_DUMP_PENDING) {
        
        // Dump This Table
        Status = CFE_TBL_DumpToBuffer(g_CONDS_AppData.ConStateTblHandle);
        if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
					"CONDS: Error during ConState Table Dumped to shared buffer | Error: 0x%08X",
					(unsigned int) Status);
		}
		else {
			CFE_EVS_SendEvent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
					"CONDS: ConState Table Dumped to shared buffer");
		}
    }
    else if (Status == CFE_TBL_INFO_VALIDATION_PENDING) {

    	// Validate this Table
    	Status = CFE_TBL_Validate(2);//g_CONDS_AppData.ConStateTblHandle);
    	if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
					"CONDS: Error during ConState Table Validation | Error: 0x%08X",
					(unsigned int) Status);
		}
    } 

    else if (Status == CFE_TBL_INFO_UPDATE_PENDING) {

    	//Clear Saved memory
        CONDS_FreeStateMemory();

        //Update the Table
        CONDS_DropPtrs();

		Status = CFE_TBL_Update(g_CONDS_AppData.ConStateTblHandle);
		if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(CONDS_TABLE_MANAGE_ERR_EID, CFE_EVS_ERROR,
					"CONDS: Error during ConState Table Update | Error: 0x%08X",
					(unsigned int) Status);
		}
		else {
			CFE_EVS_SendEvent(CONDS_TBL_INF_EID, CFE_EVS_INFORMATION,
				"CONDS: ConState Table Updated");
			g_CONDS_AppData.HkTlm.ConStateTblLoads++;

			//Initialize new Format tables, and allocate memory
			CONDS_InitFrmtTables();
			CONDS_AllocateStateMemory();
		}

		CONDS_GrabPtrs();
}
    else {
        return;
    }
} /* End of ManageConState() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_IsValidMessageId - checks if the given message id appears */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean CONDS_IsValidStateMsgId(CFE_SB_MsgId_t MsgId) {
	int32 Status;

	//Make sure we have the table pointers
	Status = CONDS_GrabPtrs();

	if (Status != CFE_SUCCESS) {
		return Status;
	}

	for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {
		if ((g_CONDS_AppData.ConStatePtr)[i].MsgId == MsgId ) {
			return TRUE;
		}
	}
	return FALSE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CONDS_IsValidMessageId - checks if the given message id appears */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CONDS_SaveStateData(CFE_SB_MsgPtr_t MsgPtr) {
	int32 Status;
	int32 bad = 0;

	//Make sure we have the table pointers
	Status = CONDS_GrabPtrs();

	if (Status != CFE_SUCCESS) {
		return Status;
	}

	for (int i = 0; i < CONDS_CONSTATE_TYPE_CNT; i++) {
		if ((g_CONDS_AppData.ConStatePtr)[i].MsgId == CFE_SB_GetMsgId(MsgPtr)) {

			//Found Correct Entry

			//Make Sure Data Ptr is initialized
			if ((g_CONDS_AppData.ConStatePtr)[i].DataPtr == NULL) {
				CFE_EVS_SendEvent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
						"CONDS: Data Received for Pointer not Initialized | MsgId: 0x%08X Error: 0x%08X",
						CFE_SB_GetMsgId(MsgPtr), CONDS_SAVEDATA_ERR_EID);
				bad++;
				continue;
			}


			//Get Size of Data to Copy
			int Size = FRMT_TBL_GetTotalSize((FRMT_TBL_t*)(g_CONDS_AppData.ConStatePtr)[i].FrmtPtr);


			//Check to make sure the sizes of data match
			if (Size != CFE_SB_GetUserDataLength(MsgPtr)) {
				CFE_EVS_SendEvent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
						"CONDS: Save Data size mismatch Expect: %d Actual: %d | MsgId: 0x%08X Error: 0x%08X",
						Size, CFE_SB_GetUserDataLength(MsgPtr),
						CFE_SB_GetMsgId(MsgPtr), CONDS_SAVEDATA_ERR_EID);
				bad++;
				continue;
			}


			//Copy Data
			memcpy((g_CONDS_AppData.ConStatePtr)[i].DataPtr, CFE_SB_GetUserData(MsgPtr), Size);
			//Continue on, in case data needs to be copied to multiple locations
		}

	}

	if (bad !=0) {
		CFE_EVS_SendEvent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
				"CONDS: Data Save Could not save data to %d locations - See Error Log for Details",
				bad);
		return CONDS_SAVEDATA_ERR_EID;
	}
	else {
		return CFE_SUCCESS;
	}
} /* End SaveStateData */


/************************
** END OF FILE COMMENT **
************************/
