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
** File Name:  conds_msg.h
**
** Title:  Message Definition Header File for CONDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-26
**
** Purpose:  To define CONDS's command and telemetry message definitions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-26 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _CONDS_MSG_H_
#define _CONDS_MSG_H_


/** \condscmd No Operation
**
**  \par Description
**       This commands has no effect, other than to generate a event message
**       to show life from the CONDS Application.
**
**  \condscmdmnemonic \CONDS_NOOP
**
**  \par Command Structure
**       #CONDS_NoArgCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - The CONDS usCmdCounter Housekeeping field will be incremented
**       - A #CONDS_CMD_INF_EIF debug event message will be sent with this CC
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**
**       Evidence of failure may be found in the following telemetry:
**       - command error counter will increment
**       - The #CONDS_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa #CONDS_NoopCmd
*/
#define CONDS_NOOP_CC               0

/** \condscmd Reset Counters
**
**  \par Description
**       This Command Resets the following Housekeeping counters. #CONDS_HkTlm_t
**       	- usCmdCnt - number of accepted commands
**       	- usCmdErrCnt - number of commands rejected
**       	- ConStateTblLoads - number of Constellation State Table Loads
**       	- FrmtTblLoads - number of Format Table Loads performed
**
**
**  \condscmdmnemonic \CONDS_RESETCTRS
**
**  \par Command Structure
**       #CONDS_NoArgCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - The CONDS Housekeeping counters will reset to 0.
**       - A #CONDS_CMD_INF_EIF debug event message will be sent with this CC
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**
**       Evidence of failure may be found in the following telemetry:
**       - command error counter will increment
**       - The #CONDS_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa #CONDS_NoopCmd
*/
#define CONDS_RESET_CC              1

/** \condscmd Data Request Command
**
**  \par Description
**       This initiates the output of a specified data type
**
**  \condscmdmnemonic \CONDS_DATAREQ
**
**  \par Command Structure
**       #CONDS_DataReqCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - The CONDS usCmdCounter Housekeeping field will be incremented
**       - A #CONDS_CMD_INF_EIF debug event message will be sent with this CC
**       - A CFE_SB_Msg will be send with the message Id specified in the cmd msg.
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - The software bus message sending failed.
**
**       Evidence of failure may be found in the following telemetry:
**       - command error counter will increment
**       - The #CONDS_DATAREQCMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa #CONDS_DataReqCmd
*/
#define CONDS_DATAREQ_CC			2

/*
** Message Data Structures
*/
/** \brief CONDS Housekeeping Telemetry Packet Data Structure
 *
 * SEE ALSO: #CONDS_HK_TLM_MID
 * */
typedef struct
{
    uint8              	TlmHeader[CFE_SB_TLM_HDR_SIZE];		/** < \brief CFE SB Message Header */
    uint8              	usCmdCnt;							/** < \brief Count of good commands accepted by CONDS */
    uint8              	usCmdErrCnt;						/** < \brief Count of commands rejected by CONDS */
    uint16 				ConStateTblLoads;					/** < \brief Count of number of ConState Table Loads */
    uint16				FrmtTblLoads;						/** < \brief Count of number of Format Table Loads */
} CONDS_HkTlm_t;


/* \brief CONDS No Argument Command Structure
 *
 * SEE ALSO: #CONDS_NOOP_CC #CONDS_RESET_CC
 * */
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE]; /** < \brief CFE SB Message Header*/
} CONDS_NoArgCmd_t;


/* \brief CONDS Data Request Command Structure
 *
 * SEE ALSO: #CONDS_DATAREQ_CC
 * */
typedef struct  {
	uint8				usCmdHeader[CFE_SB_CMD_HDR_SIZE]; 	/* <\brief CFE_SB MEssage Header */
	CFE_SB_MsgId_t		destId;								/* <\brief Message ID to which this data should be sent */
	char 				dataName[OS_MAX_API_NAME];			/* <\brief The Name of the data to send */
} CONDS_DataReqCmd_t;


#endif /* _CONDS_MSG_H_ */

/*=======================================================================================
** End of file conds_msg.h
**=====================================================================================*/
    
