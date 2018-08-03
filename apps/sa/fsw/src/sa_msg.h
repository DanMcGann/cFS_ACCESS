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
** File Name:  sa_msg.h
**
** Title:  Message Definition Header File for SA Application
**
** $Author:    unk
** $Revision: 1.1 $
** $Date:      2018-04-18
**
** Purpose:  To define SA's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-18 | unk | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SA_MSG_H_
#define _SA_MSG_H_

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
** SA command codes
*/
#define SA_NOOP_CC                 0
#define SA_RESET_CC                1
#define SA_DATAREQ_CC			   2

/*
** Local Structure Declarations
**
** SEE ALSO: #SA_HK_TLM_MID
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */
    uint8 				SatFrmtTblVerifyErrCnt;

} SA_HkTlm_t;


/*
 *
 * SEE ALSO: #SA_NOOP_CC #SA_RESET_CC #SA_DATAREQ_CC
 */
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} SA_NoArgCmd_t;




#endif /* _SA_MSG_H_ */

/*=======================================================================================
** End of file sa_msg.h
**=====================================================================================*/
    
