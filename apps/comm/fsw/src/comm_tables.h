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
** File Name:  comm_tables.h
**
** Title:  Header File for COMM Application table functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define functional prototypes for the COMM application table functions,
** 		and to provide the table data defintions for the COMM Application tables
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/


#ifndef _COMM_TABLES_H_
#define _COMM_TABLES_H_

/* Includes */
#include "cfe.h"
#include "comm_msg.h"
#include "comm_linkdefs.h"


/****************************************************************************************
*
* COMM Table Data Definitions
*
****************************************************************************************/

/* \brief Data description for a file to link */
typedef struct {
	boolean Used; /*<< \brief Indicates that this file is a currently used table entry */

	boolean RetainFlag; /*<< \brief Marks that this file should be linked on every link */

	boolean DeleteOnLink; /* << \brief Flag that specifies whether or not the file should
	 	 	 	 	 	 	 	 be removed from the file system after a successful link*/

	uint8 LinkAttempts; /* << \brief Number of links this file has persisted without output */

	uint8 MaxLinkAttempts; /* << \brief Number of links to attempt before this file is removed */

	uint8 Priority; /*<< \brief the importance priority of linking this file 1 greatest priority */

	char File[OS_MAX_PATH_LEN]; /* << brief The path and name of the file to be linked */

} COMM_LinkFile_t;


/****************************************************************************************
*
* COMM Table Functions - Standard
*
****************************************************************************************/

/****************************************************************************************
* \brief Initializes COMM Tables
*
* \par Description:
* 	Registers and loads default COMM XLink and DLink Tables.
*
* \returns
* 	\retstmt CFE_SUCCESS - If all operations occur without error
* 	\retstmt CFE_TBL error core - if an error occurs
* \endreturns
****************************************************************************************/
int32 COMM_InitTables();

/****************************************************************************************
* \brief Verifies the COMM DLink Table
*
* \par Description:
* 	Verifies that the data contained in the COMM DLink table is valid
*
* \returns
* 	\retstmt CFE_SUCCESS - If the table is valid
* 	\retstmt COMM_VERIFYTBL_ERR_EID - if the table is not valid
* \endreturns
****************************************************************************************/
int32 COMM_VerifyDLinkTbl();

/****************************************************************************************
* \brief Verifies the COMM XLink Table
*
* \par Description:
* 	Verifies that the data contained in the COMM XLink table is valid
*
* \returns
* 	\retstmt CFE_SUCCESS - If the table is valid
* 	\retstmt COMM_VERIFYTBL_ERR_EID - if the table is not valid
* \endreturns
****************************************************************************************/
int32 COMM_VerifyXLinkTbl();

/****************************************************************************************
* \brief Verifies the COMM Link Table Data
*
* \par Description:
* 	Verifies that the data contained in the COMM XLink table is valid
*
* \returns
* 	\retstmt CFE_SUCCESS - If the table is valid
* 	\retstmt COMM_VERIFYTBL_ERR_EID - if the table is not valid
* \endreturns
****************************************************************************************/
int32 COMM_VerifyLinkTbl(COMM_LinkFile_t* TblPtr, int32 Length);

/****************************************************************************************
* \brief Gets the pointers for the COMM DLink Table
*
* \par Description:
* 	Using CFE Table services grabs the pointers and semephore for both the DLink
* 	table
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_GRABPTR_ERR_EID - if the table is not valid
* \endreturns
****************************************************************************************/
int32 COMM_GrabDLinkPtrs();


/****************************************************************************************
* \brief Releases the pointers for the COMM DLink Table
*
* \par Description:
* 	Using CFE Table services drops the pointers and semephore for the DLink
* 	table
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_DROPPTR_ERR_EID - if the pointers were dropped successfully
* \endreturns
****************************************************************************************/
int32 COMM_DropDLinkPtrs();

/****************************************************************************************
* \brief Gets the pointers for the COMM XLink Table
*
* \par Description:
* 	Using CFE Table services grabs the pointers and semephore for both the XLink
* 	table
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_GRABPTR_ERR_EID - if the table is not valid
* \endreturns
****************************************************************************************/
int32 COMM_GrabXLinkPtrs();


