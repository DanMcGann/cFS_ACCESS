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
** File Name:  sa_app.h
**
** Title:  Header File for SA Application
**
** $Author:    unk
** $Revision: 1.1 $
** $Date:      2018-04-18
**
** Purpose:  To define SA's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-18 | unk | Build #: Code Started
**
**
**   Notes:
**   Future Design changes
**   	- allow for platform cfg parameter that makes sending state data zero copy
**   		(would allow for for efficiency if the sat state data is big)
**
**=====================================================================================*/
    
#ifndef _SA_APP_H_
#define _SA_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sa_events.h"
#include "sa_platform_cfg.h"
#include "sa_mission_cfg.h"
#include "sa_perfids.h"
#include "sa_msgids.h"
#include "sa_msg.h"
#include "access_lib.h"



/*
** Local Defines
*/
#define SA_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[SA_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  StatePipeId;
    uint16           usStatePipeDepth;
    char             cStatePipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in sa/fsw/src/sa_msg.h */
    SA_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */

    /* Table data */
    CFE_TBL_Handle_t SatFrmtHandle;
    FRMT_TBL_t * SatFrmtPtr;
    uint32 SatFrmtEntries;

    /* Memory Pool Data */
    CFE_ES_MemHandle_t MemPoolHandle; //TODO Setup mempool
    uint8 MemPoolBuffer[SA_MEM_POOL_BUFFER_SIZE];

    /* Satellite State Data */
    char* SatStatePtr;

} SA_AppData_t;

/*
** External Global Variables
*/
extern SA_AppData_t g_SA_AppData;
/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
int32  SA_InitApp(void);
int32  SA_InitEvent(void);
int32  SA_InitData(void);
int32  SA_InitPipe(void);

void  SA_AppMain(void);

void  SA_CleanupCallback(void);

int32  SA_RcvMsg(int32 iBlocking);

void  SA_ProcessNewData(void);
void  SA_ProcessNewCmds(void);
void  SA_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  SA_ReportHousekeeping(void);
void  SA_SendOutData(void);

#endif /* _SA_APP_H_ */

/*=======================================================================================
** End of file sa_app.h
**=====================================================================================*/
    
