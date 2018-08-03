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
** File Name:  sem_tables.c
**
** Title:  Source File for SEM Application Tables functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To provide implementation for  SEM's table relating functions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
#include "sem_tables.h"
#include "sem_app.h"

//For shared table information
#include "sa_platform_cfg.h"

/****************************************************************************************
* SEM_InitTables - Initializes Tables
****************************************************************************************/
int32 SEM_InitTables(void) {
	int32 Status1;
	int32 Status2;
	int32 Status3;

	/* Initialize and Load Tables */
	Status1 = SEM_InitActionTable();
	Status2 = SEM_InitScheduleTable();
	Status3 = SEM_InitPlanFrmtTable();


	if(Status1 != CFE_SUCCESS || Status2 != CFE_SUCCESS || Status3 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Table Init Failed - SEM requires tables for functionality - STOP");
		return SEM_INIT_TBL_ERR_EID;
	}

	/*
	 * Register Share Access to Satellite State Format Table
	 */
	char buff[100];
	sprintf(buff, "SA_APP.%s", SA_SATFRMT_TBLNAME);

	Status1 = CFE_TBL_Share(&g_SEM_AppData.SatFrmtTblHandle,
						buff);

	if (Status1 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Could Not Share Satellite State Format Table | Error: 0x%08X",
			Status1);
	}
	return Status1;
} /* end SEM_InitTables */

/****************************************************************************************
* SEM_InitActionTable - Initializes Action Table
****************************************************************************************/
int32 SEM_InitActionTable(void) {
	int32 Status = CFE_SUCCESS;
	uint16 ActionTblFlags =  CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
	boolean NeedToLoadAction = FALSE;


	#if (SEM_MAKE_TABLES_CRITICAL == 1)
	ActionTblFlags |= CFE_TBL_OPT_CRITICAL;
	#endif

	/*
	**  Register the Action Table with Table Services
	*/
	Status = CFE_TBL_Register(&g_SEM_AppData.ActionTblHandle,
								SEM_ACTIONTBL_NAME,
								sizeof(SEM_ActionTbl_t),
								ActionTblFlags,
								(CFE_TBL_CallbackFuncPtr_t) SEM_VerifyActionTable);

	if(Status == CFE_TBL_INFO_RECOVERED_TBL) {

		//Table is registered and resorted from data
		NeedToLoadAction = FALSE;
		CFE_EVS_SendEvent(SEM_INIT_TBL_CDS_EID, CFE_EVS_INFORMATION,
			"SEM: Action Table recovered from CDS");
		return Status;
	}
	else if ( (Status == CFE_SUCCESS) ) {
		//Table registered and needs loading
		NeedToLoadAction = TRUE;
	}
	else {
		//Table registration failed
		CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Unable to register Action Table, Error = 0x%08X",
			(unsigned int) Status);
		return Status;
	}   //Done With Registration process


	/*
	**  Load the Action Table
	*/
	if (NeedToLoadAction) {
		Status = CFE_TBL_Load(g_SEM_AppData.ActionTblHandle,
				CFE_TBL_SRC_FILE,
				SEM_ACTIONTBL_FILENAME);

		if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
				"SEM: Unable to load default Action Table: Filename = '%s' | Error: 0x%08X",
				SEM_ACTIONTBL_FILENAME, Status);
		}
	} //Done with load process

	return Status;
} /* end SEM_InitActionTable */


