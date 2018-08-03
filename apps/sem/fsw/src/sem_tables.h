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
** File Name:  sem_tables.h
**
** Title:  Header File for SEM Application Tables
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  To define SEM's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#ifndef _SEM_TABLES_H_
#define _SEM_TABLES_H_

/****************************************************************************************
* Includes
****************************************************************************************/
#include "cfe.h"
#include "sem_tbldefs.h"
/****************************************************************************************
*
*
* Table Function Definitions
*
*
****************************************************************************************/

/****************************************************************************************
* Table Initialization
****************************************************************************************/
/* \brief Initializes the SEM Tables
**
** \par Description:
**		Initializes the SEM tables. This includes registering and loading the Action Table
**		and the Schedule table, and requesting share access for the Sat State Format Table
**		(Owned by State aggregator Application)
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_INIT_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_InitTables(void);

/* \brief Initializes the SEM Action Table
**
** \par Description:
**		Initializes the SEM Action Table by registering the table with Table Services, and
**		loading the configured table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_INIT_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_InitActionTable(void);

/* \brief Initializes the SEM Schedule Table
**
** \par Description:
**		Initializes the SEM Schedule Table by registering the table with Table Services, and
**		loading the configured table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_INIT_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_InitScheduleTable(void);

/* \brief Initializes the SEM Plan Format Table
**
** \par Description:
**		Initializes the SEM Plan Format Table by registering the table with Table Services,
**		and loading the configured table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_INIT_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_InitPlanFrmtTable(void);

/****************************************************************************************
* Table Management Functions
****************************************************************************************/

/* \brief Manages all of SEM's tables
**
** \par Description:
**		Manages SEM's action and schedule tables, analizing them for validation requests,
**		and table loads.
**		NOTE: Schedule Table can only be loaded while not currently being executed.
*/
int32 SEM_ManageTables(void);

/****************************************************************************************
*
* Table Validation Definitions
*
****************************************************************************************/

/* \brief Verifies the SEM Action Table
 *
 * \param TblPtr - pointer to the table to verify
**
** \par Description:
**		Validates the data stores in the given pointer to verify that it conforms to
**		the expected Action Table Format.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_VERIFY_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_VerifyActionTable(void* TblPtr);

/* \brief Verifies the SEM Schedule Table
 *
 * 	\param param TblPtr - pointer to the table to verify
**
** \par Description:
**		Validates the data stores in the given pointer to verify that it conforms to
**		the expected Action Table Format.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_VERIFY_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_VerifyScheduleTable(void* TblPtr);

/* \brief Verifies the SEM Plan Format Table
 *
 * 	\param param TblPtr - pointer to the table to verify
**
** \par Description:
**		Validates the data stores in the given pointer to verify that it conforms to
**		the expected Plan Format table.
**
**	\returns
**		\retstmt CFE_SUCCESS - If registrations and loads are successful
**		\retstmt SEM_VERIFY_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_VerifyPlanFrmtTable(void* TblPtr);


/****************************************************************************************
*
* Table Pointer Functions
*
****************************************************************************************/

/* \brief Gets the SEM Schedule Table Pointers
**
** \par Description:
**		Retrieves and saves in AppData the SEM table pointers
**
**	\returns
**		\retstmt CFE_SUCCESS - If retrieval of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_GrabScheduleTblPtr(void);

/* \brief Gets the SEM Action Table Pointer
**
** \par Description:
**		Retrieves and saves in AppData the SEM Action table pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If retrieval of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_GrabActionTblPtr(void);

/* \brief Gets the SEM Plan Format Table Pointer
**
** \par Description:
**		Retrieves and saves in AppData the SEM Plan Format table pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If retrieval of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_GrabPlanFrmtTblPtr(void);

/* \brief Gets the Satellite State Format Table Pointer
**
** \par Description:
**		Retrieves and saves in AppData Satellite State Format Table Pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If retrieval of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_GrabSatStateFrmtPtr(void);

/* \brief Drops the SEM Schedule Table Pointer
**
** \par Description:
**		Releases and saves in AppData the SEM Schedule table pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If release of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_DropScheduleTblPtr(void);

/* \brief Drops the SEM Action Table Pointer
**
** \par Description:
**		Releases and saves in AppData the SEM Action table pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If release of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_DropActionTblPtr(void);

/* \brief Drops the SEM Plan Format Table Pointer
**
** \par Description:
**		Releases and saves in AppData the SEM Plan Format table pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If release of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error
**	\endreturns
*/
int32 SEM_DropPlanFrmtTblPtr(void);

