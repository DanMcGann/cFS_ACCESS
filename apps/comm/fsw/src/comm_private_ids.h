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
** File Name:  comm_private_ids.h
**
** Title:  ID Header File for COMM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  This header file contains declarations and definitions of COMM's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _COMM_PRIVATE_IDS_H_
#define _COMM_PRIVATE_IDS_H_

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
*/

/* Event IDs */
#define COMM_RESERVED_EID  0

#define COMM_INF_EID        1
#define COMM_INIT_INF_EID   2
#define COMM_ILOAD_INF_EID  3
#define COMM_CDS_INF_EID    4
#define COMM_CMD_INF_EID    5
#define COMM_INITLINK_INF_EID 6

#define COMM_ERR_EID         51
#define COMM_INIT_ERR_EID    52
#define COMM_ILOAD_ERR_EID   53
#define COMM_CDS_ERR_EID     54
#define COMM_CMD_ERR_EID     55
#define COMM_PIPE_ERR_EID    56
#define COMM_MSGID_ERR_EID   57
#define COMM_MSGLEN_ERR_EID  58

#define COMM_VERIFYTBL_ERR_EID 59
#define COMM_GRABPTR_ERR_EID 60
#define COMM_DROPPTR_ERR_EID 61
#define COMM_ADDFILE_ERR_EID 62
#define COMM_HANDLEACK_ERR_EID 63
#define COMM_TBLINIT_ERR_EID 64
#define COMM_ACK_ERR_EID 65
#define COMM_INITLINK_ERR_EID 66

#define COMM_EVT_CNT  14


/* Error ids */
#define COMM_NO_ENTRY -1

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#endif /* _COMM_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file comm_private_ids.h
**=====================================================================================*/
    