/****************************************************************************************
* SEM_InitScheduleTable - Initializes Action Table
****************************************************************************************/
int32 SEM_InitScheduleTable(void) {
	int32 Status = CFE_SUCCESS;
	uint16 ScheduleTblFlags =  CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
	boolean NeedToLoadSchedule = FALSE;

	#if (SEM_MAKE_TABLES_CRITICAL == 1)
	ScheduleTblFlags |= CFE_TBL_OPT_CRITICAL;
	#endif

	/*
	 * Register the Schedule Table with Table services
	 */
	Status = CFE_TBL_Register(&g_SEM_AppData.ScheduleTblHandle,
								SEM_SCHEDULETBL_NAME,
								sizeof(SEM_ActionTbl_t),
								ScheduleTblFlags,
								(CFE_TBL_CallbackFuncPtr_t) SEM_VerifyScheduleTable);

	if(Status == CFE_TBL_INFO_RECOVERED_TBL) {
		//Table is registered and resorted from data
		NeedToLoadSchedule = FALSE;
		CFE_EVS_SendEvent(SEM_INIT_TBL_CDS_EID, CFE_EVS_INFORMATION,
			"SEM: Schedule Table recovered from CDS");
	}
	else if ( (Status == CFE_SUCCESS) ) {
		//Table registered and needs loading
		NeedToLoadSchedule = TRUE;
	}
	else {
		//Table registration failed
		CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Unable to register Schedule Table, Error = 0x%08X",
			(unsigned int) Status);
		return Status;
	}   //Done With Registration process


	/*
	**  Load the Schedule Table
	*/
	if (NeedToLoadSchedule && SEM_SCHEDULETBL_INITDEFAULT) {
		Status = CFE_TBL_Load(g_SEM_AppData.ScheduleTblHandle,
				CFE_TBL_SRC_FILE,
				SEM_SCHEDULETBL_FILENAME);

		if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
				"SEM: Unable to load default Schedule Table: Filename = '%s' | Error: 0x%08X",
				SEM_SCHEDULETBL_FILENAME, Status);
		}
	} //Done with load process
	return Status;
} /* end SEM_InitScheduleTable */

/****************************************************************************************
* SEM_InitPlanFrmtTable - Initializes Action Table
****************************************************************************************/
int32 SEM_InitPlanFrmtTable(void) {
	int32 Status = CFE_SUCCESS;
	uint16 PlanFrmtTblFlags =  CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_LOAD_DUMP;
	boolean NeedToLoadSchedule = FALSE;

	#if (SEM_MAKE_TABLES_CRITICAL == 1)
	PlanFrmtTblFlags |= CFE_TBL_OPT_CRITICAL;
	#endif

	/*
	 * Register the Schedule Table with Table services
	 */
	Status = CFE_TBL_Register(&g_SEM_AppData.PlanFrmtTblHandle,
								SEM_PLANFRMTTBL_NAME,
								(sizeof(FRMT_TBL_t) + FRMT_TBL_ENTRY_SIZE * SEM_PLANFRMTTBL_ENTRIES),
								PlanFrmtTblFlags,
								(CFE_TBL_CallbackFuncPtr_t) &SEM_VerifyPlanFrmtTable);

	if(Status == CFE_TBL_INFO_RECOVERED_TBL) {
		//Table is registered and resorted from data
		NeedToLoadSchedule = FALSE;
		CFE_EVS_SendEvent(SEM_INIT_TBL_CDS_EID, CFE_EVS_INFORMATION,
			"SEM: Plan Format Table recovered from CDS");
	}
	else if ( (Status == CFE_SUCCESS) ) {
		//Table registered and needs loading
		NeedToLoadSchedule = TRUE;
	}
	else {
		//Table registration failed
		CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Unable to register Plan Format Table, Error = 0x%08X",
			(unsigned int) Status);
		return Status;
	}   //Done With Registration process


	/*
	**  Load the Schedule Table
	*/
	if (NeedToLoadSchedule) {
		Status = CFE_TBL_Load(g_SEM_AppData.PlanFrmtTblHandle,
				CFE_TBL_SRC_FILE,
				SEM_PLANFRMTTBL_FILENAME);

		if (Status != CFE_SUCCESS) {
			CFE_EVS_SendEvent(SEM_INIT_TBL_ERR_EID, CFE_EVS_ERROR,
				"SEM: Unable to load default Plan Format Table: Filename = '%s' | Error: 0x%08X",
				SEM_PLANFRMTTBL_FILENAME, Status);
		}
	} //Done with load process
	return Status;
} /* end SEM_InitPlanFrmtTable */

