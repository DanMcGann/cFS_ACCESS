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
** File Name:  sem_ecec.c
**
** Title:  Source File for SEM Application Executive Functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To provide implementation for functions that facilitate the
** 				validation, and initiation of Satellite Actions planned in the
** 				SEM Schedule table
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#include "sem_exec.h"
#include "sem_tbldefs.h"
#include "sem_app.h"
#include "sem_tables.h"
#include "sem_utils.h"

#include "sc_msgids.h"
#include "sc_msgdefs.h"

/*=====================================================================================
** Name: SEM_ReqPlan
**
** Purpose: Interface with planning layer to request a new plan
**
** Returns:
**    None
**
** Routines Called:
**
** Called By:
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**
** Limitations, Assumptions, External Events, and Notes:
**	This is the main way that the execution and perception layer interacts with
**	the planning layer. The planning layer (regardless of its form of implementation)
**	Should have the means respond to the message generated in this function, and output
**	A satellite plan.
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_ReqPlan(void) {
	/* Create Cmd Header*/
	char msg[CFE_SB_CMD_HDR_SIZE];

	/* Init the Message */
	CFE_SB_InitMsg(&msg,
		APL_PLAN_REQ_MSGID,
		CFE_SB_CMD_HDR_SIZE,
		TRUE);

	/* Set the Message Command Code */
	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&msg, APL_PLAN_REQ_CC);

	/* Send out Msg */
	int32 Status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&msg);

	/* Check results and report */
	if (Status == CFE_SUCCESS) {
		CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
			"SEM: Requesting new Plan from the APL");
	}
	else{
		CFE_EVS_SendEvent(SEM_ERR_EID, CFE_EVS_INFORMATION,
			"SEM: Failed to Request new Plan from the APL");
	}
} /* end SEM_ReqPlan */

/*=====================================================================================
** Name: SEM_CheckPrereqs
**
** Purpose: Checks the prerequisites of the given action against the current state of
** 	of the satellite as reported from the Satellite State Aggregator.
**
** Returns:
**    None
**
** Routines Called:
**
** Called By:
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**
** Limitations, Assumptions, External Events, and Notes:
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean SEM_CheckPrereqs(SEM_ActionId_t id) {
	/* Get and Save the Prereqs of the for the given action */
	SEM_GrabActionTblPtr();
	SEM_ActionTblEntry_t * action;

	action = SEM_GetAction(id);

	/* Action Id is invalid */
	if (action == NULL) {
		return FALSE;
	}

	/* Action is valid - save this actions prerequisites */
	SATCHECK_Checker_t  prereqs[SATCHECK_MAX_ACTION_CHECKS];

	/* Note prereq information is copied - this is done to prevent any chance of a
	 * deadlock. This does so by preventing SEM from requiring both the action table,
	 * and the satellite state format table at the same time, as another application
	 * may.
	 *
	 * By holding only one shared resource at a time, one is always freed before the
	 * other is requested, preventing this deadlocking
	 */

	CFE_PSP_MemCpy(&prereqs,
			action->precheck,
			sizeof(SATCHECK_Checker_t[SATCHECK_MAX_ACTION_CHECKS]));

	/* Release the Action Table */

	SEM_DropActionTblPtr();


	/* Validate all the Prereqs Pass */
	boolean result = TRUE;


	/* For each prereq */
	for (int32 i = 0; i < SATCHECK_MAX_ACTION_CHECKS; i ++) {

		/* If This Checker is not used skip it*/
		if (prereqs[i].FunPtr == NULL) {
			continue;
		}

		/* Get the Sat State Format Table */
		SEM_GrabSatStateFrmtPtr();

		/* Get pointers to all the required data */
		void* params[prereqs[i].NumFields];

		for (int32 j = 0; j < prereqs[i].NumFields; j++) {
			/* Get the offset to the desired data */
			int32 offset = FRMT_TBL_GetOffsetToField(g_SEM_AppData.SatFrmtTblPtr,
								prereqs[i].InputFields[j]);

			/* The Grab of the data was unsuccessful  */
			if (offset < 0) {

				/* report error and return */
				CFE_EVS_SendEvent(SEM_INVALID_ACTION_ERR_EID, CFE_EVS_ERROR,
					"SEM: Could not check state data not available for prereq #%d for action %d",
					i, id);
				return FALSE;
			}

			/* Calculate and Save this pointer in the local list */
			params[j] = (void*) (g_SEM_AppData.SatStatePtr + offset);

			/* Continue for all require params */

		}/* end of parameter gathering */


		/* release the Format table pointer for now */
		SEM_DropSatStateFrmtPtr();

		/* Run checker function on the gathered data */
		result = result && prereqs[i].FunPtr(params);

	} /* end for each prereq check */

	/* Return result of all the prereqs listed for this action */
	return result;
} /* end SEM_CheckPrereqs*/
/*=====================================================================================
** Name: SEM_InitAction()
**
**
** Purpose:
** 		Initiates the execution of the specified action, by sending either the action's
** 		initiation command, or instructing SC to begin the configured RTS.
**
**  Notes:
** 		Actions should only be initiated under the circumstance that all of their
** 		Prerequisite checks successfully pass.
**
**	Parameters: ActionId_t	- The action of which to initiate.
**
=====================================================================================*/
int32 SEM_InitAction(SEM_ActionId_t id) {
	int32 Status;

	/* Get the action table */
	Status = SEM_GrabActionTblPtr();
	if(Status != CFE_SUCCESS) {
		return Status;
	}

	/* Lookup the Action */
	SEM_ActionTblEntry_t * act_ptr;

	act_ptr = SEM_GetAction(id);

	/* Action was found */
	if (act_ptr != NULL) {
		/* Action initializer is a command */
		if (act_ptr->type == CMD) {

			/* Send out the command */
			Status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&(act_ptr->init));
		}
		/* Action initializer is a RTS */
		else {

			/* Create a SC Rts Command */
			char msg[CFE_SB_CMD_HDR_SIZE + sizeof(uint16)];
			CFE_SB_InitMsg(&msg,
					SC_CMD_MID,
					(CFE_SB_CMD_HDR_SIZE + sizeof(uint16)),
					TRUE);

			/* Set Cmd code */
			CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&msg, SC_START_RTS_CC);

			/* Set Cmd Data */
			CFE_PSP_MemCpy(CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&msg),
					&id,
					sizeof(uint16));

			/* Send out the message */
			Status = CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&msg);
		}
	}
	/* Action was not found */
	else {
		Status = SEM_INVALID_ACTION_ERR_EID;
	}

	return Status;
} /* end SEM_InitAction */

