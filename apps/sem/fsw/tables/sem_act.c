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
SEM_ActionTbl_t SEM_ActionTable =
{
		.NumEntries = 0, /* Number of entries in this table */
		.Actions = {
				{ /* Action 0 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 1 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 2 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 3 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 4 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 5 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 6 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 7 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 8 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 9 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 10 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 11 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 12 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 13 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 14 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 15 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 16 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 17 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},
				{ /* Action 18 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},

				{ /* Action 19 */
						SEM_UNUSED_ENTRY, 		/* Action Id */
						CMD, 	/* Action Type {CMD or RTS}*/
						{}, 	/* Action Init (Cmd Packet or RTS Id) */
						0, 		/* Action Duration (in Seconds)*/
						{}, 	/* Action preChecks */
						{} 		/* Action DurChecks */
				},

				/* Action N... */
		}

};

/*
 * Table Header
 */
CFE_TBL_FILEDEF(SEM_ActionTable, // @suppress("Unused variable declaration in file scope")
		SEM_APP.ACT_TBL,
		SEM Action Table,
		sem_act.tbl)

/**********************
* End of File Comment *
**********************/