/****************************************************************************************
* SEM_ManageTables - Manages Table
****************************************************************************************/
int32 SEM_ManageTables(void) {
	/* init Local Data */
	int32 Status;

	SEM_DropActionTblPtr();
	/*
	 * Manage Action Table
	 */

	/* Default CFE Table Management */
	Status = CFE_TBL_Manage(g_SEM_AppData.ActionTblHandle);

	if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED) {
		CFE_EVS_SendEvent(SEM_MANAGE_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Action Table Management Failed | Error: 0x%08X", Status);
	}

	/*
	 * Manage Schedule Table
	 */
	SEM_DropScheduleTblPtr();
	/* Get the table status */
	Status = CFE_TBL_GetStatus(g_SEM_AppData.ScheduleTblHandle);

	/* Nothing To Do */
	if (Status == CFE_SUCCESS) {
		/* NADA */
	}
	/* The Table Needs to be Validated */
	else if (Status == CFE_TBL_INFO_VALIDATION_PENDING) {
		/* Validate Table */
		CFE_TBL_Validate(g_SEM_AppData.ScheduleTblHandle);
	}
	/* A new table has been loaded */
	else if (Status == CFE_TBL_INFO_UPDATE_PENDING) {
		/* The Schedule is still set to be executed */
		if (g_SEM_AppData.ExeScheduleFlag) {

			/* Report error */
			CFE_EVS_SendEvent(SEM_INVALID_SCHLOAD_ERR_EID, CFE_EVS_ERROR,
				"SEM: Schedule Load attempt while current schedule is executing - Blocking Load");

			/* Do not load, exit function instead */
			Status = SEM_INVALID_SCHLOAD_ERR_EID;
		}
		/* There is no schedule executing, continue with update */
		else {
			CFE_TBL_Update(g_SEM_AppData.ScheduleTblHandle);
		}
	}
	/* The ground has requested a dump of the table */
	else if (Status == CFE_TBL_INFO_DUMP_PENDING) {
		/* Dump the Table */
		CFE_TBL_DumpToBuffer(g_SEM_AppData.ScheduleTblHandle);
	}
	/* Something has gone wrong */
	else {
		/* Report error */
		CFE_EVS_SendEvent(SEM_INVALID_SCHLOAD_ERR_EID, CFE_EVS_ERROR,
			"SEM: Table Management has failed | Error: 0x%08X", Status);
	}

	/* Retrieve dropped pointers */
	SEM_GrabScheduleTblPtr();

	/* Exit */
	return Status;
} /* end SEM_ManageTables */

/****************************************************************************************
* SEM_VerifyActionTable - Verifies Action Table
****************************************************************************************/
int32 SEM_VerifyActionTable(void* TblPtr) {

	/* Entry Counters */
	int32 num_used = 0;
	int32 good = 0;
	int32 bad = 0;


	SEM_ActionTbl_t * tbl = (SEM_ActionTbl_t *) TblPtr;

	/* For each entry in the table */
	for (int i = 0; i < SEM_MAX_ACTIONS; i++) {

		/* Is this entry used */
		if (tbl->Actions[i].id != SEM_UNUSED_ENTRY) {
			/* Increment counter */
			num_used++;

			/* Validate the Action Data */
			if (tbl->Actions[i].type == CMD) {
				/* Action is of type command */

				/* Validate checksum */
				if(CFE_SB_ValidateChecksum((CFE_SB_MsgPtr_t)&tbl->Actions[i].init)) {
					/* Checksum is good */
					good++;
				}
				else {
					/* Checksum us and command is invalid */
					bad++;
				}

			}
			/* Action is of type RTS */
			else {
				good++; /* RTS are verified independently by the SC App */
			}
			/* validate Checkers */
			//ADDME - Check validation
		}

	}

	/* Validate the number of used entries */
	if (num_used != tbl->NumEntries) {

		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_INFORMATION,
			"SEM: Action Table Verification num entires does not match number of used entries | Non-Fatal Error");
	}


	/* Report Results */

	/* Verification Failed */
	if (bad != 0) {

		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Action Table Verification Failed | Good: %d, Bad: %d", good, bad);

		return SEM_VERIFY_TBL_ERR_EID;
	}
	/* Verification passed */
	else {

		CFE_EVS_SendEvent(SEM_VERIFY_TBL_INF_EID, CFE_EVS_ERROR,
			"SEM: Action Table Verification Success | Good: %d, Bad: %d", good, bad);

		return CFE_SUCCESS;
	}
} /* end SEM_VerifyActionTable */

