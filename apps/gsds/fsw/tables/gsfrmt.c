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
*	File: gsfrmt.c
*
*	Purpose: Defines default ground station format
*
*	Notes: Mission Software developer should modify this table for mission
*	specific ground station data structure information
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
FRMT_TBL_t GSDS_GsFtmtTbl =
{ 1, 	/* Number of entries in this format table */
		{
			/*
			 * Data Feild 0
			 */
			{
					0,				/* FeildId - Unique Identification of field */
					0,				/* Offset - Offset (in bytes) */
					32,				/* Size - The size of this data */
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
CFE_TBL_FILEDEF(GSDS_GsFtmtTbl, // @suppress("Unused variable declaration in file scope")
		GSDS_APP.GSFRMT,
		GSDS Gs Format Table,
		gsfrmt.tbl)

/**********************
* End of File Comment *
**********************/
