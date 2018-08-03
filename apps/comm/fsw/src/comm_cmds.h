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
** File Name:  comm_cmds.h
**
** Title:  Header File for COMM Application command functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define functional prototypes for the COMM application command functions.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/



#ifndef _COMM_CMDS_H_
#define _COMM_CMDS_H_

/* Includes */
#include "cfe.h"
#include "comm_msg.h"
#include "comm_app.h"


/****************************************************************************************
*
* COMM Command Functions - Standard
*
****************************************************************************************/

/****************************************************************************************
* \brief No Operation Command
*
* \par Description:
* 	Command with no operation other than to increment command counter

****************************************************************************************/
void COMM_NoopCmd(CFE_SB_MsgPtr_t MsgPtr);

/****************************************************************************************
* \brief Reset Counters Command
*
* \par Description:
* 	Command function that resets all COMM counters
****************************************************************************************/
void COMM_RstCmd(CFE_SB_MsgPtr_t MsgPtr);


/****************************************************************************************
* \brief Register File Command
*
* \par Description:
* 	Command Registers the given file in the specified link table.
* 	This is the main way that functions interact with COMM. All information to
* 	be down or cross linked should be registered once complete.
****************************************************************************************/
void COMM_RegisterFileCmd(CFE_SB_MsgPtr_t MsgPtr);

/****************************************************************************************
* \brief Start Link Command
*
* \par Description:
* 	Command that initiates the transfer of files to whatever target is currently linked
* 	with the satellite's radio system.
****************************************************************************************/
void COMM_StartLinkCmd(CFE_SB_MsgPtr_t MsgPtr);

/****************************************************************************************
* \brief Stop Link Command
*
* \par Description:
* 	Command that Stops the transfer of files to whatever target is currently linked
* 	with the satellite's radio system.
****************************************************************************************/
void COMM_StopLinkCmd(CFE_SB_MsgPtr_t MsgPtr);

/****************************************************************************************
* \brief Ack Link Command
*
* \par Description:
* 	Command that Confirms the success of transfer of files from the target system.
*
* 	Note: If the target system does not provide an acknowledgment  message, an empty
* 	should be sent to the COMM application, indicating that the success of the link
* 	is inconclusive. In this case COMM will assume that all files failed to be
* 	Received by the target system successfully.
****************************************************************************************/
void COMM_AckLinkCmd(CFE_SB_MsgPtr_t MsgPtr);






#endif /* _COMM_CMDS_H_ */