/****************************************************************************************
* SEM_VerifyScheduleTable - Verifies Schedule Table
****************************************************************************************/
int32 SEM_VerifyScheduleTable(void* TblPtr) {

	/* Entry Counters */
	int32 num_used =0;
	int32 good = 0;
	int32 bad = 0;

	SEM_ScheduleTbl_t * tbl = (SEM_ScheduleTbl_t *) TblPtr;

	/* for each entry in the table */
	for (int i = 0; i < SEM_MAX_SCHEDULE_LEN; i++) {
		/* This entry is being used */
		if (tbl->Schedule[i].Status != SEM_UNUSED) {
			num_used++;

			/* This entry is going to be initiated */
			if (tbl->Schedule[i].Status == SEM_SCHEDULED) {

				/* The Action is a valid action */
				if (SEM_IsValidActionId(tbl->Schedule[i].Action)) {
					good++;
				}
				/* The action is not a valid action */
				else {
					bad++;
				}
			}

		}

	}

	/* Validate the number of used entries */
	if (num_used != tbl->NumEntries) {

		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_INFORMATION,
			"SEM: Schedule Table Verification num entires does not match number of used entries | Non-Fatal Error");
	}


	/* Report Results */

	/* Verification Failed */
	if (bad != 0) {

		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Schedule Table Verification Failed | %d Invalid Actions Scheduled", bad);

		return SEM_VERIFY_TBL_ERR_EID;
	}
	/* Verification passed */
	else {

		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Schedule Table Verification Success | Good: %d, Bad: %d", good, bad);

		return CFE_SUCCESS;
	}

} /* end SEM_VerifyScheduleTable */