/* \brief Drops the Satellite State Format Table Pointer
**
** \par Description:
**		Releases and saves in AppData Satellite State Format Table Pointer
**
**	\returns
**		\retstmt CFE_SUCCESS - If release of pointers was successful
**		\retstmt SEM_GRABPTRS_TBL_ERR_EID - If there is an error initializing the table
**	\endreturns
*/
int32 SEM_DropSatStateFrmtPtr(void);


/****************************************************************************************
*
* Conditional Functions
*
****************************************************************************************/
/* \brief Checks if the given action id is valid
**
** \par Description:
**		Searched the SEM Action table for an entry with a matching action if.
**
**	\returns
**		\retstmt TRUE iff the ActionId is found in the SEM Action Table
**	\endreturns
*/
boolean SEM_IsValidActionId(SEM_ActionId_t id);

/* \brief Check if this plan is an override plan
**
** \par Description:
**		looks at the first entry of the plan (if there is one), for the
**		SEM_OVERRIDE_PLAN_FLAG. Returns the boolean value of the data located
**		in the specified field. If the flag is absent, returns false.
**
**	\returns
**		\retstmt TRUE iff the OVERRIDE_PLAN marked field of the first plan
**		entry evaluates to true.
**	\endreturns
*/
boolean SEM_IsOverridePlan(void* PlanPtr, int32 Len);

/****************************************************************************************
*
* Table Query Functions
*
****************************************************************************************/
/* \brief Returns the Scheduled action
**
** \par Description:
**		Searches the SEM Schedule Table for the next scheduled action if there is one.
**
**	\returns
**		\retstmt ActionId_t - the id of the action to execute
**		\retstmt SEM_NO_ACTION - Iff there is no action to execute.
**	\endreturns
*/
SEM_ActionId_t SEM_GetScheduledAction(void);


/* \brief Returns a pointer to the specified action
**
** \par Description:
**		Searches the SEM Action table for the action with the specified id if it exists.
**		fails if the given id does not correspond to a saved action
**
**	\par Assumption:
**		The calling function or program has grabbed the action table pointer and
**		is holding it for the duration of the execution of this function.
**
**	\returns
**		\retstmt SEM_ActionTblEntry_t * - Pointer to the action
**		\retstmt NULL - Iff there is no action that corresponds to the given action Id.
**	\endreturns
*/
SEM_ActionTblEntry_t * SEM_GetAction(SEM_ActionId_t Id);

/****************************************************************************************
*
* Table Modification Functions
*
****************************************************************************************/
/* \brief Sets the Schedule Entry to the given status
**
** \par Description:
**		If the schedule Table index given is in bounds, sets its status flag to the
**		given status flag.
**
*/
void SEM_SetSchStatus(int32 index, enum SEM_ActionStatus_t status);

/* \brief Translates a satellite plan and loads the corresponding schedule
**
** \par Description:
**		Using the Plan Format Table and the given satellite plan
**		Constructs a Schedule and loads that schedule into the schedule table.
**
**
**	\returns
**		\retstmt CFE_SUCCESS - on nominal operation
**		\retstmt
**	\endreturns
*/
int32 SEM_TranslateAndLoadPlan(void* PlanPtr, int32 PlanDataLen);

#endif /* _SEM_TABLES_H_ */
