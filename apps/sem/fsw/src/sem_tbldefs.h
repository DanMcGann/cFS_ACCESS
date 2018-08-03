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
** File Name:  sem_tbldefs.h
**
** Title:  Header File for SEM Application Table Data Definitions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To define SEM's table data types and structures
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#ifndef _SEM_TBLDEFS_H_
#define _SEM_TBLDEFS_H_
/****************************************************************************************
* Includes
****************************************************************************************/
#include "satcheck_lib.h"
#include "sem_platform_cfg.h"
#include "sem_mission_cfg.h"


/****************************************************************************************
*
*
*Application Format Table Flags
*
*
****************************************************************************************/

/* Plan Format Table Flags */
/* REQUIRED */
#define SEM_PFT_START_TIME_FLAG 	1
#define SEM_PFT_ACTION_ID_FLAG 		2

/* OPTIONAL */
#define SEM_PFT_OVERRIDE_FLAG 		3

/****************************************************************************************
*
*
* Table Data Definitions
*
*
****************************************************************************************/



/****************************************************************************************
* Action Table
****************************************************************************************/

/* MACROS */
#define SEM_UNUSED_ENTRY 	(-1)
#define SEM_NO_ACTION 		(-2)
#define SEM_END_SCH			(-3)

/* \brief Type for action unique identifier */
typedef int32 SEM_ActionId_t;



/* Enumeration of types of actions that can be executed */
enum SEM_ActionType_t{
	CMD = 0, /* << \brief Flag Indicating Action is initiated by a Command Message */
	RTS = 1/* << \brief Flag Indicating that the action is initiated by starting a RTS*/
};



/* \brief Data type for Action Initiator
 * - can be either a Command Message to send out or SC RTS to begin executing. */
union SEM_ActionInit_t{
	char 			Cmd[SEM_MAX_ACTION_CMD_LEN];
	int32 			Rts;
};



/* \brief Definition of entry for SEM's Action Table*/
typedef struct {

	SEM_ActionId_t id; /* << \brief Unique Action Identifier number */

	enum SEM_ActionType_t type; 	/* << \brief Action execution type flag - as single
							command or SC RTS */

	union SEM_ActionInit_t init; 	/* << \brief Action Initiator Data used to
									Initiate execution of the action */

	uint32 duration;	/* << \brief the length of time that this action takes to perform*/

	SATCHECK_Checker_t precheck[SATCHECK_MAX_ACTION_CHECKS]; 	/* << \brief Satellite State
																Checks to be performed prior
																to action execution */

	SATCHECK_Checker_t durcheck[SATCHECK_MAX_ACTION_CHECKS]; 	/* << \brief Satellite State
																Checks to be performed while
																action is executing */

} SEM_ActionTblEntry_t;



/* \brief SEM Action Table Definition */
typedef struct {

	int32 NumEntries; /* << \brief Number of Actions Saved in this Action Table */

	SEM_ActionTblEntry_t Actions[SEM_MAX_ACTIONS]; /* << \brief Actions that this satellite can perform */

} SEM_ActionTbl_t;

/****************************************************************************************
* Schedule Table
****************************************************************************************/
/* \brief Status flags for action entries in the SEM Schedule Table*/
enum SEM_ActionStatus_t {
	SEM_UNUSED, 		 /*<< \brief Indicates the Schedule Table Entry
	 	 	 	 	 	 is not being used*/

	SEM_SKIPPED,		/*<< \brief Indicates the Action was Skipped -
						Due to the action occurring at a "past" time */

	SEM_CHECKFAILED, 	/*<< \brief Indicates the action was not executed because
					 	 its precheck failed */

	SEM_EXECUTED, 		/*<< \brief Indicates that the Action was successfully initiated*/

	SEM_SCHEDULED, 		/*<< \brief Indicates that the Action is scheduled but that
						initiation of the action has not yet been attempted.*/

	SEM_INITFAILED 		/*<< \brief Indicates that the Action failed to be initiated
	 	 	 	 	 	properly and therefore could not be executed*/
};

/* \brief << Data type for Schedule Table Entry */
typedef struct {

	enum SEM_ActionStatus_t 	Status; 	/*<< \brief The Status of the action
									represented by this entry */

	CFE_TIME_SysTime_t	StartTime;	/*<< \brief The Time at which this action
									is set to start */

	SEM_ActionId_t		Action;		/*<< \brief Id of the action to be initiated */

} SEM_ScheduleTblEntry_t;


/* \brief << Data type for Schedule Table */
typedef struct {

	int32 NumEntries; /*<< \brief The number of actions currently scheduled */

	SEM_ScheduleTblEntry_t Schedule[SEM_MAX_SCHEDULE_LEN]; 	/* \brief The Schedule of
															actions of the Satellite */

} SEM_ScheduleTbl_t;

#endif /* _SEM_TBLDEFS_H_ */
