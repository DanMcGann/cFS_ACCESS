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
** File Name:  gsds_app.c
**
** Title:  Function Definitions for GSDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This source file contains all necessary function definitions to run GSDS
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
**   2018-04-04 | Daniel McGann | Build #: Code Started
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

#include "gsds_platform_cfg.h"
#include "gsds_mission_cfg.h"
#include "gsds_app.h"
#include "gsds_table.h"
#include "gsds_cmd.h"

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
GSDS_AppData_t  g_GSDS_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/
    
/*=====================================================================================
** Name: GSDS_InitEvent
**
** Purpose: To initialize and register event table for GSDS application
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
**    GSDS_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_GSDS_AppData.EventTbl
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 GSDS_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_GSDS_AppData.EventTbl, 0x00, sizeof(g_GSDS_AppData.EventTbl));

    g_GSDS_AppData.EventTbl[0].EventID = GSDS_RESERVED_EID;
    g_GSDS_AppData.EventTbl[1].EventID = GSDS_INF_EID;
    g_GSDS_AppData.EventTbl[2].EventID = GSDS_INIT_INF_EID;
    g_GSDS_AppData.EventTbl[3].EventID = GSDS_ILOAD_INF_EID;
    g_GSDS_AppData.EventTbl[4].EventID = GSDS_CDS_INF_EID;
    g_GSDS_AppData.EventTbl[5].EventID = GSDS_CMD_INF_EID;
    g_GSDS_AppData.EventTbl[6].EventID = GSDS_TBLINIT_INF_EID;
    g_GSDS_AppData.EventTbl[7].EventID = GSDS_VERIY_TBL_INF_EID;


    g_GSDS_AppData.EventTbl[ 8].EventID = GSDS_ERR_EID;
    g_GSDS_AppData.EventTbl[ 9].EventID = GSDS_INIT_ERR_EID;
    g_GSDS_AppData.EventTbl[10].EventID = GSDS_ILOAD_ERR_EID;
    g_GSDS_AppData.EventTbl[11].EventID = GSDS_CDS_ERR_EID;
    g_GSDS_AppData.EventTbl[12].EventID = GSDS_CMD_ERR_EID;
    g_GSDS_AppData.EventTbl[13].EventID = GSDS_PIPE_ERR_EID;
    g_GSDS_AppData.EventTbl[14].EventID = GSDS_MSGID_ERR_EID;
    g_GSDS_AppData.EventTbl[15].EventID = GSDS_MSGLEN_ERR_EID;
    g_GSDS_AppData.EventTbl[16].EventID = GSDS_TBLINIT_ERR_EID;
    g_GSDS_AppData.EventTbl[17].EventID = GSDS_VERIY_TBL_ERR_EID;
    g_GSDS_AppData.EventTbl[18].EventID = GSDS_ADDGS_ERR_EID;
    g_GSDS_AppData.EventTbl[19].EventID = GSDS_GETGGSDATA_ERR_EID;
    g_GSDS_AppData.EventTbl[20].EventID = GSDS_GRABPTRS_ERR_EID;
    g_GSDS_AppData.EventTbl[21].EventID = GSDS_MANAGE_TBL_ERR_EID;



    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_GSDS_AppData.EventTbl,
                               GSDS_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("GSDS - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: GSDS_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for GSDS application
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
**    GSDS_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_GSDS_AppData.usSchPipeDepth
**    g_GSDS_AppData.cSchPipeName
**    g_GSDS_AppData.SchPipeId
**    g_GSDS_AppData.usCmdPipeDepth
**    g_GSDS_AppData.cCmdPipeName
**    g_GSDS_AppData.CmdPipeId
**    g_GSDS_AppData.usTlmPipeDepth
**    g_GSDS_AppData.cTlmPipeName
**    g_GSDS_AppData.TlmPipeId
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 GSDS_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_GSDS_AppData.usSchPipeDepth = GSDS_SCH_PIPE_DEPTH;
    memset((void*)g_GSDS_AppData.cSchPipeName, '\0', sizeof(g_GSDS_AppData.cSchPipeName));
    strncpy(g_GSDS_AppData.cSchPipeName, "GSDS_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_GSDS_AppData.SchPipeId,
                                 g_GSDS_AppData.usSchPipeDepth,
                                 g_GSDS_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(GSDS_WAKEUP_MID, g_GSDS_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("GSDS - Sch Pipe failed to subscribe to GSDS_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto GSDS_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("GSDS - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto GSDS_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_GSDS_AppData.usCmdPipeDepth = GSDS_CMD_PIPE_DEPTH ;
    memset((void*)g_GSDS_AppData.cCmdPipeName, '\0', sizeof(g_GSDS_AppData.cCmdPipeName));
    strncpy(g_GSDS_AppData.cCmdPipeName, "GSDS_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_GSDS_AppData.CmdPipeId,
                                 g_GSDS_AppData.usCmdPipeDepth,
                                 g_GSDS_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(GSDS_CMD_MID, g_GSDS_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("GSDS - CMD Pipe failed to subscribe to GSDS_CMD_MID. (0x%08X)\n", iStatus);
            goto GSDS_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(GSDS_SEND_HK_MID, g_GSDS_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("GSDS - CMD Pipe failed to subscribe to GSDS_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto GSDS_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("GSDS - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto GSDS_InitPipe_Exit_Tag;
    }

GSDS_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: GSDS_InitData
**
** Purpose: To initialize global variables used by GSDS application
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
**    GSDS_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_GSDS_AppData.InData
**    g_GSDS_AppData.OutData
**    g_GSDS_AppData.HkTlm
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 GSDS_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init housekeeping packet */
    memset((void*)&g_GSDS_AppData.HkTlm, 0x00, sizeof(g_GSDS_AppData.HkTlm));
    CFE_SB_InitMsg(&g_GSDS_AppData.HkTlm,
                   GSDS_HK_TLM_MID, sizeof(g_GSDS_AppData.HkTlm), TRUE);

    return (iStatus);
}
    
