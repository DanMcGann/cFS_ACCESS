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
** File Name:  conds_app.c
**
** Title:  Function Definitions for CONDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-26
**
** Purpose:  This source file contains all necessary function definitions to run CONDS
**           application.
**
** Functions Defined:
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-26 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/


/*
** Include Files
*/
#include <string.h>

#include "cfe.h"

#include "conds_platform_cfg.h"
#include "conds_mission_cfg.h"
#include "conds_app.h"
#include "conds_table.h"
#include "conds_cmds.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* g_CONDS_AppData -- application global data structure            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CONDS_AppData_t  g_CONDS_AppData;
    
/*=====================================================================================
** Name: CONDS_AppMain
**
** Purpose: To define CONDS application's entry point and main process loop
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    CONDS_InitApp
**    CONDS_RcvMsg
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void CONDS_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(CONDS_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (CONDS_InitApp() != CFE_SUCCESS)
    {
        g_CONDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(CONDS_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(CONDS_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(CONDS_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_CONDS_AppData.uiRunStatus) == TRUE)
    {
        CONDS_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(CONDS_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_CONDS_AppData.uiRunStatus);
} 
 /*=====================================================================================
** Name: CONDS_InitApp
**
** Purpose: To initialize all data local to and used by CONDS application
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    CONDS_InitEvent
**    CONDS_InitPipe
**    CONDS_InitData
**    CONDS_InitTables
**
** Called By:
**    CONDS_AppMain
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 CONDS_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_CONDS_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("CONDS - Failed to register the app (0x%08X)\n", iStatus);
        goto CONDS_InitApp_Exit_Tag;
    }

    if ((CONDS_InitEvent() != CFE_SUCCESS) ||   //Register Events
        (CONDS_InitPipe() != CFE_SUCCESS) ||    //Create Message Pipes
        (CONDS_InitData() != CFE_SUCCESS) ||   //Initialize gloabl app data
        (CONDS_InitTables() != CFE_SUCCESS) ||	//Init ConState Table
		(CONDS_InitFrmtTables() != CFE_SUCCESS) ||		//Init Format Tables
	    (CONDS_SubscribeStateMsgs() != CFE_SUCCESS) || 	//Subscribe to necessary messages
	    (CONDS_AllocateStateMemory() != CFE_SUCCESS))	//Allocate memory to store data.
    {
        iStatus = -1;
        goto CONDS_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    //OS_TaskInstallDeleteHandler((void*)&CONDS_CleanupCallback);

CONDS_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(CONDS_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "CONDS - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("CONDS - Application failed to initialize\n");
    }

    return (iStatus);
}
  
/*=====================================================================================
** Name: CONDS_InitEvent
**
** Purpose: To initialize and register event table for CONDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    CONDS_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
    CFE_SB_InitMsg(&g_CONDS_AppData.HkTlm,
                   CONDS_HK_TLM_MID, sizeof(g_CONDS_AppData.HkTlm), TRUE);
**    g_CONDS_AppData.EventTbl
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 CONDS_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_CONDS_AppData.EventTbl, 0x00, sizeof(g_CONDS_AppData.EventTbl));

    g_CONDS_AppData.EventTbl[0].EventID = CONDS_RESERVED_EID;
    g_CONDS_AppData.EventTbl[1].EventID = CONDS_INF_EID;
    g_CONDS_AppData.EventTbl[2].EventID = CONDS_INIT_INF_EID;
    g_CONDS_AppData.EventTbl[3].EventID = CONDS_ILOAD_INF_EID;
    g_CONDS_AppData.EventTbl[4].EventID = CONDS_CDS_INF_EID;
    g_CONDS_AppData.EventTbl[5].EventID = CONDS_CMD_INF_EID;
    g_CONDS_AppData.EventTbl[6].EventID = CONDS_INIT_TBL_CDS_EID;
    g_CONDS_AppData.EventTbl[7].EventID = CONDS_TBL_INF_EID;

    g_CONDS_AppData.EventTbl[ 8].EventID = CONDS_ERR_EID;
    g_CONDS_AppData.EventTbl[ 9].EventID = CONDS_INIT_ERR_EID;
    g_CONDS_AppData.EventTbl[10].EventID = CONDS_ILOAD_ERR_EID;
    g_CONDS_AppData.EventTbl[11].EventID = CONDS_CDS_ERR_EID;
    g_CONDS_AppData.EventTbl[12].EventID = CONDS_CMD_ERR_EID;
    g_CONDS_AppData.EventTbl[13].EventID = CONDS_PIPE_ERR_EID;
    g_CONDS_AppData.EventTbl[14].EventID = CONDS_MSGID_ERR_EID;
    g_CONDS_AppData.EventTbl[15].EventID = CONDS_MSGLEN_ERR_EID;
    g_CONDS_AppData.EventTbl[16].EventID = CONDS_INIT_TBL_ERR_EID;
    g_CONDS_AppData.EventTbl[17].EventID = CONDS_SUB_STATEMSGS_ERR_EID;
    g_CONDS_AppData.EventTbl[18].EventID = CONDS_TABLE_VERIFY_ERR_EID;
    g_CONDS_AppData.EventTbl[19].EventID = CONDS_SAVEDATA_ERR_EID;
    g_CONDS_AppData.EventTbl[20].EventID = CONDS_DATAREQCMD_ERR_EID;
    g_CONDS_AppData.EventTbl[21].EventID = CONDS_TBLGRABPTRS_ERR_EID;
    g_CONDS_AppData.EventTbl[22].EventID = CONDS_TABLE_MANAGE_ERR_EID;
    g_CONDS_AppData.EventTbl[23].EventID = CONDS_ALLOCMEM_ERR_EID;


    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_CONDS_AppData.EventTbl,
                               CONDS_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("CONDS - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: CONDS_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for CONDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    CONDS_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_CONDS_AppData.usSchPipeDepth
**    g_CONDS_AppData.cSchPipeName
**    g_CONDS_AppData.SchPipeId
**    g_CONDS_AppData.usCmdPipeDepth
**    g_CONDS_AppData.cCmdPipeName
**    g_CONDS_AppData.CmdPipeId
**    g_CONDS_AppData.usTlmPipeDepth
**    g_CONDS_AppData.cTlmPipeName
**    g_CONDS_AppData.TlmPipeId
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 CONDS_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_CONDS_AppData.usSchPipeDepth = CONDS_SCH_PIPE_DEPTH;
    memset((void*)g_CONDS_AppData.cSchPipeName, '\0', sizeof(g_CONDS_AppData.cSchPipeName));
    strncpy(g_CONDS_AppData.cSchPipeName, "CONDS_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_CONDS_AppData.SchPipeId,
                                 g_CONDS_AppData.usSchPipeDepth,
                                 g_CONDS_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(CONDS_WAKEUP_MID, g_CONDS_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("CONDS - Sch Pipe failed to subscribe to CONDS_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto CONDS_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("CONDS - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto CONDS_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_CONDS_AppData.usCmdPipeDepth = CONDS_CMD_PIPE_DEPTH ;
    memset((void*)g_CONDS_AppData.cCmdPipeName, '\0', sizeof(g_CONDS_AppData.cCmdPipeName));
    strncpy(g_CONDS_AppData.cCmdPipeName, "CONDS_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_CONDS_AppData.CmdPipeId,
                                 g_CONDS_AppData.usCmdPipeDepth,
                                 g_CONDS_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(CONDS_CMD_MID, g_CONDS_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("CONDS - CMD Pipe failed to subscribe to CONDS_CMD_MID. (0x%08X)\n", iStatus);
            goto CONDS_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(CONDS_SEND_HK_MID, g_CONDS_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("CONDS - CMD Pipe failed to subscribe to CONDS_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto CONDS_InitPipe_Exit_Tag;
        }
        
    }
    else
    {

        goto CONDS_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_CONDS_AppData.usConStatePipeDepth = CONDS_TLM_PIPE_DEPTH;
    memset((void*)g_CONDS_AppData.cConStatePipeName, '\0', sizeof(g_CONDS_AppData.cConStatePipeName));
    strncpy(g_CONDS_AppData.cConStatePipeName, "CONDS_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_CONDS_AppData.ConStatePipeId,
                                 g_CONDS_AppData.usConStatePipeDepth,
                                 g_CONDS_AppData.cConStatePipeName);
    /* Constellation state information arrives in the form of telemetry packets 
        these packets are subscribed to after successful table initialization.
    */
    
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("CONDS - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto CONDS_InitPipe_Exit_Tag;
    }

