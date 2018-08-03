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
** File Name:  gsds_cmd.h
**
** Title:  Specifications for GSDS Commands
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-12
**
** Purpose:  This file provides the specifications for all GSDS command functions
**
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

/*
 * Includes
 */
#include "cfe.h"

/************************************************************************/
/** \brief Sends out Ground Station Info based on Id
**
**  \par Description
**       Finds ground station data with matching Id, and sends that info
**       to the specified message id
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message
**
**  \sa #CFE_SB_RcvMsg
**
*************************************************************************/
void GSDS_GsReqIdCmd(CFE_SB_MsgPtr_t MsgPtr);

/************************************************************************/
/** \brief Sends out Ground Station info for all that match
**
**  \par Description
**       Finds all ground stations whos fields match the given data. Sends
**       list of ground stations to given message id.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message
**
**  \sa #CFE_SB_RcvMsg
**
*************************************************************************/
void GSDS_GsReqMatchCmd(CFE_SB_MsgPtr_t MsgPtr);

/************************************************************************/
/** \brief Saves the Ground Station
**
**  \par Description
**       Saves the given Ground Station Data to the Ground Station State
**       Table.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message
**
**  \sa #CFE_SB_RcvMsg
**
*************************************************************************/
void GSDS_AddGsCmd(CFE_SB_MsgPtr_t MsgPtr);

/************************************************************************/
/** \brief Edits the Specified Ground Station
**
**  \par Description
**       Replaces the given ground station's data with the message
**       specified data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message
**
**  \sa #CFE_SB_RcvMsg
**
*************************************************************************/
void GSDS_EditGsCmd(CFE_SB_MsgPtr_t MsgPtr);


/***************/
/* End of File */
/***************/
