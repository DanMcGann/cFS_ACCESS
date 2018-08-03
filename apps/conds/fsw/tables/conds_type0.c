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
*	File: conds_type0_tbl.c
*
*	Purpose: Defines default constellation state table
*
*	Notes: Mission Software developer should modify this table for mission
*	specific constellation state information.
*
*	Author: Daniel McGann
******************************************************************************/

/*
 * Includes
 */
#include "cfe.h"
#include "access_lib.h"
#include "cfe_tbl_filedef.h"

/*
 * Default Values
 */



/*
 * Default Format Table
 */
FRMT_TBL_t CONDS_Type0Table =
{ 1, 	/* Number of entries in this format table */
		{
			/*
			 * Data Feild 0
			 */
			{
					0,				/* FeildId - Unique Identification of field */
					0,				/* Offset - Offset (in bytes) */
					0,				/* Size - The size of this data */
					FRMT_TBL_NOFLAG	/* Application Specific Flag  */
			}

			/*
			 * Add Additional Fields as needed
			 */
		}
};

/*
 * Table Header
 */
CFE_TBL_FILEDEF(CONDS_Type0Table, // @suppress("Unused variable declaration in file scope")
		CONDS_APP.TYPE0,
		CONDS Data Type 0 Table,
		conds_type0.tbl)

/**********************
* End of File Comment *
**********************/
