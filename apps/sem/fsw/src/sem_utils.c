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
** File Name:  sem_utils.c
**
** Title:  Source File for SEM Application Utility Functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To provide implementation SEM utility funcitions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
#include "cfe.h"
#include "sem_app.h"
#include "sem_exec.h"
#include "sem_events.h"
#include "sem_mission_cfg.h"


/*=====================================================================================
** Name: SEM_ErrorCheckExit
**
** Purpose: Checks for critical planning errors, and guide satellite to next state.
** 	This function is used to exit from the SEM executive functions. Each exit has
** 	requires the same common checks, and a request for a new plan if no critical
** 	errors and in autonomous mode.
**
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
void SEM_ErrorCheckExit(void) {
	/*
	 * Check to see if Exit was caused because of an error
	 */
	/* Has SEM received too many invalid schedules */
	if (g_SEM_AppData.InvalidScheduleCnt > SEM_MAX_ACCEPT_INVALID_PLANS) {
		/*
		 * Report and go to safe mode
		 */
		CFE_EVS_SendEvent(SEM_CRIT_ERR_EID, CFE_EVS_ERROR,
			"SEM: Invalid Schedules Limit Reached - Going to Safe Mode");
		ACCESS_ENTER_SAFE_MODE();
		return;
	}
	/* The planner or something else has scheduled too many invalid plans */
	else if (g_SEM_AppData.InvalidActionsScheduledCnt > SEM_MAX_ACCEPT_INVALID_ACTIONS) {

		/*
		 * Report and go to safe mode
		 */
		CFE_EVS_SendEvent(SEM_CRIT_ERR_EID, CFE_EVS_ERROR,
			"SEM: Invalid Action Limit Reached - Going to Safe Mode");
		ACCESS_ENTER_SAFE_MODE();
		return;

	}
	/* Too many actions have failed prereqs, Something is wrong */
	else if (g_SEM_AppData.ActionPrereqsFailedCnt > SEM_MAX_ACCEPT_PREREQ_FAILURES) {

		/*
		 * Report and go to safe mode
		 */
		CFE_EVS_SendEvent(SEM_CRIT_ERR_EID, CFE_EVS_ERROR,
			"SEM: Action Prereq Failure Limit Reached - Going to Safe Mode");
		ACCESS_ENTER_SAFE_MODE();
		return;

	}
	/* No Critical Error - attempt replan if in autonomous mode */
	else if (g_SEM_AppData.AutonomyState) {
		SEM_ReqPlan();
	}
} /* end SEM_ErrorCheckExit */
