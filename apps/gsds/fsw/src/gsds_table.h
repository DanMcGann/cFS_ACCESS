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
** File Name:  gsds_table.h
**
** Title:  Specifications for GSDS Tables
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This file provides the specifications for all GSDS table data and functions
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


#ifndef _GSDS_TABLE_H_
#define _GSDS_TABLE_H_

/*
 * Includes
 */
#include "cfe.h"
#include "gsds_platform_cfg.h"

/*
**	Application Format Table Flags
*/
/*	\brief Ground Station Identification Data Flag
**	\par Description: This flag is used in the GSDS GS Format Table to indicate
**		 the format table field containing Ground Station identification information
*/
#define GSDS_ID_FLAG 1
/*	\brief Ground Station Status Flag
**	\par Description: This flag is used in the GSDS GS Format Table to indicate
**		 the format table field containing Ground Station status(Enable or Disable)
*/
#define GSDS_STATUS_FLAG 2


/*	\brief GSDS Ground Station Entry
**
**	\par Description:
**		Buffer containing the format table defined ground station data.
*/
typedef struct {

	uint8 	IsUsed;						/**< \brief Is this entry being used to store data */

	char 	Data[GSDS_MAX_DATA_LEN];	/**< \brief Buffer containing format table specified data on the ground station */

} GSDS_GSEntry_t;

/*	\brief GSDS State Table
**
**	\par Description:
**		Table containing all information on ground stations accessible to this satellite
**		This data is intended to be updated from uplink from the ground, and accessed
**		by request to be used by the satellite planning algorithms.
*/
typedef struct {

	uint16 NumEntries; /**< \brief The number of used entries in the GS State table */

	GSDS_GSEntry_t Stations[GSDS_MAX_GS]; /**< \brief The list of ground stations*/

} GSDS_StateTbl_t;



/****************************************************************************************
**	Table Function Prototypes
****************************************************************************************/

/* \brief Ground Station State Table Initialization
**
** \par Description:
**		Initializes GS State Table with default table image (as defined in platform_cfg
**		file). Registers and loads this table in accordance with CFE Table guidelines.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt GSDS_INIT_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 GSDS_InitTable(void);

/* \brief Ground Station Format Table Initialization
**
** \par Description:
**		Initializes GS Format Table with default table image (as defined in platform_cfg
**		file). Registers and loads this table in accordance with CFE Table guidelines.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt GSDS_TBLINIT_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 GSDS_InitFrmtTable(void);


/* \brief GS State Table Verification
**
** \par Description:
**		Verifies the GS State table for proper data. Table verification is required
**		by CFE table services
**
**	\param [in] TblPtr - Pointer to the GS State Table
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt GSDS_VERIY_TBL_ERR_EID - If there is an error verifying the table
**	\endreturns
*/
int32 GSDS_VerifyStateTable(void* TblPtr);

/* \brief GS Format Table Verification
**
** \par Description:
**		Verifies the GS format table. Uses Format table API to provide common
**		format table verification.
**
**	\param [in] TblPtr - Pointer to the GS Format Table
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt GSDS_VERIY_TBL_ERR_EID - If there is an error verifying the table
**	\endreturns
*/
int32 GSDS_VerifyFrmtTable(void* TblPtr);

/* \brief Gets the pointers for GSDS Tables
**
** \par Description:
**		Grabs the pointers for te GSDS State table and Format Table. Does so
**		Through the CFE_TBL API.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt A CFE_TBL Error Code if there is an error getting the pointers
**	\endreturns
*/
int32 GSDS_GrabPtrs(void);

/* \brief Releases the pointers for GSDS Tables
**
** \par Description:
**		Drops the pointers for the GSDS State table and Format Table. Does so
**		Through the CFE_TBL API.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt A CFE_TBL Error Code if there is an error dropping the pointers
**	\endreturns
*/
int32 GSDS_DropPtrs(void);

/* \brief Adds a ground station to the existing table image.
**
** \par Description:
**		Adds a ground station to the existing table image. By Populating the first
**		Unused table entry with the given information, and copying format table
**		defined data into the table.
**
**
**	\param [in]	char* data - Pointer to the ground station informational data to be
**			added to the table
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt GSDS_ADDGS_ERR_EID - If there is a problem adding the ground station
**	\endreturns
*/
int32 GSDS_AddGs(char* Data);

/* \brief Retrieves the specified ground station data
**
** \par Description:
**		Copies the ground station data from the table into the given pointer.
**
**	\param [in] uint32 id - Unique id of the ground station to retrieve data from
**	\param [in]	char* data - Pointer to the location where the ground station data should
**	be saved to.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registration and load are successful
**		\retstmt GSDS_GETGGSDATA_ERR_EID - If there is a problem adding the ground station
**	\endreturns
*/
int32 GSDS_GetGsId(char * id, char* retptr);

/* \brief Retrieves Ground Station For all requested Ground Stations
**
** \par Description:
**		Copies all ground stations who's data field flagged matched the data
**		located in equal. and returns them as a array of ground stations. If
**		flag is set to FRMT_TBL_NOFLAG the will copy a list of all ground stations
**		into the given location
**
**	\par Assumptions: The data location pointed to by char should be large enough to
**		contain up to NumEntries ground stations.
**
**	\par Notes: This function is used to get all ground stations that have an enabled
**		status. But can be used to get ground stations that match any field
**		information. This could be data like Country Locates, or Data Transfer rate
**		(anything that is saved as part of ground station data - as defined
**		in the ground station format table)
**
**	\param [in] uint32 flag - flag of field containing data to match
**	\param [in]	char* equal - Data field equality value.
**	\param [in]	char* data - Pointer to the location where the ground station data should
**	be saved to.
**
**	\returns
**		\retstmt If Successful the number of ground stations matching
**	\endreturns
*/
int32 GSDS_GetGsAllMatch(uint8 flag, char equal[GSDS_MAX_DATA_LEN], char* retptr);

#endif /* _GSDS_TABLE_H_ */
