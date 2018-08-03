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
** File Name:  sem_tables.h
**
** Title:  Header File for SEM Application Executive Functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To define the SEM application Executive function prototypes. Executive
** 				functions are all functions that facilitate the processing,
** 				validation, and inititation of actions listed in the SEM Schedule
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#ifndef _SEM_EXEC_H_
#define _SEM_EXEC_H_

/****************************************************************************************
* Includes
****************************************************************************************/
#include "cfe.h"
#include "sem_tbldefs.h"

/****************************************************************************************
*
* Schedule Execution Functions
*
****************************************************************************************/



/*=====================================================================================
** Name: SEM_ReqPlan
**
** \brief Requests a new plan from the satellite's planning layer.
**
** \par Description:
** 		Sends request to the Satellite's planning layer asking to be sent a new plan
** 		of actions.
**
** 		This can be prompted nominally, by a plan ending, or scheduled replan. However,
** 		will also be triggered if a non-critical error occurs that causes the failure
** 		of some scheduled action.
**
=====================================================================================*/
void SEM_ReqPlan(void);

/*=====================================================================================
** Name: SEM_CheckPrereqs()
**
** \brief Validates key state telemetry required for initiation of the given action
**
** \par Description:
** 		Using the Currently available satellite state information, and the configured
** 		action prerequisite list, checks to ensure the current satellite is within safe
** 		parameters to begin execution of the given action.
**
**	\param [in] ActionId_t	- The action of which to check the prerequisites.
**
=====================================================================================*/
boolean SEM_CheckPrereqs(SEM_ActionId_t id);

/*=====================================================================================
** Name: SEM_InitAction()
**
** \brief Begins execution of the given action.
**
** \par Description:
** 		Initiates the execution of the specified action, by sending either the action's
** 		initiation command, or instructing SC to begin the configured RTS.
**
** 	\par Notes, and Assumptions
** 		Actions should only be initiated under the circumstance that all of their
** 		Prerequisite checks successfully pass.
**
**	\param [in] ActionId_t	- The action of which to initiate.
**
**	\returns
**		\retstmt CFE_SUCCESS - The the initialization of the action is successful
**		\retstmt SEM_INVALID_ACTION_ERR_EID - If the initiation of the action fails
**  \endreturns
=====================================================================================*/
int32 SEM_InitAction(SEM_ActionId_t id);

/*=====================================================================================
** Name: SEM_StartSchedule
**
** \brief Starts execution of the current schedule
**
** \par Description:
** 		Starts the execution of the current plan. On start skips all actions listed
** 		that were scheduled to be initiated in the past. Can be used to restart a
** 		previously halted plan, but will skip any actions that were schedules to
** 		execute while the schedule was not being executed.
**
** \par Developer Notes:
** 		If you are developing a planning algorithm for use on the access framework
** 		planner should allow for time in between the completion of the planning process
** 		the first action to execute so the ACCESS Framework has time to accept,
** 		translate, schedule, and begin execution of that plan. The exact time is
** 		not provided, as it will depend on the length and format of the plan, as well
** 		as the processor which is running this software.
**
=====================================================================================*/
void SEM_StartSchedule(void);

/*=====================================================================================
** Name: SEM_HaltSchedule
**
** \brief Stops execution of the current schedule
**
** \par Description:
** 		Stops the execution of the current schedule. Additionally, this function
** 		if the halt was caused by some critical error. If there is not a critical
** 		error on request will initiate a replan for the satellite from the
** 		satellite's Planning Layer to replace the now compromised plan
**
** \param   uint8 AttemptReplan - Indicates that a new plan should be requested if
**    			execution of the current schedule did not result from a critical error.
**
=====================================================================================*/
void SEM_HaltSchedule(void);

/*=====================================================================================
** Name: SEM_ProcessSchedule
**
** 	\brief Initiates actions set to begin execution in this second
**
**	\par Description:
**		Main handler of the execution of the current schedule. Provides robust
**		mechanism to initiate each scheduled command. This function is
**		run on every wake up of the SEM application (1 Hz).
**
**=====================================================================================*/
void SEM_ProcessSchedule(void);


#endif /* _SEM_EXEC_H_ */
