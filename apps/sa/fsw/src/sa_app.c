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
** File Name:  sa_app.c
**
** Title:  Function Definitions for SA Application
**
** $Author:    unk
** $Revision: 1.1 $
** $Date:      2018-04-18
**
** Purpose:  This source file contains all necessary function definitions to run SA
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
**   2018-04-18 | unk | Build #: Code Started
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
#include "access_lib.h"

#include "sa_platform_cfg.h"
#include "sa_mission_cfg.h"
#include "sa_app.h"
#include "sa_tables.h"


SA_AppData_t g_SA_AppData;


/*=====================================================================================
** Name: SA_InitEvent
**
** Purpose: To initialize and register event table for SA application
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
**    SA_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_SA_AppData.EventTbl
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 SA_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    //TODO Update with all event IDS
    /* Create the event table */
    memset((void*)g_SA_AppData.EventTbl, 0x00, sizeof(g_SA_AppData.EventTbl));

    g_SA_AppData.EventTbl[0].EventID = SA_RESERVED_EID;
    g_SA_AppData.EventTbl[1].EventID = SA_INF_EID;
    g_SA_AppData.EventTbl[2].EventID = SA_INIT_INF_EID;
    g_SA_AppData.EventTbl[3].EventID = SA_ILOAD_INF_EID;
    g_SA_AppData.EventTbl[4].EventID = SA_CDS_INF_EID;
    g_SA_AppData.EventTbl[5].EventID = SA_CMD_INF_EID;

    g_SA_AppData.EventTbl[ 6].EventID = SA_ERR_EID;
    g_SA_AppData.EventTbl[ 7].EventID = SA_INIT_ERR_EID;
    g_SA_AppData.EventTbl[ 8].EventID = SA_ILOAD_ERR_EID;
    g_SA_AppData.EventTbl[ 9].EventID = SA_CDS_ERR_EID;
    g_SA_AppData.EventTbl[10].EventID = SA_CMD_ERR_EID;
    g_SA_AppData.EventTbl[11].EventID = SA_PIPE_ERR_EID;
    g_SA_AppData.EventTbl[12].EventID = SA_MSGID_ERR_EID;
    g_SA_AppData.EventTbl[13].EventID = SA_MSGLEN_ERR_EID;

    g_SA_AppData.EventTbl[14].EventID = SA_INIT_TBL_ERR_EID;
    g_SA_AppData.EventTbl[15].EventID = SA_VERIFY_TBL_ERR_EID;
    g_SA_AppData.EventTbl[16].EventID = SA_GRABPTRS_ERR_EID;
    g_SA_AppData.EventTbl[17].EventID = SA_DROPPTRS_ERR_EID;
    g_SA_AppData.EventTbl[18].EventID = SA_ALLOCMEM_ERR_EID;
    g_SA_AppData.EventTbl[19].EventID = SA_PRCSSDATA_ERR_EID;
    g_SA_AppData.EventTbl[20].EventID = SA_FREEMEM_ERR_EID;
    g_SA_AppData.EventTbl[21].EventID = SA_SUBMSGS_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_SA_AppData.EventTbl,
                               SA_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SA - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: SA_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for SA application
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
**    SA_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_SA_AppData.usSchPipeDepth
**    g_SA_AppData.cSchPipeName
**    g_SA_AppData.SchPipeId
**    g_SA_AppData.usCmdPipeDepth
**    g_SA_AppData.cCmdPipeName
**    g_SA_AppData.CmdPipeId
**    g_SA_AppData.usTlmPipeDepth
**    g_SA_AppData.cTlmPipeName
**    g_SA_AppData.TlmPipeId
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 SA_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_SA_AppData.usSchPipeDepth = SA_SCH_PIPE_DEPTH;
    memset((void*)g_SA_AppData.cSchPipeName, '\0', sizeof(g_SA_AppData.cSchPipeName));
    strncpy(g_SA_AppData.cSchPipeName, "SA_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_SA_AppData.SchPipeId,
                                 g_SA_AppData.usSchPipeDepth,
                                 g_SA_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(SA_WAKEUP_MID, g_SA_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("SA - Sch Pipe failed to subscribe to SA_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto SA_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("SA - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto SA_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_SA_AppData.usCmdPipeDepth = SA_CMD_PIPE_DEPTH ;
    memset((void*)g_SA_AppData.cCmdPipeName, '\0', sizeof(g_SA_AppData.cCmdPipeName));
    strncpy(g_SA_AppData.cCmdPipeName, "SA_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_SA_AppData.CmdPipeId,
                                 g_SA_AppData.usCmdPipeDepth,
                                 g_SA_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(SA_CMD_MID, g_SA_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("SA - CMD Pipe failed to subscribe to SA_CMD_MID. (0x%08X)\n", iStatus);
            goto SA_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(SA_SEND_HK_MID, g_SA_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("SA - CMD Pipe failed to subscribe to SA_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto SA_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("SA - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto SA_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_SA_AppData.usStatePipeDepth = SA_STATE_PIPE_DEPTH;
    memset((void*)g_SA_AppData.cStatePipeName, '\0', sizeof(g_SA_AppData.cStatePipeName));
    strncpy(g_SA_AppData.cStatePipeName, "SA_STATE_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_SA_AppData.StatePipeId,
                                 g_SA_AppData.usStatePipeDepth,
                                 g_SA_AppData.cStatePipeName);
    if (iStatus == CFE_SUCCESS)
    {
    	/* State Data Messages are subscribed to during table initiation */
    }
    else
    {
        CFE_ES_WriteToSysLog("SA - Failed to create STATE pipe (0x%08X)\n", iStatus);
        goto SA_InitPipe_Exit_Tag;
    }

SA_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: SA_InitData
**
** Purpose: To initialize global variables used by SA application
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
**    SA_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_SA_AppData.InData
**    g_SA_AppData.OutData
**    g_SA_AppData.HkTlm
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 SA_InitData()
{
    int32  iStatus=CFE_SUCCESS;
    /* Init housekeeping packet */
    memset((void*)&g_SA_AppData.HkTlm, 0x00, sizeof(g_SA_AppData.HkTlm));
    CFE_SB_InitMsg(&g_SA_AppData.HkTlm,
                   SA_HK_TLM_MID, sizeof(g_SA_AppData.HkTlm), TRUE);

    CFE_ES_PoolCreate(&g_SA_AppData.MemPoolHandle,
    		g_SA_AppData.MemPoolBuffer,
			sizeof(g_SA_AppData.MemPoolBuffer));
    return (iStatus);
}
    
/*=====================================================================================
** Name: SA_InitApp
**
** Purpose: To initialize all data local to and used by SA application
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
**    SA_InitEvent
**    SA_InitPipe
**    SA_InitData
**
** Called By:
**    SA_AppMain
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 SA_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_SA_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SA - Failed to register the app (0x%08X)\n", iStatus);
        goto SA_InitApp_Exit_Tag;
    }

    if ((SA_InitEvent() != CFE_SUCCESS) || 
        (SA_InitPipe() != CFE_SUCCESS) || 
        (SA_InitData() != CFE_SUCCESS) ||
		(SA_InitFrmtTable() != CFE_SUCCESS) ||
		(SA_AllocMem() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto SA_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&SA_CleanupCallback);

SA_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SA_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "SA - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("SA - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: SA_CleanupCallback
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_CleanupCallback()
{
	/* Not used */
}
    
/*=====================================================================================
** Name: SA_RcvMsg
**
** Purpose: To receive and process messages for SA application
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
**    SA_ProcessNewCmds
**    SA_ProcessNewData
**    SA_SendOutData
**
** Called By:
**    SA_Main
**
** Global Inputs/Reads:
**    g_SA_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_SA_AppData.uiRunStatus
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 SA_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SA_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_SA_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SA_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case SA_WAKEUP_MID:
                SA_ProcessNewCmds();
                SA_ProcessNewData();


                break;

            default:
                CFE_EVS_SendEvent(SA_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "SA - Recvd invalid SCH msgId (0x%08X)", MsgId);
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
        CFE_EVS_SendEvent(SA_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SA: SB pipe read error (0x%08X), app will exit", iStatus);
        g_SA_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: SA_ProcessNewData
**
** Purpose: To process incoming data subscribed by SA application
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
**    SA_RcvMsg
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   StateMsgPtr=NULL;
    CFE_SB_MsgId_t  StateMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&StateMsgPtr, g_SA_AppData.StatePipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
        	StateMsgId = CFE_SB_GetMsgId(StateMsgPtr);

        	if (SA_GrabPtrs() == CFE_SUCCESS) {
            	int32 field = FRMT_TBL_GetFlaggedFieldId(g_SA_AppData.SatFrmtPtr,
						StateMsgId);

            	iStatus = FRMT_TBL_InsertData(g_SA_AppData.SatFrmtPtr,
						field,
						CFE_SB_GetUserData(StateMsgPtr),
						g_SA_AppData.SatStatePtr);

            	if (iStatus != CFE_SUCCESS) {
            		CFE_EVS_SendEvent(SA_PRCSSDATA_ERR_EID, CFE_EVS_ERROR,
            				"SA: Process Data could not save state data point Error: 0x%08X",
							iStatus);
            	}

        	}
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(SA_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "SA: CMD pipe read error (0x%08X)", iStatus);
            g_SA_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: SA_ProcessNewCmds
**
** Purpose: To process incoming command messages for SA application
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
**    SA_ProcessNewAppCmds
**    SA_ReportHousekeeping
**
** Called By:
**    SA_RcvMsg
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
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_SA_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case SA_CMD_MID:
                    SA_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case SA_SEND_HK_MID:
                    SA_ReportHousekeeping();
                    break;

                default:
                	if (SA_IsValidMsgIdFlag(CmdMsgId)) {
                		SA_ProcessNewData();
                	}
                	else {
                		CFE_EVS_SendEvent(SA_MSGID_ERR_EID, CFE_EVS_ERROR,
                							"SA - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                	}
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(SA_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "SA: CMD pipe read error (0x%08X)", iStatus);
            g_SA_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: SA_ProcessNewAppCmds
**
** Purpose: To process command messages targeting SA application
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
**    SA_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_SA_AppData.HkTlm.usCmdCnt
**    g_SA_AppData.HkTlm.usCmdErrCnt
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case SA_NOOP_CC:
                g_SA_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(SA_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "SA - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case SA_RESET_CC:
                g_SA_AppData.HkTlm.usCmdCnt = 0;
                g_SA_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(SA_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "SA - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            case SA_DATAREQ_CC:
            	SA_SatStateDataReqCmd(MsgPtr);
            	break;
            default:
                g_SA_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(SA_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "SA - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}

/*=====================================================================================
** Name: SA_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
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
**    SEM_RcvMsg
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_SendOutData()
{
    /* Publish the Satellite State Data to any applications that could need it */

	if (SA_GrabPtrs() != CFE_SUCCESS) {
		return;
	}

	/* WARN Copying data is not the best solution if the satellite state is very large */
	/* create local data */
	int32 size = FRMT_TBL_GetTotalSize(g_SA_AppData.SatFrmtPtr);
	char buffer[size + CFE_SB_TLM_HDR_SIZE];

	/* Initialize The Message */
	CFE_SB_InitMsg(buffer, SA_OUT_DATA_MID, (size + CFE_SB_TLM_HDR_SIZE), TRUE);

	CFE_PSP_MemCpy(CFE_SB_GetUserData((CFE_SB_MsgPtr_t)buffer), g_SA_AppData.SatStatePtr, size);

	/* Send the Message */

	CFE_SB_SendMsg((CFE_SB_MsgPtr_t) buffer);
} /* end SA_SendOutData */

/*=====================================================================================
** Name: SA_ReportHousekeeping
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
**    SA_ProcessNewCmds
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
** Author(s):  GSFC, unk
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_SA_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_SA_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: SA_AppMain
**
** Purpose: To define SA application's entry point and main process loop
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
**    SA_InitApp
**    SA_RcvMsg
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
** Author(s):  unk 
**
** History:  Date Written  2018-04-18
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SA_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SA_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (SA_InitApp() != CFE_SUCCESS)
    {
        g_SA_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(SA_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(SA_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(SA_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_SA_AppData.uiRunStatus) == TRUE)
    {
        SA_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SA_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_SA_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file sa_app.c
**=====================================================================================*/
    
