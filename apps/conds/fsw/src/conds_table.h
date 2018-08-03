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
/*******************************************************************************
*   File: conds_table.h
*
*   Purpose: This header file provides the definitions for all Constellation 
*       Data Store table data and functions
*
*
*   Author: Daniel McGann
*
*******************************************************************************/
#ifndef _CONDS_TABLE_H_
#define _CONDS_TABLE_H_

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "cfe.h"
#include "access_lib.h"
#include "conds_platform_cfg.h"

/*******************************************************************************
*   Table Data Structures
*******************************************************************************/

/*  \brief Data type for Constellation State Table Entry */
typedef struct {
    char                FrmtName[OS_MAX_API_NAME];      /**< \brief Name of the data type table */
    char                FrmtFilename[OS_MAX_PATH_LEN];  /**< \brief File Name of data type format table*/
    CFE_TBL_Handle_t    FrmtHandle;                     /**< \brief Table Handle for corresponding Format Table */
    FRMT_TBL_t* 	FrmtPtr;							/**< \brief Pointer to this Format Table */
    CFE_SB_MsgId_t      MsgId;                          /**< \brief Message Id for incoming data packet */
    uint32*             DataPtr;                        /**< \brief Pointer to locally stored state data */
} CONDS_ConStateEntry_t;


/*******************************************************************************
*   Table Functions 
*******************************************************************************/

/******************************************************************************/
/*  \brief  Initializes CONDS tables, including format tables.
**
**  \par Description: This Function creates and loads the Constellation
**          State table. If this is successful, this function also initializes
**          the format tables for all data types, and subscribes to each data 
**          types message Ids.
**
**  \par Called From: #CONDS_InitApp
**
**  \par Assumptions, External Events, Notes, etc.
**      The given ConstellationS tate Table is well formed.
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_InitTables (void);

/******************************************************************************/
/*  \brief  Initializes CONDS Format tables
**
**  \par Description: This Function creates registers and loads 
**          all format tables listed in the constellation state table
**
**  \par Called From: #CONDS_InitTables
**
**  \par Assumptions, External Events, Notes, etc.
**      The given ConstellationS tate Table is well formed.
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_InitFrmtTables(void);

/******************************************************************************/
/*  \brief  Gets and saves Pointers for all Tables
**
**  \par Description: This Function gets the pointers for all tables used
**  		in the CONDS app (ConState Table and all format tables). If
**  		this function is called while the pointers are already grabbed
**  		it simply returns successful.
**
**
**  \par Assumptions, External Events, Notes, etc.
**     	(none)
**
**  \returns
**  	\retstmt CFE_SUCCESS - if all pointers were grabbed successfully
**  	\retstmt GetAddress Error - if there was an error grabbing a pointer
**  \endreturns
*/
/******************************************************************************/
int32 CONDS_GrabPtrs(void);

/******************************************************************************/
/*  \brief  Releases Pointers for all tables used in the CONDS app
**
**  \par Description: This Function releases the pointers for all tables used
**  		in the CONDS app (ConState Table and all format tables) and sets
**  		their saved locations to NULL.
**
**
**  \par Assumptions, External Events, Notes, etc.
**     	(none)
**
**  \returns
**  	\retstmt CFE_SUCCESS - if all pointers were grabbed successfully
**  	\retstmt GetAddress Error - if there was an error grabbing a pointer
**  \endreturns
*/
/******************************************************************************/
int32 CONDS_DropPtrs(void);