/****************************************************************************************
* SEM_VerifyPlanFrmtTable - Verifies Plan format Table
****************************************************************************************/
int32 SEM_VerifyPlanFrmtTable(void* TblPtr) {

	FRMT_TBL_t* tbl = (FRMT_TBL_t*)TblPtr;
	if (FRMT_TBL_GetFlaggedFieldId(tbl, SEM_PFT_ACTION_ID_FLAG) < 0 )
	{
		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Plan Format Table does not contain entry with [REQUIRED] SEM_PFT_ACTION_ID_FLAG");

		return SEM_VERIFY_TBL_ERR_EID;
	}

	if (FRMT_TBL_GetFlaggedFieldId(tbl, SEM_PFT_START_TIME_FLAG) < 0 )
		{
		CFE_EVS_SendEvent(SEM_VERIFY_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Plan Format Table does not contain entry with [REQUIRED] SEM_PFT_START_TIME_FLAG");

			return SEM_VERIFY_TBL_ERR_EID;
		}
	return CFE_SUCCESS;
} /* end SEM_VerifyPlanFrmtTable */

/****************************************************************************************
* SEM_GrabPtrs
****************************************************************************************/
int32 SEM_GrabScheduleTblPtr(void) {
	int32 Status2;

	/* Grab Pointer for Schedule Table */
	Status2 = CFE_TBL_GetAddress((void **)&g_SEM_AppData.ScheduleTblPtr,
				g_SEM_AppData.ScheduleTblHandle);

	/* failed for at least one table */
	if ( Status2 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Schedule Table | Error: 0x%08X", Status2);
		return Status2;
	}

	/* grab Pointer succeeded */
	return CFE_SUCCESS;
} /* end SEM_GrabSchedulePtr */

int32 SEM_GrabActionTblPtr(void) {
	int32 Status1;

	/* Grab Pointer for Schedule Table */
	Status1 = CFE_TBL_GetAddress((void **)&g_SEM_AppData.ActionTblPtr,
				g_SEM_AppData.ActionTblHandle);

	/* failed for at least one table */
	if (Status1 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Action Table | Error: 0x%08X", Status1);
		return Status1;
	}

	/* grab Pointer succeeded */
	return CFE_SUCCESS;
} /* end SEM_GrabActionPtr */

int32 SEM_GrabPlanFrmtTblPtr(void) {
	int32 Status1;

	/* Grab Pointer for Schedule Table */
	Status1 = CFE_TBL_GetAddress((void **)&g_SEM_AppData.PlanFrmtTblPtr,
				g_SEM_AppData.PlanFrmtTblHandle);

	/* failed for at least one table */
	if (Status1 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Plan Format Table | Error: 0x%08X", Status1);
		return Status1;
	}

	/* grab Pointer succeeded */
	return CFE_SUCCESS;
} /* end SEM_GrabActionPtr */


/****************************************************************************************
* SEM_GrabSatStateFrmtPtrs
****************************************************************************************/
int32 SEM_GrabSatStateFrmtPtr(void) {
	int32 Status;

	/* Grab Pointer for Schedule Table */
	Status = CFE_TBL_GetAddress((void **)&g_SEM_AppData.SatFrmtTblPtr,
				g_SEM_AppData.SatFrmtTblHandle);


	/* failed for at least one table */
	if (Status != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Grab Pointers Failed for Sat State Format Table | Error: 0x%08X", Status);
		return Status;
	}

	/* grab Pointer succeeded */
	return CFE_SUCCESS;
} /* end SEM_GrabSatStateFrmtPtrs */

/****************************************************************************************
* SEM_GrabPtrs
****************************************************************************************/
int32 SEM_DropScheduleTblPtr(void) {
	int32 Status2;

	/* Grab Pointer for Schedule Table */
	Status2 = CFE_TBL_ReleaseAddress(g_SEM_AppData.ScheduleTblHandle);

	/* failed for at least one table */
	if (Status2 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Schedule Table | Error: 0x%08X", Status2);
		return Status2;
	}

	/* Drop Pointer succeeded */
	g_SEM_AppData.ScheduleTblPtr = NULL;
	return CFE_SUCCESS;
} /* end SEM_DropSchedulePtr */

int32 SEM_DropActionTblPtr(void) {
	int32 Status1;

	/* Grab Pointer for Schedule Table */
	Status1 = CFE_TBL_ReleaseAddress(g_SEM_AppData.ActionTblHandle);

	/* failed for at least one table */
	if (Status1 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Action Table | Error: 0x%08X", Status1);
		return Status1;
	}

	/* Drop Pointer succeeded */
	g_SEM_AppData.ActionTblPtr = NULL;
	return CFE_SUCCESS;
} /* end SEM_DropActionPtr */

int32 SEM_DropPlanFrmtTblPtr(void) {
	int32 Status1;

	/* Grab Pointer for Schedule Table */
	Status1 = CFE_TBL_ReleaseAddress(g_SEM_AppData.PlanFrmtTblHandle);

	/* failed for at least one table */
	if (Status1 != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Plan Format Table | Error: 0x%08X", Status1);
		return Status1;
	}

	/* Drop Pointer succeeded */
	g_SEM_AppData.PlanFrmtTblPtr = NULL;
	return CFE_SUCCESS;
} /* end SEM_DropPlanFrmtTblPtr */

/****************************************************************************************
* SEM_GrabSatStateFrmtPtrs
****************************************************************************************/
int32 SEM_DropSatStateFrmtPtr(void) {
	int32 Status;

	/* Grab Pointer for Schedule Table */
	Status = CFE_TBL_ReleaseAddress(g_SEM_AppData.SatFrmtTblHandle);


	/* failed for at least one table */
	if (Status != CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_GRABPTRS_TBL_ERR_EID, CFE_EVS_ERROR,
			"SEM: Drop Pointers Failed for Sat State Format Table | Error: 0x%08X", Status);
		return Status;
	}

	/* Drop Pointer succeeded */
	g_SEM_AppData.SatFrmtTblPtr = NULL;
	return CFE_SUCCESS;
} /* end SEM_DropSatStateFrmtPtr */

