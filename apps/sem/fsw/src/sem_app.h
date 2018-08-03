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
** File Name:  sem_app.h
**
** Title:  Header File for SEM Application
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
    
#ifndef _SEM_APP_H_
#define _SEM_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "access_lib.h"

#include "sem_events.h"
#include "sem_platform_cfg.h"
#include "sem_mission_cfg.h"
#include "sem_perfids.h"
#include "sem_msgids.h"
#include "sem_msg.h"
#include "sem_tbldefs.h"



/*
** Local Defines
*/
#define SEM_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
	/* AUTONOMY AND STATE INFORMATION */
	/* \brief Current operational State of SEM - either Autonomous or Not autonomous
	 *
	 * \par Description: SEM has two operational modes Autonomous and Non-Autonomous.
	 * These modes define SEM's ability to continue operation after the competition of
	 * its current tasks.
	 *
	 * In Non-Autonomous mode SEM will only respond to the commands given
	 * if a plan is scheduled SEM will execute that plan to completion (if possible)
	 * and then pend for further instruction.
	 *
	 * In Autonomous mode, SEM when lacking an executing schedule will request one
	 * from the ACCESS Planning Layer. Additionally, given the invalidity, or failure of
	 * a received plan SEM will if possible request an updated plan from the ACESS
	 * Planning layer, or if necessary take the satellite into a predefined safe mode
	 * in the case that a critical error has been detected with either the physical state
	 * of the satellite or the operational reliability of the Planning layer.
	 *
	 * */

	uint8 			AutonomyState;


	/* Operational Data */

    /* \brief CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[SEM_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* \brief CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* \brief CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* \brief Task-related applications status */
    uint32  uiRunStatus;
    
    /* \brief not used*/
    SEM_InData_t   InData;

    /* \brief not used */
    SEM_OutData_t  OutData;

    /* \brief Housekeeping telemetry - for downlink only.
       Data structure should be defined in sem/fsw/src/sem_msg.h */
    SEM_HkTlm_t  HkTlm;

    /* Table Data */
    CFE_TBL_Handle_t		ActionTblHandle; /* << \brief Table handle for SEM Action Table*/
    SEM_ActionTbl_t*		ActionTblPtr; /* << \brief Pointer to Action Table*/

    CFE_TBL_Handle_t		ScheduleTblHandle; /* << \brief Table handle for SEM Schedule Table*/
    SEM_ScheduleTbl_t* 		ScheduleTblPtr; /* << \brief Pointer to SEM Schedule Table */

    CFE_TBL_Handle_t		SatFrmtTblHandle; /* << \brief Handle for SA Satellite State Format Table (SSF) */
    FRMT_TBL_t*				SatFrmtTblPtr; /* << \brief Pointer (When owned) to SSF */

    CFE_TBL_Handle_t		PlanFrmtTblHandle;/* << \brief Table Handle for SEM Plan Format Table*/
    FRMT_TBL_t*				PlanFrmtTblPtr; /* << \brief Pointer to SEM Plan format table*/

    /* App Execution Information */
    uint32 					ScheduleCnt; /* << \brief The count of schedules that have been executed */
    uint8					ActionsSkipped; /* << \brief Count of scheduled actions that have been skipped by SEM */
    uint8 					ExeScheduleFlag; /* << \brief Flag indicating whether or not a schedule is currently being executed*/
    uint32 					NextActionIndex; /* << \brief The schedule Table index of the next action to be executed */

    CFE_ES_MemHandle_t		MemPoolHandle;/* << \brief Memory Pool Handle for SEM Dynamic Memory */
    uint8 					MemPoolBuffer[SEM_MEM_POOL_BUFFER_SIZE]; /* << \brief Buffer for SEM Dynamic Memory */

    char* 					SatStatePtr;/* << \brief Pointer current satellite state data */


    uint8 					InvalidScheduleCnt;/* << \brief Count of invalid schedules received by SEM */
    uint8 					InvalidActionsScheduledCnt; /* << \brief Count of invalid actions scheduled */
    uint8 					ActionPrereqsFailedCnt; /* << \brief Count of actions who's prerequisite checks have failed */


} SEM_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/
extern SEM_AppData_t g_SEM_AppData;
/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
/* \brief SEM Application initiation function - called on startup */
int32  SEM_InitApp(void);

/* \brief Registers and initializes SEM Event table with event services */
int32  SEM_InitEvent(void);

/* \brief Initiates SEM operational data */
int32  SEM_InitData(void);

/* \brief Initiates SEM Cmd, Tlm, and Schedule Pipes*/
int32  SEM_InitPipe(void);

/* \brief Main operational loop for SEM */
void  SEM_AppMain(void);

/* \brief Shutdown cleanup function */
void  SEM_CleanupCallback(void);

/* \brief Controls Message Response and operations on SEM WAKEUP */
int32  SEM_RcvMsg(int32 iBlocking);

/* \brief Processes the reception of new satellite state information */
void  SEM_ProcessNewData(void);

/* \brief Processes SEM reaction to new Commands */
void  SEM_ProcessNewCmds(void);

/* \brief Processes reaction to SEM Application Commands (NOOP, RESET, SCHPLAN)*/
void  SEM_ProcessNewAppCmds(CFE_SB_Msg_t*);

/* \brief Sends housekeeping data on */
void  SEM_ReportHousekeeping(void);

/* \brief Sends out application data (NOT CURRENTLY USED - see sem_app.c for details) */
void  SEM_SendOutData(void);

/* \brief Validates the message length of received commands */
boolean  SEM_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _SEM_APP_H_ */

/*=======================================================================================
** End of file sem_app.h
**=====================================================================================*/
    