/*=====================================================================================
** Name: GSDS_InitApp
**
** Purpose: To initialize all data local to and used by GSDS application
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
**    GSDS_InitEvent
**    GSDS_InitPipe
**    GSDS_InitData
**
** Called By:
**    GSDS_AppMain
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 GSDS_InitApp()
{
    int32  iStatus=CFE_SUCCESS;
    g_GSDS_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();

    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("GSDS - Failed to register the app (0x%08X)\n", iStatus);
        goto GSDS_InitApp_Exit_Tag;
    }

    if ((GSDS_InitEvent() != CFE_SUCCESS) ||
        (GSDS_InitPipe() != CFE_SUCCESS) ||
        (GSDS_InitData() != CFE_SUCCESS) ||
		(GSDS_InitTable() != CFE_SUCCESS) ||
		(GSDS_InitFrmtTable() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto GSDS_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&GSDS_CleanupCallback);

GSDS_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(GSDS_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "GSDS - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("GSDS - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: GSDS_CleanupCallback
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void GSDS_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other c        return CFE_SUCCESS;leanup here */
}
    
/*=====================================================================================
** Name: GSDS_RcvMsg
**
** Purpose: To receive and process messages for GSDS application
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
**    GSDS_ProcessNewCmds
**    GSDS_ProcessNewData
**    GSDS_SendOutData
**
** Called By:
**    GSDS_Main
**
** Global Inputs/Reads:
**    g_GSDS_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_GSDS_AppData.uiRunStatus
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 GSDS_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(GSDS_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_GSDS_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(GSDS_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case GSDS_WAKEUP_MID:
                GSDS_ProcessNewCmds();
                break;

            default:
                CFE_EVS_SendEvent(GSDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "GSDS - Recvd invalid SCH msgId (0x%08X)", MsgId);
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
        CFE_EVS_SendEvent(GSDS_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "GSDS: SB pipe read error (0x%08X), app will exit", iStatus);
        g_GSDS_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: GSDS_ProcessNewCmds
**
** Purpose: To process incoming command messages for GSDS application
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
**    GSDS_ProcessNewAppCmds
**    GSDS_ReportHousekeeping
**
** Called By:
**    GSDS_RcvMsg
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void GSDS_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_GSDS_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case GSDS_CMD_MID:
                    GSDS_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case GSDS_SEND_HK_MID:
                    GSDS_ReportHousekeeping();
                    break;
                default:
                    CFE_EVS_SendEvent(GSDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "GSDS - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(GSDS_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "GSDS: CMD pipe read error (0x%08X)", iStatus);
            g_GSDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: GSDS_ProcessNewAppCmds
**
** Purpose: To process command messages targeting GSDS application
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
**    GSDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_GSDS_AppData.HkTlm.usCmdCnt
**    g_GSDS_AppData.HkTlm.usCmdErrCnt
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void GSDS_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case GSDS_NOOP_CC:
                g_GSDS_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(GSDS_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "GSDS - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case GSDS_RESET_CC:
                g_GSDS_AppData.HkTlm.usCmdCnt = 0;
                g_GSDS_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(GSDS_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "GSDS - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            case GSDS_GSREQID_CC:
            	GSDS_GsReqIdCmd(MsgPtr);
            	break;

            case GSDS_GSREQMATCH_CC:
				GSDS_GsReqMatchCmd(MsgPtr);
				break;

            case GSDS_ADDGS_CC:
            	GSDS_AddGsCmd(MsgPtr);
            	break;

            case GSDS_EDITGS_CC:
            	GSDS_EditGsCmd(MsgPtr);
            	break;

            default:
                g_GSDS_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(GSDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "GSDS - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: GSDS_ReportHousekeeping
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
**    GSDS_ProcessNewCmds
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void GSDS_ReportHousekeeping()
{
	/* Manage Both the GS State table and GS Format table on each hk request */
	int32 tbl_status;
	if (GSDS_DropPtrs() == CFE_SUCCESS) {

		/* Manage and Report on State Table */
		tbl_status = CFE_TBL_Manage(g_GSDS_AppData.StateHandle);

		if (tbl_status != CFE_SUCCESS && tbl_status != CFE_TBL_INFO_UPDATED) {
			CFE_EVS_SendEvent(GSDS_MANAGE_TBL_ERR_EID, CFE_EVS_ERROR,
					"GSDS: State Table manage Error: 0x%08X", tbl_status);
		}
		else if (tbl_status == CFE_TBL_INFO_UPDATED) {
			g_GSDS_AppData.HkTlm.StateTableLoadCnt++;
		}

		/* Manage and Report on Format Table */
		tbl_status = CFE_TBL_Manage(g_GSDS_AppData.GsFrmtHandle);

		if (tbl_status != CFE_SUCCESS && tbl_status != CFE_TBL_INFO_UPDATED) {
			CFE_EVS_SendEvent(GSDS_MANAGE_TBL_ERR_EID, CFE_EVS_ERROR,
					"GSDS: Format Table manage Error: 0x%08X", tbl_status);
		}
		else if (tbl_status == CFE_TBL_INFO_UPDATED) {
			g_GSDS_AppData.HkTlm.FrmtTableLoadCnt++;
		}
	}

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_GSDS_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_GSDS_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: GSDS_VerifyCmdLength
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
**    GSDS_ProcessNewCmds
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean GSDS_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            CFE_EVS_SendEvent(GSDS_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "GSDS - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_GSDS_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: GSDS_AppMain
**
** Purpose: To define GSDS application's entry point and main process loop
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
**    GSDS_InitApp
**    GSDS_RcvMsg
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
** History:  Date Written  2018-04-04
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void GSDS_AppMain()
{
	/* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(GSDS_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (GSDS_InitApp() != CFE_SUCCESS)
    {
        g_GSDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(GSDS_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(GSDS_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(GSDS_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_GSDS_AppData.uiRunStatus) == TRUE)
    {
        GSDS_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(GSDS_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_GSDS_AppData.uiRunStatus);
} 

/*=======================================================================================
** End of file gsds_app.c
**=====================================================================================*/
    
