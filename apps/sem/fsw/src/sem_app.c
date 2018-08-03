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
** File Name:  sem_app.c
**
** Title:  Function Definitions for SEM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  This source file contains all necessary function definitions to run SEM
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
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/


/*======================================================================================
** Include Files
**=====================================================================================*/
#include <string.h>

#include "cfe.h"

#include "access_lib.h"

#include "sem_platform_cfg.h"
#include "sem_mission_cfg.h"

#include "sem_app.h"
#include "sem_tables.h"
#include "sem_cmds.h"
#include "sem_exec.h"

#include "sa_msgids.h"
/*
** Global Variables
*/
SEM_AppData_t g_SEM_AppData;

    
/*=====================================================================================
** Name: SEM_InitEvent
**
** Purpose: To initialize and register event table for SEM application
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
**    SEM_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_SEM_AppData.EventTbl
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
int32 SEM_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_SEM_AppData.EventTbl, 0x00, sizeof(g_SEM_AppData.EventTbl));

    g_SEM_AppData.EventTbl[0].EventID = SEM_RESERVED_EID;
    g_SEM_AppData.EventTbl[1].EventID = SEM_INF_EID;
    g_SEM_AppData.EventTbl[2].EventID = SEM_INIT_INF_EID;
    g_SEM_AppData.EventTbl[3].EventID = SEM_ILOAD_INF_EID;
    g_SEM_AppData.EventTbl[4].EventID = SEM_CDS_INF_EID;
    g_SEM_AppData.EventTbl[5].EventID = SEM_CMD_INF_EID;
    g_SEM_AppData.EventTbl[6].EventID = SEM_VERIFY_TBL_INF_EID;


    g_SEM_AppData.EventTbl[ 7].EventID = SEM_ERR_EID;
    g_SEM_AppData.EventTbl[ 8].EventID = SEM_CRIT_ERR_EID;


    g_SEM_AppData.EventTbl[ 9].EventID = SEM_INIT_ERR_EID;
    g_SEM_AppData.EventTbl[10].EventID = SEM_ILOAD_ERR_EID;
    g_SEM_AppData.EventTbl[11].EventID = SEM_CDS_ERR_EID;
    g_SEM_AppData.EventTbl[12].EventID = SEM_CMD_ERR_EID;
    g_SEM_AppData.EventTbl[13].EventID = SEM_PIPE_ERR_EID;
    g_SEM_AppData.EventTbl[14].EventID = SEM_MSGID_ERR_EID;
    g_SEM_AppData.EventTbl[15].EventID = SEM_MSGLEN_ERR_EID;

    g_SEM_AppData.EventTbl[16].EventID = SEM_INIT_TBL_ERR_EID;
    g_SEM_AppData.EventTbl[17].EventID = SEM_VERIFY_TBL_ERR_EID;
    g_SEM_AppData.EventTbl[18].EventID = SEM_GRABPTRS_TBL_ERR_EID;
    g_SEM_AppData.EventTbl[19].EventID = SEM_DROPPTRS_TBL_ERR_EID;
    g_SEM_AppData.EventTbl[20].EventID = SEM_INIT_TBL_CDS_EID;
    g_SEM_AppData.EventTbl[21].EventID = SEM_INVALID_ACTION_ERR_EID;
    g_SEM_AppData.EventTbl[22].EventID = SEM_INVALID_SCHLOAD_ERR_EID;
    g_SEM_AppData.EventTbl[23].EventID = SEM_MANAGE_TBL_ERR_EID;


    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_SEM_AppData.EventTbl,
                               SEM_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SEM - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: SEM_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for SEM application
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
**    SEM_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_SEM_AppData.usSchPipeDepth
**    g_SEM_AppData.cSchPipeName
**    g_SEM_AppData.SchPipeId
**    g_SEM_AppData.usCmdPipeDepth
**    g_SEM_AppData.cCmdPipeName
**    g_SEM_AppData.CmdPipeId
**    g_SEM_AppData.usTlmPipeDepth
**    g_SEM_AppData.cTlmPipeName
**    g_SEM_AppData.TlmPipeId
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
int32 SEM_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_SEM_AppData.usSchPipeDepth = SEM_SCH_PIPE_DEPTH;
    memset((void*)g_SEM_AppData.cSchPipeName, '\0', sizeof(g_SEM_AppData.cSchPipeName));
    strncpy(g_SEM_AppData.cSchPipeName, "SEM_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_SEM_AppData.SchPipeId,
                                 g_SEM_AppData.usSchPipeDepth,
                                 g_SEM_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(SEM_WAKEUP_MID, g_SEM_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("SEM - Sch Pipe failed to subscribe to SEM_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto SEM_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("SEM - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto SEM_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_SEM_AppData.usCmdPipeDepth = SEM_CMD_PIPE_DEPTH ;
    memset((void*)g_SEM_AppData.cCmdPipeName, '\0', sizeof(g_SEM_AppData.cCmdPipeName));
    strncpy(g_SEM_AppData.cCmdPipeName, "SEM_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_SEM_AppData.CmdPipeId,
                                 g_SEM_AppData.usCmdPipeDepth,
                                 g_SEM_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(SEM_CMD_MID, g_SEM_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("SEM - CMD Pipe failed to subscribe to SEM_CMD_MID. (0x%08X)\n", iStatus);
            goto SEM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(SEM_SEND_HK_MID, g_SEM_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("SEM - CMD Pipe failed to subscribe to SEM_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto SEM_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("SEM - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto SEM_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_SEM_AppData.usTlmPipeDepth = SEM_TLM_PIPE_DEPTH;
    memset((void*)g_SEM_AppData.cTlmPipeName, '\0', sizeof(g_SEM_AppData.cTlmPipeName));
    strncpy(g_SEM_AppData.cTlmPipeName, "SEM_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_SEM_AppData.TlmPipeId,
                                 g_SEM_AppData.usTlmPipeDepth,
                                 g_SEM_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
    	if (CFE_SB_Subscribe(SA_OUT_DATA_MID, g_SEM_AppData.TlmPipeId) != CFE_SUCCESS) {
    		CFE_ES_WriteToSysLog("SEM - CMD Pipe failed to subscribe to SA_OUT_DATA_MID");
    	}
    }
    else
    {
        CFE_ES_WriteToSysLog("SEM - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto SEM_InitPipe_Exit_Tag;
    }

SEM_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: SEM_InitData
**
** Purpose: To initialize global variables used by SEM application
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
**    SEM_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_SEM_AppData.InData
**    g_SEM_AppData.OutData
**    g_SEM_AppData.HkTlm
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
int32 SEM_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init output data */
    memset((void*)&g_SEM_AppData.OutData, 0x00, sizeof(g_SEM_AppData.OutData));
    CFE_SB_InitMsg(&g_SEM_AppData.OutData,
                   SEM_OUT_DATA_MID, sizeof(g_SEM_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_SEM_AppData.HkTlm, 0x00, sizeof(g_SEM_AppData.HkTlm));
    CFE_SB_InitMsg(&g_SEM_AppData.HkTlm,
                   SEM_HK_TLM_MID, sizeof(g_SEM_AppData.HkTlm), TRUE);

    CFE_ES_PoolCreate(&g_SEM_AppData.MemPoolHandle,
    		g_SEM_AppData.MemPoolBuffer,
			SEM_MEM_POOL_BUFFER_SIZE);

    return (iStatus);
}
    
