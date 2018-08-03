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
** File Name:  comm_app.c
**
** Title:  Function Definitions for COMM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  This source file contains all necessary function definitions to run COMM
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>

#include "cfe.h"

#include "comm_platform_cfg.h"
#include "comm_mission_cfg.h"
#include "comm_app.h"
#include "comm_cmds.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
COMM_AppData_t  g_COMM_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/
    
/*=====================================================================================
** Name: COMM_InitEvent
**
** Purpose: To initialize and register event table for COMM application
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
**    COMM_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_COMM_AppData.EventTbl
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 COMM_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_COMM_AppData.EventTbl, 0x00, sizeof(g_COMM_AppData.EventTbl));
    //TODO Set upEvent IDs
    g_COMM_AppData.EventTbl[0].EventID = COMM_RESERVED_EID;
    g_COMM_AppData.EventTbl[1].EventID = COMM_INF_EID;
    g_COMM_AppData.EventTbl[2].EventID = COMM_INIT_INF_EID;
    g_COMM_AppData.EventTbl[3].EventID = COMM_ILOAD_INF_EID;
    g_COMM_AppData.EventTbl[4].EventID = COMM_CDS_INF_EID;
    g_COMM_AppData.EventTbl[5].EventID = COMM_CMD_INF_EID;

    g_COMM_AppData.EventTbl[ 6].EventID = COMM_ERR_EID;
    g_COMM_AppData.EventTbl[ 7].EventID = COMM_INIT_ERR_EID;
    g_COMM_AppData.EventTbl[ 8].EventID = COMM_ILOAD_ERR_EID;
    g_COMM_AppData.EventTbl[ 9].EventID = COMM_CDS_ERR_EID;
    g_COMM_AppData.EventTbl[10].EventID = COMM_CMD_ERR_EID;
    g_COMM_AppData.EventTbl[11].EventID = COMM_PIPE_ERR_EID;
    g_COMM_AppData.EventTbl[12].EventID = COMM_MSGID_ERR_EID;
    g_COMM_AppData.EventTbl[13].EventID = COMM_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_COMM_AppData.EventTbl,
                               COMM_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("COMM - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: COMM_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for COMM application
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
**    COMM_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_COMM_AppData.usSchPipeDepth
**    g_COMM_AppData.cSchPipeName
**    g_COMM_AppData.SchPipeId
**    g_COMM_AppData.usCmdPipeDepth
**    g_COMM_AppData.cCmdPipeName
**    g_COMM_AppData.CmdPipeId
**    g_COMM_AppData.usTlmPipeDepth
**    g_COMM_AppData.cTlmPipeName
**    g_COMM_AppData.TlmPipeId
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 COMM_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_COMM_AppData.usSchPipeDepth = COMM_SCH_PIPE_DEPTH;
    memset((void*)g_COMM_AppData.cSchPipeName, '\0', sizeof(g_COMM_AppData.cSchPipeName));
    strncpy(g_COMM_AppData.cSchPipeName, "COMM_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_COMM_AppData.SchPipeId,
                                 g_COMM_AppData.usSchPipeDepth,
                                 g_COMM_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(COMM_WAKEUP_MID, g_COMM_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("COMM - Sch Pipe failed to subscribe to COMM_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto COMM_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("COMM - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto COMM_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_COMM_AppData.usCmdPipeDepth = COMM_CMD_PIPE_DEPTH ;
    memset((void*)g_COMM_AppData.cCmdPipeName, '\0', sizeof(g_COMM_AppData.cCmdPipeName));
    strncpy(g_COMM_AppData.cCmdPipeName, "COMM_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_COMM_AppData.CmdPipeId,
                                 g_COMM_AppData.usCmdPipeDepth,
                                 g_COMM_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(COMM_CMD_MID, g_COMM_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("COMM - CMD Pipe failed to subscribe to COMM_CMD_MID. (0x%08X)\n", iStatus);
            goto COMM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(COMM_SEND_HK_MID, g_COMM_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("COMM - CMD Pipe failed to subscribe to COMM_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto COMM_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("COMM - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto COMM_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_COMM_AppData.usTlmPipeDepth = COMM_TLM_PIPE_DEPTH;
    memset((void*)g_COMM_AppData.cTlmPipeName, '\0', sizeof(g_COMM_AppData.cTlmPipeName));
    strncpy(g_COMM_AppData.cTlmPipeName, "COMM_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_COMM_AppData.TlmPipeId,
                                 g_COMM_AppData.usTlmPipeDepth,
                                 g_COMM_AppData.cTlmPipeName);

COMM_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: COMM_InitData
**
** Purpose: To initialize global variables used by COMM application
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
**    COMM_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_COMM_AppData.InData
**    g_COMM_AppData.OutData
**    g_COMM_AppData.HkTlm
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 COMM_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init housekeeping packet */
    memset((void*)&g_COMM_AppData.HkTlm, 0x00, sizeof(g_COMM_AppData.HkTlm));
    CFE_SB_InitMsg(&g_COMM_AppData.HkTlm,
                   COMM_HK_TLM_MID, sizeof(g_COMM_AppData.HkTlm), TRUE);

    return (iStatus);
}
    
