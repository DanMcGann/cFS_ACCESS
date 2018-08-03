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
** File Name:  gsds_app.h
**
** Title:  Header File for GSDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  To define GSDS's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _GSDS_APP_H_
#define _GSDS_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "gsds_events.h"
#include "gsds_platform_cfg.h"
#include "gsds_mission_cfg.h"
#include "gsds_perfids.h"
#include "gsds_msgids.h"
#include "gsds_msg.h"
#include "gsds_table.h"
#include "access_lib.h"



/*
** Local Defines
*/
#define GSDS_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[GSDS_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in gsds/fsw/src/gsds_msg.h */
    GSDS_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */

    /* Table Information */
    CFE_TBL_Handle_t StateHandle; 	/* << \brief Table Handle for Ground Station State Table*/
    GSDS_StateTbl_t * StatePtr;		/* << \brief Pointer to Ground Station State Table */

    CFE_TBL_Handle_t GsFrmtHandle;	/* << \brief Table Handle for Ground Station Format Table*/
    FRMT_TBL_t * GsFrmtPtr;	/* << \brief Table pointer for Ground Station Format Table */

} GSDS_AppData_t;

/*
** External Global Variables
*/
extern GSDS_AppData_t g_GSDS_AppData;
/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
int32  GSDS_InitApp(void);
int32  GSDS_InitEvent(void);
int32  GSDS_InitData(void);
int32  GSDS_InitPipe(void);

void  GSDS_AppMain(void);

void  GSDS_CleanupCallback(void);

int32  GSDS_RcvMsg(int32 iBlocking);

void  GSDS_ProcessNewData(void);
void  GSDS_ProcessNewCmds(void);
void  GSDS_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  GSDS_ReportHousekeeping(void);

boolean  GSDS_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _GSDS_APP_H_ */

/*=======================================================================================
** End of file gsds_app.h
**=====================================================================================*/
    
