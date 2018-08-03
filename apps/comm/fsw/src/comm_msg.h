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
** File Name:  comm_msg.h
**
** Title:  Message Definition Header File for COMM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define COMM's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#include "comm_linkdefs.h"

#ifndef _COMM_MSG_H_
#define _COMM_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/

//TODO DOCS FOR MESSAGES

/*
** Local Defines
*/

/*
** COMM command codes
*/
#define COMM_NOOP_CC                 0
#define COMM_RESET_CC                1
#define COMM_REGISTERFILE_CC         2
#define COMM_STARTLINK_CC            3
#define COMM_STOPLINK_CC             4
#define COMM_ACKLINK_CC              5

/*
** Local Structure Declarations
*/
/* \brief COMM House Keeping Tlm Packet structure
 *
 * SEE ALSO: #COMM_HK_TLM_MID
 * */
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];/*<<\brief CCSDS Message Header */
    uint8              usCmdCnt;/*<<\brief Counter of Successful Commands */
    uint8              usCmdErrCnt;/*<<\brief Counter of invalid Commands Received */
    uint32 			   LinkFileFailCnt;/*<<\brief The number of files that have failed to link*/
    uint32			   LinkCnt;/*<<\brief The number of Links that have been successful*/
    uint16			   XLinkFailCnt;/*<<\brief The number of Cross Links that have failed */
    uint16			   DLinkFailCnt;/*<<\brief The number of Down Links that have Failed*/

} COMM_HkTlm_t;

/* \brief Structure for no argument command message
 *
 * SEE ALSO: #COMM_NOOP_CC #COMM_RESET_CC #COMM_STOPLINK_CC
 * */
typedef struct {
    uint8              	CmdHeader[CFE_SB_CMD_HDR_SIZE]; /*<< \brief CSDS Message Header*/
}COMM_NoArgCmd_t;

/* \brief Structure for Register File Command message
 *
 * SEE ALSO: #COMM_REGISTERFILE_CC
 * */
typedef struct {
    uint8              	CmdHeader[CFE_SB_CMD_HDR_SIZE];/*<< \brief Command Header*/
    COMM_LinkType_t		LinkType;/*<< \brief Type of link to register this file with */
    uint8 				Priority;/*<< \brief The priority of transferring this file */
    boolean 			Retain;/*<< \brief Flag indicating whether or not this file
    								 should linked on each instance of the specified link*/
    boolean				DelOnLink;/*<< \brief Flag indicating that this file should be
     	 	 	 	 	 	 	 	 removed from the satellite file system after successful link*/
    char 				File[OS_MAX_PATH_LEN];/*<< \brief The path and name of the file to
    							 	 transfer*/
}COMM_RegisterFileCmd_t;

/* \brief Structure for Start and Stop link commands
 *
 * SEE ALSO: #COMM_STARTLINK_CC
 * */
typedef struct {
    uint8              	CmdHeader[CFE_SB_CMD_HDR_SIZE];/*<< \brief Command header*/
    COMM_LinkType_t		LinkType;/*<< \brief The type of link to stop or start*/
    CFE_TIME_SysTime_t  LinkDur; /*<< \brief The length of time that this links will last */

}COMM_StartCmd_t;

/* \brief Structure for the Link Acknowledgment command
 *
 * SEE ALSO: #COMM_ACKLINK_CC
 * */
typedef struct {
    uint8              	CmdHeader[CFE_SB_CMD_HDR_SIZE];/*<< \brief Command Header*/
	int32 				NumFiles;/*<< \brief The number of files listed */
	char 				Files[][OS_MAX_PATH_LEN];/*<< \brief Files that were successfully
	 	 	 	 	 	 	 	 Received by the link target */
}COMM_AckMsg_t;


#endif /* _COMM_MSG_H_ */

/*=======================================================================================
** End of file comm_msg.h
**=====================================================================================*/
    
