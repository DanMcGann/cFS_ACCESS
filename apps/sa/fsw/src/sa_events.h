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
** File Name:  sa_events.h
**
** Title:  ID Header File for SA Application
**
** $Author:    unk
** $Revision: 1.1 $
** $Date:      2018-04-18
**
** Purpose:  This header file contains declarations and definitions of SA's
** 	event Ids, and error codes.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-18 | unk | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SA_PRIVATE_IDS_H_
#define _SA_PRIVATE_IDS_H_

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
#define SA_RESERVED_EID  0

#define SA_INF_EID        1
#define SA_INIT_INF_EID   2
#define SA_ILOAD_INF_EID  3
#define SA_CDS_INF_EID    4
#define SA_CMD_INF_EID    5

#define SA_ERR_EID         51
#define SA_INIT_ERR_EID    52
#define SA_ILOAD_ERR_EID   53
#define SA_CDS_ERR_EID     54
#define SA_CMD_ERR_EID     55
#define SA_PIPE_ERR_EID    56
#define SA_MSGID_ERR_EID   57
#define SA_MSGLEN_ERR_EID  58

#define SA_INIT_TBL_ERR_EID 59
#define SA_VERIFY_TBL_ERR_EID 60
#define SA_GRABPTRS_ERR_EID 61
#define SA_DROPPTRS_ERR_EID 62
#define SA_ALLOCMEM_ERR_EID 63
#define SA_PRCSSDATA_ERR_EID 64
#define SA_FREEMEM_ERR_EID 65
#define SA_SUBMSGS_ERR_EID 66

#define SA_EVT_CNT  22

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

#endif /* _SA_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file sa_private_ids.h
**=====================================================================================*/
    
