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
** File Name:  sem_cmds.h
**
** Title:  Header File for SEM Application Commands
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To define SEM's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#ifndef _SEM_CMDS_H_
#define _SEM_CMDS_H_

#include "cfe.h"

/*=====================================================================================
** Name: SEM_AutoModeCmd()
**
** 	\brief	This commands sets SEM's mode.
**
**	\par Description: Sets SEM mode of operation to autonomous mode. A user data
**	value of TRUE sets SEM to autonomous mode, and a user data value of FALSE, sets SEM
**	to Non-Autonomous Mode.
**=====================================================================================*/
void SEM_AutoModeCmd(CFE_SB_MsgPtr_t MsgPtr);


/*=====================================================================================
** Name: SEM_SchPlanCmd
**
** 	\brief Handles the reception of a new plan from the APL
**
**	\par Description: This function coordinates the reception, translation and
**	start of a satellite schedule of actions, based on a plan recieved from the
**	satellite's Autonomy Planning Layer (APL).
**
**	\Par Notes: A plan should normally only be received and started if no existing
**	schedule is being executed. If a plan is reviewed during the execution of an existing
**	schedule, the function will look for a boolean flag indicating this plan should
**	override any existing plan. If that flag is false, or does not exist the plan
**	will be treated as an error by the APL and thrown away.
**
**=====================================================================================*/
void SEM_SchPlanCmd(CFE_SB_MsgPtr_t MsgPtr);

#endif /* _SEM_CMDS_H_ */
