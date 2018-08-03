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
** File Name:  comm_app.h
**
** Title:  Header File for COMM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define COMM's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _COMM_APP_H_
#define _COMM_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "comm_platform_cfg.h"
#include "comm_mission_cfg.h"
#include "comm_private_ids.h"
#include "comm_perfids.h"
#include "comm_msgids.h"
#include "comm_msg.h"
#include "comm_tables.h"



/*
** Local Defines
*/
#define COMM_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
/* \brief Communications manager Application Data Structure */
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[COMM_EVT_CNT];/*<< \brief COMM Event Table*/

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; /*<< \brief Schedule Pipe Id*/
    uint16           usSchPipeDepth;/*<< \brief Schedule Pipe Depth */
    char             cSchPipeName[OS_MAX_API_NAME];/*<< \brief Schedule Pipe Name*/

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;/*<< \brief Command Pipe Id*/
    uint16           usCmdPipeDepth;/*<< \brief Command Pipe Depth */
    char             cCmdPipeName[OS_MAX_API_NAME];/*<< \brief Command Pipe Name */
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;/*<< \brief Telemetry Pipe Id*/
    uint16           usTlmPipeDepth;/*<< \brief Telemetry Pipe Depth*/
    char             cTlmPipeName[OS_MAX_API_NAME];/*<< \brief Telemetry Pipe Name */

    /* Task-related */
    uint32  uiRunStatus;/*<< \brief COMM Task Run status */

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in comm/fsw/src/comm_msg.h */
    COMM_HkTlm_t  HkTlm;/*<< \brief COMM House keeping data packet */


    CFE_TBL_Handle_t DLinkHandle;/*<< \brief Downlink file table handle*/
    COMM_LinkFile_t* DLinkPtr;/*<< \brief Pointer to Downlink file table */

    CFE_TBL_Handle_t XLinkHandle;/*<< \brief Crosslink file table handle*/
    COMM_LinkFile_t* XLinkPtr;/*<< \brief Pointer to Crosslink file table */

} COMM_AppData_t;


/*
** External Global Variables
*/
extern COMM_AppData_t g_COMM_AppData;

/*
** Local Function Prototypes
*/
int32  COMM_InitApp(void);
int32  COMM_InitEvent(void);
int32  COMM_InitData(void);
int32  COMM_InitPipe(void);

void  COMM_AppMain(void);

void  COMM_CleanupCallback(void);

int32  COMM_RcvMsg(int32 iBlocking);

void  COMM_ProcessNewData(void);
void  COMM_ProcessNewCmds(void);
void  COMM_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  COMM_ReportHousekeeping(void);
void  COMM_SendOutData(void);

boolean  COMM_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _COMM_APP_H_ */

/*=======================================================================================
** End of file comm_app.h
**=====================================================================================*/
    