/****************************************************************************************
* SEM_IsValidActionsId
****************************************************************************************/
boolean SEM_IsValidActionId(SEM_ActionId_t id) {
	boolean Result = FALSE;

	/* Is action table loaded and retrievable */
	if (SEM_GrabActionTblPtr() != CFE_SUCCESS) {
		return FALSE;
	}

	/* Look for Action Id in the Table */
	for(int i = 0; i < g_SEM_AppData.ActionTblPtr->NumEntries; i++) {
		/* This entry has the id that we are looking for */
		if (g_SEM_AppData.ActionTblPtr->Actions[i].id == id) {
			Result =  TRUE;
		}
	}

	/* Release the Action table pointer */
	SEM_DropActionTblPtr();

	return Result;
} /* end SEM_IsValidActionsId */


/****************************************************************************************
* SEM_IsValidActionsId
****************************************************************************************/
boolean SEM_IsOverridePlan(void* PlanPtr, int32 Len) {
	int32 entry_size;

	/* Get the Plan format Table */
	if(SEM_GrabPlanFrmtTblPtr() == CFE_SUCCESS) {
		entry_size = FRMT_TBL_GetTotalSize(g_SEM_AppData.PlanFrmtTblPtr);

		/* The Plan is bigger than at least 1 entry */
		if (Len >= entry_size) {
			/* the first entry has a field that evaluates to true */
			int32 id = FRMT_TBL_GetFlaggedFieldId(g_SEM_AppData.PlanFrmtTblPtr,
					SEM_PFT_OVERRIDE_FLAG);

			/* Flag Exists */
			if (id > 0) {
				boolean ret = FALSE;
				FRMT_TBL_GetFieldData(g_SEM_AppData.PlanFrmtTblPtr,
						PlanPtr,
						id,
						&ret);

				return ret;
			}
		}
	}
	/* The Plan is either malformed or is not an override plan */
	return FALSE;
} /* end */


/****************************************************************************************
* SEM_GetScheduledAction
****************************************************************************************/
SEM_ActionId_t SEM_GetScheduledAction(void) {
	/* setup local Data */
	SEM_ScheduleTbl_t * sch;
	SEM_ScheduleTblEntry_t *act;

	int32 next = g_SEM_AppData.NextActionIndex;

	SEM_ActionId_t ret = SEM_NO_ACTION;

	CFE_TIME_SysTime_t now = CFE_TIME_GetTime();
	CFE_TIME_SysTime_t onesec = {1, 0};

	/* Get Action Table Pointers */
	if(SEM_GrabScheduleTblPtr() == CFE_SUCCESS) {
		/* temp pointer to table */
		sch = g_SEM_AppData.ScheduleTblPtr;

		/* The next index is in bounds */
		if (next < sch->NumEntries) {
			/* get pointer to entry */
			act = &sch->Schedule[next];

			/* The action is set to execute in this second*/
			if ((CFE_TIME_Compare(act->StartTime, now) == CFE_TIME_A_LT_B ||
					(CFE_TIME_Compare(act->StartTime, now) == CFE_TIME_EQUAL)) &&
				CFE_TIME_Compare(CFE_TIME_Subtract(now, act->StartTime), onesec) == CFE_TIME_A_LT_B)
			{
				/* Return the Action */
				ret = act->Action;
			}

		}
		/* the next index is out of bounds */
		else {

			ret = SEM_END_SCH;

		}
	}
	/* Action Table Pointer inaccessible */
	else {
		ret =  SEM_NO_ACTION;
	}

	return ret;

} /* end SEM_GetScheduledAction */

/****************************************************************************************
* SEM_GetAction
****************************************************************************************/
SEM_ActionTblEntry_t * SEM_GetAction(SEM_ActionId_t Id) {
	SEM_ActionTbl_t * tbl;
	SEM_ActionTblEntry_t * ret = NULL;

	/* get the pointer to the table */
	if (SEM_GrabActionTblPtr() == CFE_SUCCESS) {
		tbl = g_SEM_AppData.ActionTblPtr;

		/* for each entry in the table */
		for (int i = 0; i < tbl->NumEntries; i++) {

			/* this is the action we are looking for */
			if (tbl->Actions[i].id == Id) {
				return &(tbl->Actions[i]);
			}
		}
	}

	return ret;
} /* end SEM_GetAction */


