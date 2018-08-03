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
** File Name:  sa_table.h
**
** Title:  Specifications for SA Tables
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This file provides the specifications for all SA table data and functions
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
#include "cfe.h"


#ifndef _SA_TABLES_H_
#define _SA_TABLES_H_


/* \brief Satellite State Format Table Initialization
**
** \par Description:
**		Initializes the SA satstate Format Table, registers this format table with
**		CFE Table services and loads the default SatState Table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt SA_INIT_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SA_InitFrmtTable(void);

/* \brief Ground Station State Table Verification
**
** \par Description:
**		Verify the contents of the SatFrmt Table
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt SA_VERIFY_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SA_VerifySatFrmtTable(void* TblPtr);

/* \brief Gets the pointers for SA Tables
**
** \par Description:
**		Grabs the pointers for the SatFrmt Table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt A CFE_TBL Error Code if there is an error getting the pointers
**	\endreturns
*/
int32 SA_GrabPtrs(void);

/* \brief Dropse the pointers for SA Tables
**
** \par Description:
**		Drops the pointers for the SatFrmt Table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt A CFE_TBL Error Code if there is an error getting the pointers
**	\endreturns
*/
int32 SA_DropPtrs(void);

/* \brief Allocates mem pool space for sat state data
**
** \par Description:
**		Allocates a memory pool block to store the satellite state data
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt A CFE_ES Error Code if there is an error getting the pointers
**	\endreturns
*/
int32 SA_AllocMem(void);

/* \brief Frees mem pool space for sat state data
**
** \par Description:
**		Frees a memory pool block to store the satellite state data
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt A CFE_ES Error Code if there is an error getting the pointers
**	\endreturns
*/
int32 SA_FreeMem(void);


/* \brief Checks if the given Message Id is a Corresponds to state data
**
** \par Description:
**		Returns true iff the given message id, matches a application flag of
**		a field in the SatFrmt Table
**
**	\returns True iff id exists.
*/
boolean SA_IsValidMsgIdFlag(int32 MsgId);

/* \brief Subscribes to all Satellite State Msg Ids
**
** \par Description:
**		Subscribes to all message ids listed as application flags in the
**		SatFrmtTable
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt The number of unsuccessful subscriptions
**	\endreturns
*/
int32 SA_SubSatStateMsgs(void);

/* \brief UnSubscribes to all Satellite State Msg Ids
**
** \par Description:
**		UnSubscribes to all message ids listed as application flags in the
**		SatFrmtTable
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt The number of unsuccessful unsubscriptions
**	\endreturns
*/
int32 SA_UnSubSatStateMsgs(void);


#endif /* _SA_TABLES_H_ */