/*=====================================================================================
** Name: SEM_InitApp
**
** Purpose: To initialize all data local to and used by SEM application
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
**    SEM_InitEvent
**    SEM_InitPipe
**    SEM_InitData
**
** Called By:
**    SEM_AppMain
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 SEM_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_SEM_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SEM - Failed to register the app (0x%08X)\n", iStatus);
        goto SEM_InitApp_Exit_Tag;
    }

    if ((SEM_InitEvent() != CFE_SUCCESS) || 
        (SEM_InitPipe() != CFE_SUCCESS) || 
        (SEM_InitData() != CFE_SUCCESS) ||
		(SEM_InitTables() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto SEM_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&SEM_CleanupCallback);

SEM_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SEM_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "SEM - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("SEM - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: SEM_CleanupCallback
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_CleanupCallback()
{
    /* Nothing to clean up one app close */
}
    
/*=====================================================================================
** Name: SEM_RcvMsg
**
** Purpose: To receive and process messages for SEM application
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
**    SEM_ProcessNewCmds
**    SEM_ProcessNewData
**    SEM_SendOutData
**
** Called By:
**    SEM_Main
**
** Global Inputs/Reads:
**    g_SEM_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_SEM_AppData.uiRunStatus
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
int32 SEM_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SEM_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_SEM_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SEM_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case SEM_WAKEUP_MID:
            	/* Respond to incoming Commands */
                SEM_ProcessNewCmds();

                /* Execute scheduled actions */
                SEM_ProcessSchedule();

                /* Manage Tables */
                SEM_ManageTables();

                /* Send out data of Current Action Info */
                SEM_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(SEM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "SEM - Recvd invalid SCH msgId (0x%08X)", MsgId);
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
        CFE_EVS_SendEvent(SEM_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SEM: SB pipe read error (0x%08X), app will exit", iStatus);
        g_SEM_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: SEM_ProcessNewData
**
** Purpose: To process incoming data subscribed by SEM application
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
**    SEM_RcvMsg
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_SEM_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
            	case SA_OUT_DATA_MID:
            		/* Free the Currently saved data */
            		CFE_ES_PutPoolBuf(g_SEM_AppData.MemPoolHandle,
            				(uint32 *)g_SEM_AppData.SatStatePtr);

            		/* Save the Satellite State Data Locally */
            		void* statePtr = CFE_SB_GetUserData(TlmMsgPtr);

            		CFE_ES_GetPoolBuf((uint32 **)&g_SEM_AppData.MemPoolBuffer,
            			g_SEM_AppData.MemPoolHandle,
						CFE_SB_GetUserDataLength(TlmMsgPtr));

            		CFE_PSP_MemCpy(g_SEM_AppData.SatStatePtr,
            				statePtr,
							CFE_SB_GetUserDataLength(TlmMsgPtr));
            		break;

                default:
                    CFE_EVS_SendEvent(SEM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "SEM - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(SEM_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "SEM: CMD pipe read error (0x%08X)", iStatus);
            g_SEM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: SEM_ProcessNewCmds
**
** Purpose: To process incoming command messages for SEM application
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
**    SEM_ProcessNewAppCmds
**    SEM_ReportHousekeeping
**
** Called By:
**    SEM_RcvMsg
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_SEM_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case SEM_CMD_MID:
                    SEM_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case SEM_SEND_HK_MID:
                    SEM_ReportHousekeeping();
                    break;

                default:
                    CFE_EVS_SendEvent(SEM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "SEM - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(SEM_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "SEM: CMD pipe read error (0x%08X)", iStatus);
            g_SEM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: SEM_ProcessNewAppCmds
**
** Purpose: To process command messages targeting SEM application
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
**    SEM_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_SEM_AppData.HkTlm.usCmdCnt
**    g_SEM_AppData.HkTlm.usCmdErrCnt
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
void SEM_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case SEM_NOOP_CC:
                g_SEM_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(SEM_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "SEM - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case SEM_RESET_CC:
                g_SEM_AppData.HkTlm.usCmdCnt = 0;
                g_SEM_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(SEM_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "SEM - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            case SEM_SCHPLAN_CC:
                g_SEM_AppData.HkTlm.usCmdCnt++;
            	SEM_SchPlanCmd(MsgPtr);
            	break;

            case SEM_AUTOMODE_CC:
                g_SEM_AppData.HkTlm.usCmdCnt++;
                SEM_AutoModeCmd(MsgPtr);
            	break;

            default:
                g_SEM_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(SEM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "SEM - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}


/*=====================================================================================
** Name: SEM_ReportHousekeeping
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
**    SEM_ProcessNewCmds
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_SEM_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_SEM_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: SEM_SendOutData
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
void SEM_SendOutData()
{
	/* THIS FUNCTION IS NOT USED - Could be removed with no error */
	/* However, it is a good place to implement any out data needed by applications
	 * Such as which action is currently being executed etc. This functionality was
	 * not included in ACCESS Version 0.0 because of time constrains and a lack of
	 * necessity for such funtionality.
	 */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_SEM_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_SEM_AppData.OutData);
}
    
/*=====================================================================================
** Name: SEM_VerifyCmdLength
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
**    SEM_ProcessNewCmds
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
boolean SEM_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            CFE_EVS_SendEvent(SEM_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "SEM - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_SEM_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: SEM_AppMain
**
** Purpose: To define SEM application's entry point and main process loop
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
**    SEM_InitApp
**    SEM_RcvMsg
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
** History:  Date Written  2018-04-24
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void SEM_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SEM_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (SEM_InitApp() != CFE_SUCCESS)
    {
        g_SEM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(SEM_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(SEM_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(SEM_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_SEM_AppData.uiRunStatus) == TRUE)
    {
        SEM_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SEM_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_SEM_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file sem_app.c
**=====================================================================================*/
    