/*=====================================================================================
** Name: SEM_StartSchedule
**
** Purpose: Starts execution of the current schedule
**
** Returns:
**    None
**
** Routines Called:
**
** Called By:
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**
** Limitations, Assumptions, External Events, and Notes:
**
** Algorithm:
**   Psuedo-code or description of basic algorithm
**   Starts the Execution of the current schedule, skipping any actions schedules that
**   marked as executed, skipped, or that have occurred in the past.
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_StartSchedule(void) {
	/* Init Local Data */
	CFE_TIME_SysTime_t current_time = CFE_TIME_GetTime();
	SEM_ScheduleTblEntry_t* entry;
	boolean re_entry = TRUE;


	/* Make sure we have pointers for SEM tables */
	SEM_GrabScheduleTblPtr();


	/* Reject any empty schedule */
	if (g_SEM_AppData.ScheduleTblPtr->NumEntries < 1) {

		/* This plan must be a newly scheduled plan - Increment counter*/
		g_SEM_AppData.ScheduleCnt++;
		g_SEM_AppData.ActionsSkipped = 0;

		/* Report error */
		CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
			"SEM: Attempting to start an empty Plan | Plan #%d", g_SEM_AppData.ScheduleCnt);

		/* Document Error */
		g_SEM_AppData.InvalidScheduleCnt++;

		/* Error check and replan */
		SEM_ErrorCheckExit();

		/* Exit this Function */
		return;
	}


	/* Is this a new table or a re entry to a table */

	/* The Schedule is a new schedule */
	if(g_SEM_AppData.ScheduleTblPtr->Schedule[0].Status == SEM_SCHEDULED) {
		/* Update counters */
		g_SEM_AppData.ScheduleCnt++;
		g_SEM_AppData.ActionsSkipped = 0;

		re_entry = TRUE;
	}


	/* Iterate Over the table - Look for actions that are occurring in the future */
	for (int i = 0; i < g_SEM_AppData.ScheduleTblPtr->NumEntries; i ++) {
		entry = &g_SEM_AppData.ScheduleTblPtr->Schedule[i];

		/* This action is marked as scheduled */
		if (entry->Status == SEM_SCHEDULED) {

			/* This action is set to occur in the future */
			if (CFE_TIME_Compare(current_time, entry->StartTime) != CFE_TIME_A_GT_B) {

				/* Set the next action to this index */
				g_SEM_AppData.NextActionIndex = i;

				/* Set Execution flag */
				g_SEM_AppData.ExeScheduleFlag = TRUE;

				/* Exit the Function */
				return;
			}
			/* This Action was scheduled to start at some time in the past */
			else {
				/* Set Status to skipped */
				entry->Status = SEM_SKIPPED;

				/* Increment Skipped Counter */
				g_SEM_AppData.ActionsSkipped++;

				/* Continue looking through the table */
			}
		}
		/* The action being looked at is not scheduled - Move on if possible */
	}

	/* No Future scheduled actions were found in the schedule */

	/* Report */
	CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
		"SEM: Cannot start plan with no scheduled actions - Plan is either stale or improperly created");

	/* Increment plan error counter - if this was a new plan */
	if (!re_entry) {
		g_SEM_AppData.InvalidScheduleCnt++;
	}
	/* Check for critical errors and replan if needed */
	SEM_ErrorCheckExit();

} /* end SEM_StartSchedule */


