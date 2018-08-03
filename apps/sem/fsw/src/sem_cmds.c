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


#include "sem_app.h"
#include "sem_cmds.h"
#include "sem_exec.h"
#include "sem_tables.h"
#include "sem_events.h"
#include "sem_utils.h"
/*=====================================================================================
** Name: SEM_SchPlanCmd
**
** Purpose: Handles a SEM_AUTOMODE_CC Command. Controlling SEM operations tate
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
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_AutoModeCmd(CFE_SB_MsgPtr_t MsgPtr) {

	/* check the length of the recieved message */
	if (SEM_VerifyCmdLength(MsgPtr, sizeof(SEM_BooleanCmd_t))) {
		SEM_BooleanCmd_t* msg = (SEM_BooleanCmd_t *) MsgPtr;

		g_SEM_AppData.AutonomyState = msg->AutoMode;

		if(msg->AutoMode) {
			CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
				"SEM: ENTERING AUTONOMOUS OPERATION MODE");
		}
		else {
			CFE_EVS_SendEvent(SEM_INF_EID, CFE_EVS_INFORMATION,
				"SEM: LEAVING AUTONOMOUS OPERATION MODE - User control now required");
		}

	}
	else {
		CFE_EVS_SendEvent(SEM_CMD_ERR_EID, CFE_EVS_ERROR,
			"SEM: Autonomy Mode Command received Cmd with invalid length");
	}
} /* end SEM_AutoModeCmd */

/*=====================================================================================
** Name: SEM_SchPlanCmd
**
** Purpose: Handles the reception of a new plan from the autonomy planning layer
** 	(APL). If the plan should be scheduled, it translates the plan into a SEM Schedule
** 	Table, loads, verifys, and starts execution of that plan.
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
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/

void SEM_SchPlanCmd(CFE_SB_MsgPtr_t MsgPtr) {
	/* Setup Local Data */
	boolean start_exe = FALSE;



	/* Get the Data length of the plan for later verification */
	uint16 plan_data_len = CFE_SB_GetUserDataLength(MsgPtr);

	/* Get Pointer to the Plan Data */
	void * plan_ptr = CFE_SB_GetUserData(MsgPtr);

	/*
	 * Check if this SEM is currently executing a schedule
	 */

	/* SEM is Executing a plan */
	if(g_SEM_AppData.ExeScheduleFlag) {

		/* This is a override plan */
		if (SEM_IsOverridePlan(plan_ptr, plan_data_len)) {
			/* Stop the current plan */
			SEM_HaltSchedule();
			/* Set flag to begin execution of new plan */
			start_exe = TRUE;
		}
		else {
			CFE_EVS_SendEvent(SEM_ERR_EID, CFE_EVS_ERROR,
				"SEM: Non-Override plan received while a plan was executing - Dropping plan");
			g_SEM_AppData.InvalidScheduleCnt++;
			g_SEM_AppData.HkTlm.usCmdErrCnt++;
			start_exe = FALSE;
		}
	}
	/* SEM is not Executing a Plan */
	else {
		start_exe = TRUE;
	}


	/*
	 * Translate and start execution of the plan
	 */

	/* plan should be executed */
	if(start_exe) {

		/* Translate the plan and Load the schedule */
		int32 Status = SEM_TranslateAndLoadPlan(plan_ptr, plan_data_len);
		if (Status == CFE_SUCCESS) {
			/* Begin execution of the plan */
			SEM_StartSchedule();
		}
		else {
			/* report on failure and exit */
			CFE_EVS_SendEvent(SEM_ERR_EID, CFE_EVS_ERROR,
				"SEM: Cannot Start Execution of the plan | Error: 0x%08X", Status);
			g_SEM_AppData.InvalidScheduleCnt++;
			g_SEM_AppData.HkTlm.usCmdErrCnt++;

			SEM_ErrorCheckExit();
		}

	}
} /* end SEM_SchPlanCmd */
