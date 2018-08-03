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
** File Name:  comm_exec.h
**
** Title:  Header File for COMM Application Executive functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define functional prototypes for the COMM application Executive
** 				functions.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#ifndef _COMM_EXEC_H_
#define _COMM_EXEC_H_

#include "cfe.h"
#include "comm_linkdefs.h"

/****************************************************************************************
* \brief Facilitates the transfer of files for the listed link.
*
* \par Description: This function gets all files recorded in the specified link table
* 		and commands CF to transfer each file to the link target. Tracks the size of
* 		data being transfered and compares this to the total amount of data that can be
* 		transfered based on the known radio bandwidth, and the given duration
*
* \returns
* 	\retstmt CFE_SUCCESS - If all operations function nominally
* 	\retstmt  COMM_INITLINK_ERR_EID - if some operation fails
* \endreturns
****************************************************************************************/
int32 COMM_InitLink(COMM_LinkType_t Type, CFE_TIME_SysTime_t LinkDur);

#endif /* _COMM_EXEC_H_ */