CONDS_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: CONDS_InitData
**
** Purpose: To initialize global variables used by CONDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    CONDS_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_CONDS_AppData.InData
**    g_CONDS_AppData.OutData
**    g_CONDS_AppData.HkTlm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 CONDS_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init housekeeping packet */
    memset((void*)&g_CONDS_AppData.HkTlm, 0x00, sizeof(g_CONDS_AppData.HkTlm));
    CFE_SB_InitMsg(&g_CONDS_AppData.HkTlm,
                   CONDS_HK_TLM_MID, sizeof(g_CONDS_AppData.HkTlm), TRUE);

    /* Initialize Memory Pool for the Application */
    CFE_ES_PoolCreate(&g_CONDS_AppData.MemPoolHandle,
    		g_CONDS_AppData.MemPoolBuffer,
			sizeof(g_CONDS_AppData.MemPoolBuffer));

    return (iStatus);
}
    
   
/*=====================================================================================
** Name: CONDS_RcvMsg
**
** Purpose: To receive and process messages for CONDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization 
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CONDS_ProcessNewCmds
**    CONDS_ProcessNewData
**    CONDS_SendOutData
**
** Called By:
**    CONDS_Main
**
** Global Inputs/Reads:
**    g_CONDS_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_CONDS_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 CONDS_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(CONDS_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_CONDS_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(CONDS_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case CONDS_WAKEUP_MID:
                CONDS_ProcessNewCmds();
                CONDS_ProcessNewStateData();
                break;

            default:
                CFE_EVS_SendEvent(CONDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "CONDS - Recvd invalid SCH msgId (0x%08X)", MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or nothing */
    }
    else
    {
        /* This is an example of exiting on an error.
        ** Note that a SB read error is not always going to result in an app quitting.
        */
        CFE_EVS_SendEvent(CONDS_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "CONDS: SB pipe read error (0x%08X), app will exit", iStatus);
        g_CONDS_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}

