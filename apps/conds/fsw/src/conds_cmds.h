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
/******************************************************************************
* 	File: conds_cmds.h
*
* 	Purpose:
* 		This File provides the definitions for all CONDS functions
*
*  Created on: Mar 28, 2018
*      Author: da27914
*******************************************************************************/

#ifndef _CONDS_CMDS_H_
#define _CONDS_CMDS_H_

#include "conds_app.h"

/*******************************************************************************
**	\brief Reset the CONDS Housekeeping Counters
**
**	\par Description: This Function has no effect other than to increment
**			the CONDS Housekeeping counters depending on whether the given
**			Message pointer is valid or not.
**
**	\par Assumptions, External Events, Notes, Etc.
**		(none)
********************************************************************************/
void CONDS_NoopCmd(CFE_SB_Msg_t * MsgPtr);

/*******************************************************************************
**	\brief Reset the CONDS Housekeeping Counters
**
**	\par Description: This Function resets all housekeeping counters for the
**			CONDS application, assigning them back to a value of 0.
**
**	\par Assumptions, External Events, Notes, Etc.
**		(none)
********************************************************************************/
void CONDS_ResetCmd(CFE_SB_Msg_t * MsgPtr);

/*******************************************************************************
**	\brief Sends The Requested Constellation State Data
**
**	\par Description: This Function gets the specified satellite State
**			Data from its location in memory, copies this into a SB msg
**			and sends that message on the CFE Software Bus.
**
**	\par Assumptions, External Events, Notes, Etc.
**		(none)
********************************************************************************/
void CONDS_DataReqCmd(CFE_SB_Msg_t * MsgPtr);



#endif /* _CONDS_CMDS_H_ */