/*=====================================================================================
** Name: SEM_HaltSchedule
**
** Purpose: Stops execution of the current schedule
**
** Arguments:
**    uint8 AttemptReplan - Indicates that a new plan should be requested if
**    execution of the current schedule did not result from a critical error.
**
** Returns:
**    None
**
** Routines Called:
**
** Called By:
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**
** Limitations, Assumptions, External Events, and Notes:
**
** Algorithm:
**   Psuedo-code or description of basic algorithm
**   Halts the Execution of the current schedule
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_HaltSchedule() {

	/* Set AppData Execution Flags */
	g_SEM_AppData.ExeScheduleFlag = FALSE; /* Stop Schedule Execution */
	CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
		"SEM: Halting Execution of the current Schedule");

	SEM_ErrorCheckExit();
} /* end SEM_HaltSchedule */

/*=====================================================================================
** Name: SEM_ProcessSchedule
**
** Purpose: Initiates actions set to begin execution in this second
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**
** Called By:
**    SEM_RcvMsg()
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**
** Limitations, Assumptions, External Events, and Notes:
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**    1. Querys the Schedule Table for any actions to be initiated in this second
**    		- If So...
**    			- Checks Sat State Prerequsites
**    			- IF prereqs pass -
**    			- sends Action State Checks out
**    			- Sends Action Initiation Command on the Software Bus
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_ProcessSchedule() {
	SEM_ActionId_t ActionToExe; /* The Action Id of the Action To inititate */
	boolean MoreActions = TRUE;
	int32 Status = CFE_SUCCESS;

	/* The Schedule is Not Currently Being Executed */
	if(!g_SEM_AppData.ExeScheduleFlag) {
		return;
	}


	while (MoreActions) {

		/* Query Schedule Table  for the action to execute in this second*/
		ActionToExe = SEM_GetScheduledAction();

		/* The Schedule has no remaining actions scheduled */
		if (ActionToExe == SEM_END_SCH) {
			SEM_HaltSchedule();
			MoreActions = FALSE;
		}
		/* No Action to Execute */
		else if (ActionToExe == SEM_NO_ACTION) {
			MoreActions = FALSE;
		}
		/* An Action was Returned */
		else {

			/*
			 * Process the returned action
			 */

			/* The Action returned is a valid action  */
			if (SEM_IsValidActionId(ActionToExe)) {
				/* Prereqs Passed */
				if (SEM_CheckPrereqs(ActionToExe)) {

					/* Initiate Execution of Action */
					Status = SEM_InitAction(ActionToExe);

					/* The action was successfully initialized */
					if (Status == CFE_SUCCESS) {
						CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
							"SEM: Initiating Action | ActionId: %d", ActionToExe);

						/* set schedule Entry Status */
						SEM_SetSchStatus(g_SEM_AppData.NextActionIndex, SEM_EXECUTED);

						/* Look for other actions to initiate */
						MoreActions = TRUE;
					}
					/* The Action did not Initiate Properly */
					else {
						CFE_EVS_SendEvent(SEM_ERR_EID, CFE_EVS_ERROR,
								"SEM: Initiating Action FAILED | ActionId: %d", ActionToExe);
						g_SEM_AppData.InvalidActionsScheduledCnt++;

						/* set schedule Entry Status */
						SEM_SetSchStatus(g_SEM_AppData.NextActionIndex, SEM_INITFAILED);


						/* Look for other actions to initiate */
						MoreActions = FALSE;
					}


				}
				/* Action prereqs did not pass */
				else {
					CFE_EVS_SendEvent(SEM_ERR_EID, CFE_EVS_ERROR,
						"SEM: Prereq Check Failed - Cannot execute current action");
					g_SEM_AppData.ActionPrereqsFailedCnt++;

					/* set schedule Entry Status */
					SEM_SetSchStatus(g_SEM_AppData.NextActionIndex, SEM_CHECKFAILED);


					SEM_HaltSchedule();
					MoreActions = FALSE;
				}

			}
			/* Action returned is not a valid action */
			else {

				CFE_EVS_SendEvent(SEM_INVALID_ACTION_ERR_EID, CFE_EVS_ERROR,
					"SEM: Invalid Action Scheduled - Current Plan Compromised");

				g_SEM_AppData.InvalidActionsScheduledCnt++;

				/* set schedule Entry Status */
				SEM_SetSchStatus(g_SEM_AppData.NextActionIndex, SEM_SKIPPED);

				SEM_HaltSchedule();
				MoreActions = FALSE;
			}

			/* An Entry was processed increment next action index */
			g_SEM_AppData.NextActionIndex++;
		}
	}/* End of While */
} /* end SEM_ProcessSchedule */



