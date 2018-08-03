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
** File Name:  conds_app.h
**
** Title:  Header File for CONDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-26
**
** Purpose:  To define CONDS's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-26 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _CONDS_APP_H_
#define _CONDS_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "conds_platform_cfg.h"
#include "conds_mission_cfg.h"
#include "conds_events.h"
#include "conds_msg.h"
#include "conds_perfids.h"
#include "conds_msgids.h"
#include "conds_msg.h"
#include "conds_table.h"



/*
** Local Defines
*/
#define CONDS_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[CONDS_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId;                     /** < \brief CONDS Schedule Pipe ID */
    uint16           usSchPipeDepth;                /** < \brief CONDS Schedule Pipe Depth */
    char             cSchPipeName[OS_MAX_API_NAME]; /** < \brief CONDS Schedule Pipe Name */

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;                     /** < \brief CONDS Command Pipe ID */
    uint16           usCmdPipeDepth;                /** < \brief CONDS Command Pipe Depth */
    char             cCmdPipeName[OS_MAX_API_NAME]; /** < \brief CONDS Command Pipe Name */
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  ConStatePipeId;                     /** < \brief CONDS State Info Pipe ID */
    uint16           usConStatePipeDepth;                /** < \brief CONDS State Info Pipe Depth */
    char             cConStatePipeName[OS_MAX_API_NAME]; /** < \brief CONDS State Info Pipe Name */

    /* Task-related */
    uint32  uiRunStatus;                			/** < \brief CONDS Run status*/

    /* Housekeeping telemetry - for downlink only. Data structure should be defined in conds/fsw/src/conds_msg.h */
    CONDS_HkTlm_t  HkTlm;              				 /**< \brief CONDS housekeeping data packet */

    /* Constellation State Table */
    CFE_TBL_Handle_t ConStateTblHandle; 			/**< \brief Constellation State Table Handle*/
    CONDS_ConStateEntry_t* ConStatePtr;				/**< \brief Constellation State Table Pointer*/

    uint8 FrmtTablesInit;							/** < Flag indicating the format tables have been initialized **/

    /* Memory Pool Data */
    CFE_ES_MemHandle_t	MemPoolHandle;					/**< \brief Memory Pool Handle */
    uint8 MemPoolBuffer[CONDS_MEM_POOL_BUFFER_SIZE];/**< \brief Memory Pool Buffer **/

} CONDS_AppData_t;

/**
 * Define Global AppData
 */
extern CONDS_AppData_t g_CONDS_AppData;

/**
**  \brief  CONDS Application Initialization Function
**
**  \par Description: 
**          Performs the following startup activities
**          -Registers CONDS app for cFE Event Services
**          -Creates cFE Software Bus Popes for Commands, Telemetry, and Schedule Messages
**          -Initializes application default data
**          -Registers Constellation State Table
**              -Registers any needed format tables for the Constellation State Table
**              -Subscribes to all needed messages containing state telemetry
**          -generate startup initialization event message
**
**  \par Assumptions, External Events, and Notes:
**          (none)
*/
int32  CONDS_InitApp(void);

/**
**  \brief CONDS Event Initialization Function
**
**  \par Description:
**          Initializes and registers CONDS event table with cFE Table Services
**
**
**  \par Assumptions, External Events, and Notes:
**          (none)
**
*/
int32  CONDS_InitEvent(void);

/**
**  \brief CONDS Global Data Initialization Function
**
**  \par Description:
**          Allocates memory for application global data. Populates any needed
**          data with default values
**
**  \par Assumptions, External Events, and Notes:
**      (none)
*/
int32  CONDS_InitData(void);

/**
**  \brief  CONDS Software Pip Initialization Function
**
**  \par Description:
**          Creates cFE Software Bus pipes for application Commands, 
**          State Telemetry, and Schedule Messages
**
**  \par Assumptions, External Events, and Notes:
**          (none)
*/
int32  CONDS_InitPipe(void);

/**
**  \brief ACCESS Constellation Data Store (CONDS) application entry point
**
**  \par Description:
**          CONDS application entry point and pain process loop
**
**  \par Assumptions, External Events, and Notes:
**          (none)
*/
void  CONDS_AppMain(void);

/**
**  \brief CONDS Message Recieve Protocol
**
**  \par Description:
**          Handles all incoming meesages to the CONDS application
**
**  \par Assumptions, External Events, and Notes:
**       (none)i
**
**  \param [in] isBlocking - #CFE_SB_PEND_FOREVER 
*/
int32  CONDS_RcvMsg(int32 iBlocking);

/**
**  \brief CONDS New State Information Revieced Protocol
**
**  \par Description:
**          Copies recieved data into the data location for this state info
**          as defined by the Constellation state table
**        
**
**  \par Assumptions, External Events, and Notes:
**       Constellation State Table is well formed.
*/
void  CONDS_ProcessNewStateData(void);

/**
**  \brief CONDS Command Recieved Protocol
**
**  \par Description:
**          Handles the revcieve of a new command message
**
**  \par Assumptions, External Events, and Notes:
**       (none)
*/
void  CONDS_ProcessNewCmds(void);

/**
**  \brief CONDS Application Command Recieved Propocol
**
**  \par Description:
**          Handles the reciving of a new CONDS Application Command
**          by calling the needed function based on the message command id.
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**
**  \param [in] Software Bus message Pointer (#CFE_SB_MsgPtr_t)
*/
void  CONDS_ProcessNewAppCmds(CFE_SB_Msg_t*);

/**
**  \brief CONDS Houskeeping report function
**
**  \par Description:
**          Sends out CONDS Houskeeping information as a Software Bus Message
**        
**  \par Assumptions, External Events, and Notes:
**          (none)
*/
void  CONDS_ReportHousekeeping(void);

/**
**  \brief CONDS Verify Command Length Function
**
**  \par Description:
**          Checks to ensure the software bus message matched the given expected
**          length.
**        
**  \par Assumptions, External Events, and Notes:
**          (none)
*/
boolean  CONDS_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _CONDS_APP_H_ */

/*=======================================================================================
** End of file conds_app.h
**=====================================================================================*/
    
