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
** File Name:  conds_private_ids.h
**
** Title:  ID Header File for CONDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-26
**
** Purpose:  This header file contains declarations and definitions of CONDS's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-26 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _CONDS_PRIVATE_IDS_H_
#define _CONDS_PRIVATE_IDS_H_

/*
** Local Defines
*/

/* Event IDs */
#define CONDS_RESERVED_EID  	0

#define CONDS_INF_EID        	1
#define CONDS_INIT_INF_EID   	2
#define CONDS_ILOAD_INF_EID  	3
#define CONDS_CDS_INF_EID    	4
#define CONDS_CMD_INF_EID    	5
#define CONDS_INIT_TBL_CDS_EID 	6
#define CONDS_TBL_INF_EID 		7

#define CONDS_ERR_EID         		51
#define CONDS_INIT_ERR_EID    		52
#define CONDS_ILOAD_ERR_EID   		53
#define CONDS_CDS_ERR_EID     		54
#define CONDS_CMD_ERR_EID     		55
#define CONDS_PIPE_ERR_EID    		56
#define CONDS_MSGID_ERR_EID   		57
#define CONDS_MSGLEN_ERR_EID  		58

#define CONDS_INIT_TBL_ERR_EID 		59
#define CONDS_SUB_STATEMSGS_ERR_EID 60
#define CONDS_TABLE_VERIFY_ERR_EID  61
#define CONDS_SAVEDATA_ERR_EID 		62
#define CONDS_DATAREQCMD_ERR_EID 	63
#define CONDS_TBLGRABPTRS_ERR_EID	64
#define CONDS_TABLE_MANAGE_ERR_EID	65
#define CONDS_ALLOCMEM_ERR_EID  	66

#define CONDS_EVT_CNT  24

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

#endif /* _CONDS_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file conds_private_ids.h
**=====================================================================================*/
    