/****************************************************************************************
* SEM_SetSchStatus
****************************************************************************************/
void SEM_SetSchStatus(int32 index, enum SEM_ActionStatus_t status) {
	if (SEM_GrabScheduleTblPtr() == CFE_SUCCESS) {

		SEM_ScheduleTbl_t * sch = g_SEM_AppData.ScheduleTblPtr;

		/* Is the index in bounds */
		if (index < sch->NumEntries) {
			sch->Schedule[index].Status = status;
		}
	}
} /* end SEM_SetSchStatus */



/****************************************************************************************
* SEM_TranslateAndLoadPlan()
****************************************************************************************/
int32 SEM_TranslateAndLoadPlan(void* PlanPtr, int32 PlanDataLen) {
	/* setup local data */
	int32 Status;
	SEM_ScheduleTbl_t new_sch;

	FRMT_TBL_t * frmt_ptr;
	int32 plan_entry_len;
	int32 num_plan_entries;

	/* Get the Pointer to the Plan Format Table */
	if (SEM_GrabPlanFrmtTblPtr() == CFE_SUCCESS) {

		/*
		 * Calculate local data
		 */

		/* save plan format table pointer */
		frmt_ptr = g_SEM_AppData.PlanFrmtTblPtr;

		/* Get the Size of the a plan Entry */
		plan_entry_len = FRMT_TBL_GetTotalSize(frmt_ptr);


		/* Get the number of plan entries*/
		num_plan_entries = PlanDataLen / plan_entry_len;


		/*
		 * The Plan is Empty
		 */
		if (num_plan_entries < 1) {
			return SEM_INVALID_SCHLOAD_ERR_EID;
		}
		else if (num_plan_entries > SEM_MAX_SCHEDULE_LEN) {
			CFE_EVS_SendEvent(SEM_INVALID_SCHLOAD_ERR_EID, CFE_EVS_ERROR,
					"SEM: Plan received plan that is too long, cannot schedule last %d Actions",
					(num_plan_entries - SEM_MAX_SCHEDULE_LEN));
		}


		/*
		 * Enter All Plan information into
		 */

		/* field id for start time info */
		int32 f_st = FRMT_TBL_GetFlaggedFieldId(frmt_ptr, SEM_PFT_START_TIME_FLAG);

		/* field id for action id field */
		int32 f_a = FRMT_TBL_GetFlaggedFieldId(frmt_ptr, SEM_PFT_ACTION_ID_FLAG);

		/* for each entry in the provided plan */
		for (int i = 0; i < num_plan_entries; i++) {
			/* set the start time */
			FRMT_TBL_GetFieldData(frmt_ptr,				// Pointer to Plan Frmt Tbl
					(PlanPtr + (i * plan_entry_len)),   // Pointer to "i"th plan entry
					f_st,								// Start time field
					&(new_sch.Schedule[i].StartTime));	// Save location

			/* set the action id */
			FRMT_TBL_GetFieldData(frmt_ptr,				// Pointer to Plan Frmt Tbl
					(PlanPtr + (i * plan_entry_len)),   // Pointer to "i"th plan entry
					f_a,								// Start time field
					&(new_sch.Schedule[i].Action));		// Save location

			/* set the Entry Status */
			new_sch.Schedule[i].Status = SEM_SCHEDULED;

			/* increment the schedule table number of entries */
			new_sch.NumEntries++;
		}

		/* Drop the Plan Format Table */
		SEM_DropPlanFrmtTblPtr();


		/*
		 * Load the Table with CFE Table Services
		 */

		/* Make sure the Schedule Table is dropped */
		SEM_DropScheduleTblPtr();

		/* Load the New Table */
		Status = CFE_TBL_Load(g_SEM_AppData.ScheduleTblHandle,
				CFE_TBL_SRC_ADDRESS,
				&new_sch);

		/* Grab the schedule table agian */
		SEM_GrabScheduleTblPtr();
	}
	/* Grabbing the Pointers has failed */
	else {
		Status = SEM_GRABPTRS_TBL_ERR_EID;
	}

	/* report resort */
	return Status;
} /* end SEM_TranslateAndLoadPlan */


/****************
 * End of File  *
 ****************/