/*=====================================================================================
** Name: COMM_InitApp
**
** Purpose: To initialize all data local to and used by COMM application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    COMM_InitEvent
**    COMM_InitPipe
**    COMM_InitData
**
** Called By:
**    COMM_AppMain
**
** Global Inputs/Reads:
**    TBD
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 COMM_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_COMM_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("COMM - Failed to register the app (0x%08X)\n", iStatus);
        goto COMM_InitApp_Exit_Tag;
    }

    if ((COMM_InitEvent() != CFE_SUCCESS) || 
        (COMM_InitPipe() != CFE_SUCCESS) || 
        (COMM_InitData() != CFE_SUCCESS) ||
		(COMM_InitTables() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto COMM_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&COMM_CleanupCallback);

COMM_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(COMM_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "COMM - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("COMM - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: COMM_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
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
**    TBD
**
** Global Inputs/Reads:
**    TBD
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void COMM_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
}
    
/*=====================================================================================
** Name: COMM_RcvMsg
**
** Purpose: To receive and process messages for COMM application
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
**    COMM_ProcessNewCmds
**    COMM_ProcessNewData
**    COMM_SendOutData
**
** Called By:
**    COMM_Main
**
** Global Inputs/Reads:
**    g_COMM_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_COMM_AppData.uiRunStatus
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 COMM_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(COMM_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_COMM_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(COMM_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case COMM_WAKEUP_MID:
                COMM_ProcessNewCmds();
                COMM_ProcessNewData();
                break;

            default:
                CFE_EVS_SendEvent(COMM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "COMM - Recvd invalid SCH msgId (0x%08X)", MsgId);
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
        CFE_EVS_SendEvent(COMM_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "COMM: SB pipe read error (0x%08X), app will exit", iStatus);
        g_COMM_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: COMM_ProcessNewData
**
** Purpose: To process incoming data subscribed by COMM application
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
**
** Called By:
**    COMM_RcvMsg
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void COMM_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_COMM_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {

                default:
                    CFE_EVS_SendEvent(COMM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "COMM - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(COMM_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "COMM: CMD pipe read error (0x%08X)", iStatus);
            g_COMM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: COMM_ProcessNewCmds
**
** Purpose: To process incoming command messages for COMM application
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
**    COMM_ProcessNewAppCmds
**    COMM_ReportHousekeeping
**
** Called By:
**    COMM_RcvMsg
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void COMM_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_COMM_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case COMM_CMD_MID:
                    COMM_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case COMM_SEND_HK_MID:
                    COMM_ReportHousekeeping();
                    break;

                default:
                    CFE_EVS_SendEvent(COMM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "COMM - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(COMM_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "COMM: CMD pipe read error (0x%08X)", iStatus);
            g_COMM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: COMM_ProcessNewAppCmds
**
** Purpose: To process command messages targeting COMM application
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
**    COMM_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_COMM_AppData.HkTlm.usCmdCnt
**    g_COMM_AppData.HkTlm.usCmdErrCnt
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void COMM_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case COMM_NOOP_CC:
                COMM_NoopCmd(MsgPtr);
                break;

            case COMM_RESET_CC:
                COMM_RstCmd(MsgPtr);
                break;

            case COMM_REGISTERFILE_CC:
                COMM_RegisterFileCmd(MsgPtr);
                break;

            case COMM_STARTLINK_CC:
            	COMM_StartLinkCmd(MsgPtr);
                break;

            case COMM_STOPLINK_CC:
            	COMM_StopLinkCmd(MsgPtr);
                break;

            case COMM_ACKLINK_CC:
            	COMM_AckLinkCmd(MsgPtr);
                break;

            default:
                g_COMM_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(COMM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "COMM - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: COMM_ReportHousekeeping
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
**    COMM_ProcessNewCmds
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void COMM_ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_COMM_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_COMM_AppData.HkTlm);
}
/*=====================================================================================
** Name: COMM_VerifyCmdLength
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
**    COMM_ProcessNewCmds
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean COMM_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
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

            CFE_EVS_SendEvent(COMM_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "COMM - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_COMM_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: COMM_AppMain
**
** Purpose: To define COMM application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    COMM_InitApp
**    COMM_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
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
** History:  Date Written  2018-05-14
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void COMM_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(COMM_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (COMM_InitApp() != CFE_SUCCESS)
    {
        g_COMM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(COMM_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(COMM_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(COMM_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_COMM_AppData.uiRunStatus) == TRUE)
    {
        COMM_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(COMM_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_COMM_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file comm_app.c
**=====================================================================================*/
    
