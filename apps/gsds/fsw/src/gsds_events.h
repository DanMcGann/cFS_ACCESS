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
** File Name:  gsds_private_ids.h
**
** Title:  ID Header File for GSDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This header file contains declarations and definitions of GSDS's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _GSDS_PRIVATE_IDS_H_
#define _GSDS_PRIVATE_IDS_H_

/*
** Include Files
*/

/*
** Local Defines
*/

/* Event IDs */
#define GSDS_RESERVED_EID  0

#define GSDS_INF_EID        1
#define GSDS_INIT_INF_EID   2
#define GSDS_ILOAD_INF_EID  3
#define GSDS_CDS_INF_EID    4
#define GSDS_CMD_INF_EID    5
#define GSDS_TBLINIT_INF_EID    6
#define GSDS_VERIY_TBL_INF_EID 	7

#define GSDS_ERR_EID         51
#define GSDS_INIT_ERR_EID    52
#define GSDS_ILOAD_ERR_EID   53
#define GSDS_CDS_ERR_EID     54
#define GSDS_CMD_ERR_EID     55
#define GSDS_PIPE_ERR_EID    56
#define GSDS_MSGID_ERR_EID   57
#define GSDS_MSGLEN_ERR_EID  58
#define GSDS_TBLINIT_ERR_EID 	59
#define GSDS_VERIY_TBL_ERR_EID 	60
#define GSDS_ADDGS_ERR_EID 		61
#define GSDS_GETGGSDATA_ERR_EID 62
#define GSDS_GRABPTRS_ERR_EID 	63
#define GSDS_MANAGE_TBL_ERR_EID 64

#define GSDS_EVT_CNT  22


/*
** Local Variables
*/

#endif /* _GSDS_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file gsds_private_ids.h
**=====================================================================================*/
    
