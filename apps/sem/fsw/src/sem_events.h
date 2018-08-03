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
** File Name:  sem_private_ids.h
**
** Title:  ID Header File for SEM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  This header file contains declarations and definitions of SEM's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SEM_PRIVATE_IDS_H_
#define _SEM_PRIVATE_IDS_H_

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
#define SEM_RESERVED_EID  0

#define SEM_INF_EID        1
#define SEM_INIT_INF_EID   2
#define SEM_ILOAD_INF_EID  3
#define SEM_CDS_INF_EID    4
#define SEM_CMD_INF_EID    5
#define SEM_VERIFY_TBL_INF_EID 6

#define SEM_ERR_EID         51
#define SEM_CRIT_ERR_EID	99

#define SEM_INIT_ERR_EID    52
#define SEM_ILOAD_ERR_EID   53
#define SEM_CDS_ERR_EID     54
#define SEM_CMD_ERR_EID     55
#define SEM_PIPE_ERR_EID    56
#define SEM_MSGID_ERR_EID   57
#define SEM_MSGLEN_ERR_EID  58

#define SEM_INIT_TBL_ERR_EID 59
#define SEM_VERIFY_TBL_ERR_EID 60
#define SEM_GRABPTRS_TBL_ERR_EID 61
#define SEM_DROPPTRS_TBL_ERR_EID 62
#define SEM_INIT_TBL_CDS_EID 63
#define SEM_INVALID_ACTION_ERR_EID 64
#define SEM_INVALID_SCHLOAD_ERR_EID 65
#define SEM_MANAGE_TBL_ERR_EID		66

#define SEM_EVT_CNT  24

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

#endif /* _SEM_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file sem_private_ids.h
**=====================================================================================*/
    