/*=====================================================================================
** Name: CONDS_ProcessNewStateData
**
** Purpose: To process incoming data subscribed by CONDS application
**
** Arguments:
**    None
**
** Returns:
**    Exit Status Value
**
** Routines Called:
**    CONDS_IsValidStateMsgId
**    CONDS_SaveStateData
**
** Called By:
**    CONDS_RcvMsg
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void CONDS_ProcessNewStateData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   StateMsgPtr = NULL;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&StateMsgPtr, g_CONDS_AppData.ConStatePipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
        	if (CONDS_IsValidStateMsgId(CFE_SB_GetMsgId(StateMsgPtr))) {

        		iStatus = CONDS_SaveStateData(StateMsgPtr);

        		if (iStatus != CFE_SUCCESS) {
        			CFE_EVS_SendEvent(CONDS_SAVEDATA_ERR_EID, CFE_EVS_ERROR,
        				"CONDS: Save State Data Error (0x%08X)", iStatus);
        			continue;
        		}
        	}
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(CONDS_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CONDS: CMD pipe read error (0x%08X)", iStatus);
            g_CONDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

    
/*=====================================================================================
** Name: CONDS_ProcessNewCmds
**
** Purpose: To process incoming command messages for CONDS application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CONDS_ProcessNewAppCmds
**    CONDS_ReportHousekeeping
**
** Called By:
**    CONDS_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void CONDS_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_CONDS_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case CONDS_CMD_MID:
                    CONDS_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case CONDS_SEND_HK_MID:
                    CONDS_ReportHousekeeping();
                    break;

                default:
                    CFE_EVS_SendEvent(CONDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "CONDS - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(CONDS_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CONDS: CMD pipe read error (0x%08X)", iStatus);
            g_CONDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: CONDS_ProcessNewAppCmds
**
** Purpose: To process command messages targeting CONDS application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    CONDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_CONDS_AppData.HkTlm.usCmdCnt
**    g_CONDS_AppData.HkTlm.usCmdErrCnt
** CFE_TBL_ValidateAccess
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void CONDS_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case CONDS_NOOP_CC:
                CONDS_NoopCmd(MsgPtr);
                break;

            case CONDS_RESET_CC:
                CONDS_ResetCmd(MsgPtr);
                break;

            case CONDS_DATAREQ_CC:
            	CONDS_DataReqCmd(MsgPtr);
            	break;
            //TODO Implement Other CONDS Commands

            default:
                g_CONDS_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(CONDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "CONDS - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: CONDS_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    CONDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, Daniel McGann
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void CONDS_ReportHousekeeping()
{
	/*
	 * 	Manage All CONDS Tables With each Housekeeping Request
	 * 	Format Tables are managed as a part of ConState Verification
	 */
	CONDS_TableManageConState();


    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_CONDS_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_CONDS_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: CONDS_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    CONDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-03-26
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean CONDS_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen)
{
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(CONDS_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "CONDS - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_CONDS_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
 
    
/*=======================================================================================
** End of file conds_app.c
**=====================================================================================*/
    