/******************************************************************************/
/*  \brief  Subscribes to all messages in the Constellation State Table
**
**  \par Description: This Function subscribes the CONDS telemetry pipe
**          to all message ids listed in the Constellation State Table
**
**  \par Called From: #CONDS_InitTables
**
**  \par Assumptions, External Events, Notes, etc.
**      The given ConstellationS tate Table is well formed.
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_SubscribeStateMsgs(void);

/******************************************************************************/
/*  \brief  UnSubscribes to all messages in the Constellation State Table
**
**  \par Description: This Function unsubscribes the CONDS ConState pipe
**          to all message Ids listed in the Constellation State Table
**
**  \par Called From: #CONDS_InitTables
**
**  \par Assumptions, External Events, Notes, etc.
**      The given ConstellationS state Table is well formed.
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_UnSubscribeStateMsgs(void);

/******************************************************************************/
/*  \brief  Allocates memory for data storage of Constellation State Types
**
**  \par Description: This Function allocated memory to locally store all
**          Constellation State Data Types that are defined in the 
**          Constellation State table
**
**  \par Called From: #CONDS_InitTables
**
**  \par Assumptions, External Events, Notes, etc.
**      The given ConstellationS tate Table is well formed.
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_AllocateStateMemory(void);

/******************************************************************************/
/*  \brief  Frees memory used in storage of Constellation State Types
**
**  \par Description: This Function Frees allocated memory used in storing
**          Constellation State Data
**
**  \par Called From: #CONDS_ManageConState
**
**  \par Assumptions, External Events, Notes, etc.
**      The given ConstellationS tate Table is well formed.
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_FreeStateMemory(void);

/******************************************************************************/
/*  \brief  Verifys Constellation State Table
**
**  \par Description: This function verifys the CONDS Constellation State Table.
**          As part of its verification process it manages the listed format 
**          tables which care associated with each data type entry.
**
**  \par Called From: #CFE_TBL_Manage
**
**  \par Assumptions, External Events, Notes, etc.
**      (none)
**
**  \param [in] none
*/
/******************************************************************************/
int32 CONDS_VerifyConState(void* TblData);

/******************************************************************************/
/*  \brief  Manage a CONDS owned format table
**
**  \par Description: This function manager CONDS Format tables. Loading, 
**          dumping, and updating tables when needed
**
**  \par Called From: #CONDS_VerifyConState
**
**  \par Assumptions, External Events, Notes, etc.
**      (none)
*/
/******************************************************************************/
int32 CONDS_TableManageFrmt(CFE_TBL_Handle_t TblHandle);

/******************************************************************************/
/*  \brief  Manages the Constellation State Table
**
**  \par Description: This function manager CONDS Format tables. Loading, 
**          dumping, and updating tables when needed
**
**  \par Called From: #CONDS_ReportHousekeeping
**
**  \par Assumptions, External Events, Notes, etc.
**      (none)
*/
/******************************************************************************/
void CONDS_TableManageConState(void);

/******************************************************************************/
/*  \brief  Checks if the given MsgId is valid
**
**  \par Description: This function checks to ensure that the given message id
**  		corresponds to one of the state data types listed in the
**  		Constellation State table
**
**  \par Called From: #CONDS_ProcessNewStateData
**
**  \par Assumptions, External Events, Notes, etc.
**      (none)
**
**  \returns
**  	\retstmt TRUE - if the given message id corresponds to a constellation
**  		state data type
**  	\retstmt FALSE - if the given message id does not correspond to a
**  		constellation state data type.
**  \endreturns
*/
/******************************************************************************/
boolean CONDS_IsValidStateMsgId(CFE_SB_MsgId_t MsgId);

/******************************************************************************/
/*  \brief  Saves the message data into the allocated memory
**
**  \par Description: This function saves the data from the message and saves
**  		this data into the allocated memory for the corresponding
**  		State Data Type as defined int he Constellation State Table
**
**  \par Called From: #CONDS_ProcessNewStateData
**
**  \par Assumptions, External Events, Notes, etc.
**     	(none)
**
**  \returns
**  	\retstmt CFE_SUCCESS - if the memory save was successful
**  	\retstmt CONDS_MSGID_ERR_EID - If the corresponding ConState entry
**  				cannot be found.
**  	\retstmt CONDS_SAVEDATA_ERR_EID - If no memory has been allocated
**  				for the State data, or the memory copy failed
**  \endreturns
*/
/******************************************************************************/
int32 CONDS_SaveStateData(CFE_SB_Msg_t* MsgPtr);

#endif /* _CONDS_TABLE_H_ */
