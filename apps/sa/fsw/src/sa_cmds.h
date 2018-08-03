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
** File Name:  sa_cmds.h
**
** Title:  Command Header File for SA Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-18
**
** Purpose:  This header file contains declarations and definitions of SA's
** 	Cmd Functions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-18 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/


#ifndef _SA_CMDS_H_
#define _SA_CMDS_H_

#include "cfe.h"


/* \brief Data Request Command Function
**
** \par Description:
**		Sends out the Satellite State Data in a satellite State Data Msg
*/
void SA_SatStateDataReqCmd(CFE_SB_MsgPtr_t MsgPtr);


#endif /* _SA_CMDS_H_ */
