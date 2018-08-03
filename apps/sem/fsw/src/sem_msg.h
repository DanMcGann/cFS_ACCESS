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
** File Name:  sem_msg.h
**
** Title:  Message Definition Header File for SEM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To define SEM's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SEM_MSG_H_
#define _SEM_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/



/*
** Local Defines
*/

/*
** SEM command codes
*/
#define SEM_NOOP_CC                 0
#define SEM_RESET_CC                1
#define SEM_SCHPLAN_CC				2
#define SEM_AUTOMODE_CC				3

/*
** Local Structure Declarations
**
** SEE ALSO: #SEM_HK_TLM_MID
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    uint32				CurrentSchedule;
    uint8 				ActionsSkipped;

} SEM_HkTlm_t;

/*
 *
 * SEE ALSO: #SEM_NOOP_CC #SEM_RESET_CC
 */
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} SEM_NoArgCmd_t;


/*
 * SEE ALSO: #SEM_SCHPLAN_CC
 */
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    void*  plan;
} SEM_SchPlanCmd_t;

/*
 * SEE ALSO: #SEM_AUTOMODE_CC
 */
typedef struct {
	uint8 ucCmdHeater[CFE_SB_CMD_HDR_SIZE];
	boolean AutoMode;
} SEM_BooleanCmd_t;




/* Unused data Structures */
typedef struct
{
    uint32  counter;

} SEM_InData_t;

/*
 * SEE ALSO: #SEM_OUT_DATA_MID
 */
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} SEM_OutData_t;

#endif /* _SEM_MSG_H_ */

/*=======================================================================================
** End of file sem_msg.h
**=====================================================================================*/
    
