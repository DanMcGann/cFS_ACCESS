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
/******************************************************************************
*	File: sem_action.c
*
*	Purpose: Defines default satellite action table
*
*	Notes: Mission Software developer should modify this table for mission
*	specific satellite action information
*
*	Author: Daniel McGann
******************************************************************************/

/*
 * Includes
 */
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "sem_platform_cfg.h"
#include "sem_app.h"
#include "sem_tbldefs.h"


/*
 * Default Constellation State Table
 */
SEM_ScheduleTbl_t SEM_ScheduleTable =
{
		.NumEntries = 0, /* Number of entries in this table */
		.Schedule = {
				{ /* Schedule Entry 0 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 1 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 2 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 3 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 4 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 5 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 6 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 7 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 8 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 9 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 10 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 11 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 12 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 13 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 14 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 15 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 16 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 17 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 18 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				{ /* Schedule Entry 19 */
						SEM_UNUSED, /* Entry Status (SEM_ActionStatus_t)*/
						{0,0}, 		/* Entry Start Time (CFE_TIME_SysTime_t)*/
						0, 			/* Entry Action Id  (SEM_ActionId_t) */
				},

				/* Schedule Entry N... */
		}

};

/*
 * Table Header
 */
CFE_TBL_FILEDEF(SEM_ScheduleTable, // @suppress("Unused variable declaration in file scope")
		SEM_APP.SCH_TBL,
		SEM Schedule Table,
		sem_sch.tbl)

/**********************
* End of File Comment *
**********************/
