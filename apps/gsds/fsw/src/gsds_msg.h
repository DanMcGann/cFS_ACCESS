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
** File Name:  gsds_msg.h
**
** Title:  Message Definition Header File for GSDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  To define GSDS's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _GSDS_MSG_H_
#define _GSDS_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "gsds_app.h"


/*
** Local Defines
*/

/*
** GSDS command codes
*/
#define GSDS_NOOP_CC                 0
#define GSDS_RESET_CC                1
#define GSDS_GSREQID_CC			 	 2
#define GSDS_GSREQMATCH_CC			 3
#define GSDS_ADDGS_CC				 4
#define GSDS_EDITGS_CC				 5


/*
** Local Structure Declarations
**
* SEE ALSO: #GSDS_HK_TLM_MID
* */
typedef struct
{
    uint8              	TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              	usCmdCnt;
    uint8              	usCmdErrCnt;

    uint16 			   	StateTableLoadCnt;
    uint16				StateTableVerifyErrCnt;

    uint16				FrmtTableLoadCnt;
    uint16				FrmtTableVerifyErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} GSDS_HkTlm_t;

/* Command Structures
 *
 * SEE ALSO: #GSDS_NOOP_CC #GSDS_RESET_CC
 * */
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} GSDS_NoArgCmd_t;


/*
 *
 *
 * SEE ALSO: #GSDS_GSREQID_CC
 * */
typedef struct
{
	uint8  			ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
	char 			id[GSDS_MAX_DATA_LEN];
} GSDS_GsReqIdCmd_t;


/*
 *
 * SEE ALSO: #GSDS_GSREQMATCH_CC
 * */
typedef struct
{
	uint8 			ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
	int32 			flag;
	char 			equal[GSDS_MAX_DATA_LEN];
} GSDS_GsReqMatchCmd_t;

/*
 *
 * SEE ALSO: #GSDS_ADDGS_CC
 * */
typedef struct
{
	uint8 			ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
	char			GsData[GSDS_MAX_DATA_LEN];

} GSDS_AddGsCmd_t;

/* Send Out Data Structures
 * #GSDS_OUT_DATA_MID
 *  */
typedef struct
{
	uint8  			ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
	uint16 			NumGs;
	char			GsData[GSDS_MAX_GS * GSDS_MAX_DATA_LEN];
} GSDS_GsDataOut_t;

/* TODO:  Add more private structure definitions here, if necessary. */


#endif /* _GSDS_MSG_H_ */

/*=======================================================================================
** End of file gsds_msg.h
**=====================================================================================*/
    