/****************************************************************************************
* \brief Releases the pointers for the COMM XLink Table
*
* \par Description:
* 	Using CFE Table services drops the pointers and semephore for the XLink
* 	table
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_DROPPTR_ERR_EID - if the pointers were dropped successfully
* \endreturns
****************************************************************************************/
int32 COMM_DropXLinkPtrs();


/****************************************************************************************
*
* COMM Table Functions - Specific
*
****************************************************************************************/

/****************************************************************************************
* \brief Adds the Specified File to the specified Link Table
*
* \par Description: Inserts the Specified file into the first available spot in the
* 	specified COMM Application Link table if a slot exists. If the file is marked priority
* 	1 it will overwrite the file entry with the lowest priority in the table.
*
* 	Once the file has been placed the entry flags are set accordingly.
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_ADDFILE_ERR_EID - if they file could not be added
* \endreturns
****************************************************************************************/
int32 COMM_AddFile(COMM_LinkType_t Link, boolean Retain, boolean DelOnLink, uint8 Priority, char File[OS_MAX_PATH_LEN]);

/****************************************************************************************
* \brief Gets the filename for the specified entry
*
* \par Description: If the index is valid returns the filename for the entry at
* 	the specified index.
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM Err Id - If the operation fails
* \endreturns
****************************************************************************************/
int32 COMM_GetFileName(COMM_LinkType_t Link, int32 Index, char RetFile[OS_MAX_PATH_LEN]);


/****************************************************************************************
* \brief Returns the table entry containing the specified file
*
* \par Description: If the file is listed in the specified link table
* 	returns the index of the entry.
*
* \returns
* 	\retstmt the Index of the file if it exists
* 	\retstmt -1 of the fi
* \endreturns
****************************************************************************************/
int32 COMM_GetFileEntry(COMM_LinkType_t Link, char GetFile[OS_MAX_PATH_LEN]);

/****************************************************************************************
* \brief Updates the link table to reflect that there is a link attempt for the file
*
* \par Description: Updates the file entry for the specified file in the specified link
* 	table to update its status information to reflect the fact that the satellite is
* 	attempting to send the file during the currently executing link.
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM Err Id - If the operation fails
* \endreturns
****************************************************************************************/
int32 COMM_UpdateEntry(COMM_LinkType_t Link, int32 Index);


/****************************************************************************************
* \brief Updates the File Table based on the the ack response from the link target.
*
* \par Description: Updates the File Table based on the the ack response from the link target.
* 	If the link is closed without receiving the ack message all files are assumed to
* 	hav e failed transfer.Additionally, this function handles housekeeping functions
* 	for the specified table completing the following tasks.
* 		- Removes any successfully linked entries, and their files if needed
* 		- Removes any stale entries (Entries whose link attempt counter has surpasses their
* 		max link attempt counters, and removes their file if necessary
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_HANDLEACK_ERR_EID - if they file could not be added
* \endreturns
****************************************************************************************/
int32 COMM_HandleAck(COMM_LinkType_t Link, COMM_AckMsg_t* Ack);


/****************************************************************************************
* \brief Organizes the specified Link Table before and after link events
*
* \par Description: Updates and the specified table in preparation of the satellite
* 	Performing a link operation. This consists of handling the following tasks...
* 		- Sorts the link table on priority.
*
* \returns
* 	\retstmt CFE_SUCCESS - If successful
* 	\retstmt COMM_PRELINKUPDATE_ERR_EID - if they file could not be added
* \endreturns
****************************************************************************************/
int32 COMM_PreLinkUpdate(COMM_LinkType_t Link);

/****************************************************************************************
* \brief Organizes the specified Link Table before and after link events
*
* \par Description: Compares two file entries based on their priority. Lower numbers in
* 	priority are considered more urgent. Additionally, an entry marked unused is always
* 	considered less urgent than a a used entry.
*
* \returns
* 	\retstmt -1 - If entry B < entry A in regards to priority
* 	\retstmt  0 - If entry A = entry B if regards to priority
* 	\retstmt +1 - If entry A > Entry B in regards to priority
* \endreturns
****************************************************************************************/
int32 COMM_PriorityComp(const void* A, const void* B);


#endif /* _COMM_TABLES_H_ */
